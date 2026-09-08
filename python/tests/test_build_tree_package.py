from pathlib import Path
import tomllib

import pypddl
import pypddl._pypddl


def project_version():
    pyproject = Path(__file__).resolve().parents[2] / "pyproject.toml"
    return tomllib.loads(pyproject.read_text(encoding="utf-8"))["project"]["version"]


def test_build_tree_package_exposes_current_extension_and_version():
    package_dir = Path(pypddl.__file__).parent

    assert package_dir.name == "pypddl"
    assert (package_dir / "_pypddl").is_file() or any(package_dir.glob("_pypddl*.so"))
    assert not (package_dir / "_pypddl").is_dir()
    assert Path(pypddl._pypddl.__file__).parent == package_dir
    assert pypddl.__version__ == project_version()



if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
