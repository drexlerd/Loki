from importlib import import_module as _import_module
from importlib.metadata import PackageNotFoundError, version
from pathlib import Path

# Load native dependency packages before this package loads native extensions.
_import_module("pyyggdrasil")

from . import formalism as formalism


def _source_version() -> str:
    for parent in Path(__file__).resolve().parents:
        pyproject = parent / "pyproject.toml"
        if not pyproject.exists():
            continue

        for line in pyproject.read_text(encoding="utf-8").splitlines():
            if line.startswith("version"):
                return line.split("=", maxsplit=1)[1].strip().strip("\"")

    return "0.0.0"


try:
    __version__ = version("pypddl")
except PackageNotFoundError:
    __version__ = _source_version()


def native_prefix() -> Path:
    package_dir = Path(__file__).resolve().parent
    native_dir = package_dir / "native"
    if (native_dir / "include" / "loki").is_dir():
        return native_dir
    for parent in package_dir.parents:
        if (parent / "include" / "loki").is_dir() and any((parent / name / "cmake" / "loki").is_dir() for name in ("lib", "lib64")):
            return parent
    return native_dir


def native_include_dir() -> Path:
    return native_prefix() / "include"


def native_lib_dir() -> Path:
    native_dir = native_prefix()
    for lib_dir_name in ("lib", "lib64"):
        lib_dir = native_dir / lib_dir_name
        if lib_dir.is_dir():
            return lib_dir
    return native_dir / "lib"


def cmake_prefix() -> Path:
    """Return the prefix to put on CMAKE_PREFIX_PATH to find loki via find_package."""
    return native_prefix()


def cmake_dir() -> Path:
    """Return the directory containing lokiConfig.cmake."""
    prefix = native_prefix()
    for lib_dir_name in ("lib", "lib64"):
        candidate = prefix / lib_dir_name / "cmake" / "loki"
        if (candidate / "lokiConfig.cmake").is_file():
            return candidate

    raise FileNotFoundError(
        f"lokiConfig.cmake not found under {prefix}; "
        "the installed pypddl is too old or incomplete."
    )
