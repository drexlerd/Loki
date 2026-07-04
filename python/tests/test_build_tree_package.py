from pathlib import Path

import pypddl


def project_version():
    pyproject = Path(__file__).resolve().parents[2] / "pyproject.toml"
    for line in pyproject.read_text(encoding="utf-8").splitlines():
        if line.startswith("version"):
            return line.split("=", maxsplit=1)[1].strip().strip("\"")
    raise AssertionError("project version not found")


def test_build_tree_package_exposes_current_extension_and_public_stub():
    package_dir = Path(pypddl.__file__).parent

    assert package_dir.name == "pypddl"
    assert (package_dir / "_pypddl").is_file() or any(package_dir.glob("_pypddl*.so"))
    assert (package_dir / "__init__.pyi").is_file()
    assert (package_dir / "py.typed").is_file()
    assert (package_dir / "_version.py").is_file()
    assert not (package_dir / "_pypddl").is_dir()
    assert pypddl.__version__ == project_version()
    assert pypddl.formalism.Parser is not None

    package_stub = (package_dir / "__init__.pyi").read_text(encoding="utf-8")
    assert "from . import formalism as formalism" in package_stub
    assert "__all__" not in package_stub



if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
