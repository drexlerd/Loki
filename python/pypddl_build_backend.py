import base64
import csv
import hashlib
import os
import platform
import shutil
import subprocess
import zipfile
from pathlib import Path
from tempfile import TemporaryDirectory

from scikit_build_core import build as scikit_build


ROOT_DIR = Path(__file__).resolve().parent.parent


def _num_jobs() -> int:
    return int(os.environ.get("PYPDDL_JOBS", "8"))


def _prepend_cmake_args(*args: str) -> None:
    existing = os.environ.get("CMAKE_ARGS", "")
    os.environ["CMAKE_ARGS"] = " ".join([*args, existing]).strip()


def _yggdrasil_prefix() -> Path:
    import pyyggdrasil

    return pyyggdrasil.native_prefix().resolve()


def _prepare_native_build() -> None:
    os.environ.setdefault("CMAKE_BUILD_PARALLEL_LEVEL", str(_num_jobs()))
    _prepend_cmake_args(
        f"-DCMAKE_PREFIX_PATH={_yggdrasil_prefix()}",
        "-DBUILD_SHARED_LIBS=ON",
        "-DBUILD_PYPDDL=ON",
        "-DLOKI_BUILD_TESTS=OFF",
        "-DLOKI_BUILD_BENCHMARKS=OFF",
        "-DLOKI_BUILD_EXECUTABLES=OFF",
        "-DLOKI_LINK_STATIC_DEPENDENCIES=OFF",
        "-DCMAKE_INSTALL_LIBDIR=lib",
    )


def _is_disabled(value: str) -> bool:
    return value.upper() in {"0", "FALSE", "OFF", "NO"}


def _is_native_library(path: Path) -> bool:
    name = path.name
    return ".so" in name or name.endswith(".dylib") or name.endswith(".pyd")


def _strip_args() -> list[str]:
    if platform.system() == "Darwin":
        return ["-x"]
    return ["--strip-unneeded"]


def _record_digest(path: Path) -> tuple[str, str]:
    content = path.read_bytes()
    digest = base64.urlsafe_b64encode(hashlib.sha256(content).digest()).rstrip(b"=").decode("ascii")
    return f"sha256={digest}", str(len(content))


def _rewrite_record(wheel_root: Path) -> None:
    record_files = list(wheel_root.glob("*.dist-info/RECORD"))
    if len(record_files) != 1:
        raise RuntimeError(f"expected exactly one RECORD file, found {len(record_files)}")

    record_file = record_files[0]
    rows = []
    for path in sorted(wheel_root.rglob("*")):
        if not path.is_file():
            continue

        relative_path = path.relative_to(wheel_root).as_posix()
        if path == record_file:
            rows.append((relative_path, "", ""))
        else:
            digest, size = _record_digest(path)
            rows.append((relative_path, digest, size))

    with record_file.open("w", newline="") as out:
        csv.writer(out).writerows(rows)


def _strip_wheel_native_libraries(wheel_path: Path) -> None:
    if _is_disabled(os.environ.get("PYPDDL_STRIP_WHEEL", "ON")):
        return

    strip = shutil.which("strip")
    if strip is None:
        return

    with TemporaryDirectory(prefix="pypddl-wheel-") as tmp:
        wheel_root = Path(tmp) / "wheel"
        with zipfile.ZipFile(wheel_path) as wheel:
            wheel.extractall(wheel_root)

        for path in wheel_root.rglob("*"):
            if path.is_file() and _is_native_library(path):
                subprocess.run(
                    [strip, *_strip_args(), str(path)],
                    check=False,
                    stdout=subprocess.DEVNULL,
                    stderr=subprocess.DEVNULL,
                )

        _rewrite_record(wheel_root)

        replacement_path = wheel_path.with_suffix(".tmp")
        with zipfile.ZipFile(replacement_path, "w", compression=zipfile.ZIP_DEFLATED) as wheel:
            for path in sorted(wheel_root.rglob("*")):
                if path.is_file():
                    wheel.write(path, path.relative_to(wheel_root).as_posix())

        replacement_path.replace(wheel_path)


def get_requires_for_build_wheel(config_settings=None):
    return scikit_build.get_requires_for_build_wheel(config_settings)


def get_requires_for_build_editable(config_settings=None):
    return scikit_build.get_requires_for_build_editable(config_settings)


def prepare_metadata_for_build_wheel(metadata_directory, config_settings=None):
    return scikit_build.prepare_metadata_for_build_wheel(metadata_directory, config_settings)


def build_wheel(wheel_directory, config_settings=None, metadata_directory=None):
    _prepare_native_build()
    wheel_filename = scikit_build.build_wheel(wheel_directory, config_settings, metadata_directory)
    _strip_wheel_native_libraries(Path(wheel_directory) / wheel_filename)
    return wheel_filename


def build_editable(wheel_directory, config_settings=None, metadata_directory=None):
    _prepare_native_build()
    return scikit_build.build_editable(wheel_directory, config_settings, metadata_directory)


def build_sdist(sdist_directory, config_settings=None):
    return scikit_build.build_sdist(sdist_directory)
