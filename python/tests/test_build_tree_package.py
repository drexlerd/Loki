from importlib.metadata import version
from pathlib import Path

import pypddl
import pypddl._pypddl


def test_build_tree_package_exposes_current_extension_and_version():
    package_dir = Path(pypddl.__file__).parent

    assert package_dir.name == "pypddl"
    assert (package_dir / "_pypddl").is_file() or any(package_dir.glob("_pypddl*.so"))
    assert not (package_dir / "_pypddl").is_dir()
    assert Path(pypddl._pypddl.__file__).parent == package_dir
    assert pypddl.__version__ == version("pypddl")



if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
