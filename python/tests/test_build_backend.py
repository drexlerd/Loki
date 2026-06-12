import csv
import importlib.util
import io
import os
import shutil
import subprocess
import sys
import types
import zipfile
from contextlib import contextmanager
from pathlib import Path


def load_backend():
    sys.modules.setdefault("scikit_build_core", types.SimpleNamespace(build=types.SimpleNamespace()))
    backend_path = Path(__file__).resolve().parents[1] / "pypddl_build_backend.py"
    spec = importlib.util.spec_from_file_location("pypddl_build_backend", backend_path)
    assert spec is not None
    assert spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def write_wheel(path, files):
    with zipfile.ZipFile(path, "w", compression=zipfile.ZIP_DEFLATED) as wheel:
        for name, content in sorted(files.items()):
            wheel.writestr(name, content)


def read_wheel(path):
    with zipfile.ZipFile(path) as wheel:
        return {name: wheel.read(name).decode("utf-8") for name in wheel.namelist() if not name.endswith("/")}


def read_record(files):
    return {row[0]: row[1:] for row in csv.reader(io.StringIO(files["pypddl-1.0.0.dist-info/RECORD"]))}


@contextmanager
def temporary_environment(updates):
    sentinel = object()
    old_values = {name: os.environ.get(name, sentinel) for name in updates}
    try:
        for name, value in updates.items():
            if value is None:
                os.environ.pop(name, None)
            else:
                os.environ[name] = value
        yield
    finally:
        for name, old_value in old_values.items():
            if old_value is sentinel:
                os.environ.pop(name, None)
            else:
                os.environ[name] = old_value


@contextmanager
def temporary_env(name, value):
    with temporary_environment({name: value}):
        yield


def test_num_jobs_uses_default_and_env_override():
    backend = load_backend()

    with temporary_env("PYPDDL_JOBS", None):
        assert backend._num_jobs() == 8
    with temporary_env("PYPDDL_JOBS", "6"):
        assert backend._num_jobs() == 6


def test_num_jobs_rejects_invalid_values():
    backend = load_backend()

    for value in ("0", "-1", "many"):
        with temporary_env("PYPDDL_JOBS", value):
            try:
                backend._num_jobs()
            except ValueError as error:
                assert "positive integer" in str(error)
            else:
                raise AssertionError(f"PYPDDL_JOBS={value!r} should be rejected")


def check_prepare_native_build_sets_expected_environment(tmp_path):
    backend = load_backend()
    native_prefix = tmp_path / "native"
    library_dir = native_prefix / "lib"
    library_dir.mkdir(parents=True)

    sentinel = object()
    old_pyyggdrasil = sys.modules.get("pyyggdrasil", sentinel)
    sys.modules["pyyggdrasil"] = types.SimpleNamespace(native_prefix=lambda: native_prefix)
    try:
        with temporary_environment(
            {
                "CMAKE_ARGS": "-DUSER_OPTION=ON",
                "CMAKE_BUILD_PARALLEL_LEVEL": None,
                "DYLD_LIBRARY_PATH": "/existing/dyld",
                "LD_LIBRARY_PATH": "/existing/ld",
                "PYPDDL_JOBS": "6",
            }
        ):
            backend._prepare_native_build()

            assert os.environ["CMAKE_BUILD_PARALLEL_LEVEL"] == "6"
            assert os.environ["LD_LIBRARY_PATH"] == f"{library_dir.resolve()}{os.pathsep}/existing/ld"
            assert os.environ["DYLD_LIBRARY_PATH"] == f"{library_dir.resolve()}{os.pathsep}/existing/dyld"

            cmake_args = os.environ["CMAKE_ARGS"]
            assert f"-DCMAKE_PREFIX_PATH={native_prefix.resolve()}" in cmake_args
            assert f"-DPython_EXECUTABLE={sys.executable}" in cmake_args
            assert f"-DPython3_EXECUTABLE={sys.executable}" in cmake_args
            assert "-DLOKI_BUILD_PYPDDL=ON" in cmake_args
            assert "-DLOKI_BUILD_TESTS=OFF" in cmake_args
            assert "-DCMAKE_INSTALL_LIBDIR=lib" in cmake_args
            assert cmake_args.endswith("-DUSER_OPTION=ON")
    finally:
        if old_pyyggdrasil is sentinel:
            sys.modules.pop("pyyggdrasil", None)
        else:
            sys.modules["pyyggdrasil"] = old_pyyggdrasil


def test_prepare_native_build_sets_expected_environment(tmp_path):
    check_prepare_native_build_sets_expected_environment(tmp_path)


def test_python_native_runtime_rpaths_use_dependency_libdir(tmp_path):
    cmake = shutil.which("cmake")
    if cmake is None:
        raise AssertionError("cmake executable is required for runtime rpath helper test")

    dependency_prefix = tmp_path / "dependency"
    (dependency_prefix / "lib").mkdir(parents=True)
    import pyyggdrasil

    helper = Path(pyyggdrasil.cmake_dir()) / "yggdrasilPythonHelpers.cmake"
    script = tmp_path / "check_runtime_rpaths.cmake"
    script.write_text(
        "\n".join(
            [
                "set(CMAKE_INSTALL_LIBDIR lib64)",
                f"include(\"{helper.as_posix()}\")",
                f"yggdrasil_register_python_native_runtime_prefix(\"pyyggdrasil\" \"{dependency_prefix.as_posix()}\")",
                "yggdrasil_make_python_native_runtime_rpath_string(runtime_rpath \"$ORIGIN\" \"../\")",
                "if(NOT runtime_rpath STREQUAL \"$ORIGIN:$ORIGIN/../pyyggdrasil/lib\")",
                "    message(FATAL_ERROR \"unexpected runtime rpath: ${runtime_rpath}\")",
                "endif()",
                "",
            ]
        ),
        encoding="utf-8",
    )

    subprocess.run([cmake, "-P", str(script)], check=True)


def check_fix_wheel_stubs_publishes_generated_stubs_and_preserves_top_level(tmp_path):
    backend = load_backend()
    wheel_path = tmp_path / "pypddl-1.0.0-py3-none-any.whl"
    write_wheel(
        wheel_path,
        {
            "pypddl/__init__.pyi": "from . import formalism as formalism\n",
            "pypddl/py.typed": "",
            "pypddl/formalism/__init__.py": "",
            "pypddl/formalism/__init__.pyi": "stale private reference: pypddl._pypddl.formalism\n",
            "pypddl/_pypddl/__init__.pyi": "private top-level should not replace public\n",
            "pypddl/_pypddl/formalism.pyi": "class Parser: ... # pypddl._pypddl.formalism pyyggdrasil._pyyggdrasil\n",
            "pypddl-1.0.0.dist-info/RECORD": "",
        },
    )

    backend._fix_wheel_stubs(wheel_path)

    files = read_wheel(wheel_path)
    assert files["pypddl/__init__.pyi"] == "from . import formalism as formalism\n"
    assert files["pypddl/py.typed"] == ""
    assert "pypddl/_pypddl/__init__.pyi" not in files
    assert "pypddl/_pypddl/formalism.pyi" not in files
    assert "pypddl._pypddl" not in files["pypddl/formalism/__init__.pyi"]
    assert "pyyggdrasil._pyyggdrasil" not in files["pypddl/formalism/__init__.pyi"]
    assert "class Parser" in files["pypddl/formalism/__init__.pyi"]

    record = read_record(files)
    assert "pypddl/_pypddl/__init__.pyi" not in record
    assert "pypddl/_pypddl/formalism.pyi" not in record
    assert record["pypddl/py.typed"][0].startswith("sha256=")
    assert record["pypddl/py.typed"][1] == "0"
    assert record["pypddl/formalism/__init__.pyi"][0].startswith("sha256=")
    assert record["pypddl/formalism/__init__.pyi"][1] == str(len(files["pypddl/formalism/__init__.pyi"].encode("utf-8")))
    assert record["pypddl-1.0.0.dist-info/RECORD"] == ["", ""]


def test_install_time_stub_patch_script_publishes_generated_stubs(tmp_path):
    cmake = shutil.which("cmake")
    if cmake is None:
        raise AssertionError("cmake executable is required for install-time stub patch test")

    install_prefix = tmp_path / "install"
    package_dir = install_prefix / "pypddl"
    private_stub_dir = package_dir / "_pypddl"
    formalism_dir = package_dir / "formalism"
    private_stub_dir.mkdir(parents=True)
    formalism_dir.mkdir()
    (package_dir / "__init__.pyi").write_text("from . import formalism as formalism\n", encoding="utf-8")
    (private_stub_dir / "__init__.pyi").write_text("private top-level should not replace public\n", encoding="utf-8")
    (private_stub_dir / "formalism.pyi").write_text(
        "class Parser: ... # pypddl._pypddl.formalism pyyggdrasil._pyyggdrasil\n",
        encoding="utf-8",
    )

    script = Path(__file__).resolve().parents[2] / "cmake" / "patch_python_stubs.cmake"
    subprocess.run([cmake, f"-DCMAKE_INSTALL_PREFIX={install_prefix}", "-P", str(script)], check=True)

    public_stub = formalism_dir / "__init__.pyi"
    assert (package_dir / "__init__.pyi").read_text(encoding="utf-8") == "from . import formalism as formalism\n"
    assert not private_stub_dir.exists()
    assert public_stub.is_file()
    public_stub_text = public_stub.read_text(encoding="utf-8")
    assert "class Parser" in public_stub_text
    assert "pypddl._pypddl" not in public_stub_text
    assert "pyyggdrasil._pyyggdrasil" not in public_stub_text


if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
