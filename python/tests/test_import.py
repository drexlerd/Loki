import importlib.util
from pathlib import Path
from tempfile import TemporaryDirectory

import pypddl


def test_top_level_package_exports_public_helpers_and_formalism_module():
    assert isinstance(pypddl.__version__, str)
    assert pypddl.__version__ != ""
    assert callable(pypddl.native_prefix)
    assert callable(pypddl.native_include_dir)
    assert callable(pypddl.native_lib_dir)
    assert pypddl.formalism.Parser is not None
    assert pypddl.formalism.RepositoryFactory is not None


def test_top_level_all_lists_public_package_symbols():
    assert pypddl.__all__ == (
        "__version__",
        "cmake_dir",
        "cmake_prefix",
        "formalism",
        "native_include_dir",
        "native_lib_dir",
        "native_prefix",
        "pyyggdrasil",
    )
    assert "_version_from_pyproject" not in pypddl.__all__


def test_source_public_stubs_describe_public_all():
    package_dir = Path(pypddl.__file__).parent

    assert "__all__: tuple[str, ...]" in (package_dir / "__init__.pyi").read_text(encoding="utf-8")
    formalism_stub = (package_dir / "formalism" / "__init__.pyi").read_text(encoding="utf-8")
    assert "__all__: tuple[str, ...]" in formalism_stub
    for name in (
        "DomainTranslationResult",
        "ProblemTranslationResult",
        "UndefinedTypeError",
        "DuplicateTypeError",
        "InvalidMetricError",
    ):
        assert name in formalism_stub


def test_native_prefix_layout():
    native_prefix = pypddl.native_prefix()
    native_include_dir = pypddl.native_include_dir()
    native_lib_dir = pypddl.native_lib_dir()
    package_dir = Path(pypddl.__file__).parent

    assert isinstance(native_prefix, Path)
    assert isinstance(native_include_dir, Path)
    assert isinstance(native_lib_dir, Path)
    assert native_include_dir == native_prefix / "include"
    assert native_lib_dir.name in {"lib", "lib64"}

    assert not (package_dir / "_pypddl").is_dir()

    if (native_prefix / "include" / "loki").is_dir():
        assert native_include_dir.is_dir()
        assert native_lib_dir.is_dir()
        assert (native_lib_dir / "cmake" / "loki").is_dir()
        assert (package_dir / "formalism" / "__init__.pyi").is_file()
        assert (package_dir / "py.typed").is_file()
    else:
        assert (package_dir / "__init__.pyi").is_file()
        assert (package_dir / "py.typed").is_file()
        assert (package_dir / "formalism" / "__init__.pyi").is_file()
        assert (package_dir / "_version.py").is_file()
        assert (package_dir / "_pypddl").is_file() or any(package_dir.glob("_pypddl*.so"))


def test_source_version_parser_reads_project_section_only():
    with TemporaryDirectory(prefix="pypddl-version-test-") as tmp:
        pyproject = Path(tmp) / "pyproject.toml"
        pyproject.write_text(
            """
[tool.other]
version = "9.9.9"

[project]
name = "pypddl"
version = "1.2.3"
""".strip(),
            encoding="utf-8",
        )

        assert pypddl._version_from_pyproject(pyproject) == "1.2.3"

        original_tomllib = pypddl.tomllib
        try:
            pypddl.tomllib = None
            assert pypddl._version_from_pyproject(pyproject) == "1.2.3"
        finally:
            pypddl.tomllib = original_tomllib

        pyproject.write_text(
            """
[tool.other]
version = "9.9.9"
""".strip(),
            encoding="utf-8",
        )
        assert pypddl._version_from_pyproject(pyproject) is None


def test_formalism_package_reexports_native_public_symbols():
    native_formalism = pypddl._pypddl.formalism
    missing = [name for name in dir(native_formalism) if not name.startswith("_") and not hasattr(pypddl.formalism, name)]

    assert missing == []


def test_formalism_all_matches_reexported_public_symbols():
    exported = set(pypddl.formalism.__all__)
    assert "Parser" in exported
    assert "RepositoryFactory" in exported
    assert "translate_domain" in exported
    assert not any(name.startswith("_") for name in exported)
    assert exported == {name for name in dir(pypddl.formalism) if not name.startswith("_")}


def test_programmatic_gripper_example_uses_public_api():
    example_path = Path(__file__).resolve().parents[1] / "example" / "programmatic_gripper.py"
    spec = importlib.util.spec_from_file_location("programmatic_gripper", example_path)
    assert spec is not None
    assert spec.loader is not None
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)

    _, domain, one_ball, two_ball = module.build_gripper()

    domain_text = pypddl.formalism.format_domain(domain)
    one_ball_text = pypddl.formalism.format_task(one_ball)
    reparsed = pypddl.formalism.Parser(domain_text)
    reparsed_one_ball = reparsed.parse_task(one_ball_text)

    assert reparsed.domain().get_name() == domain.get_name()
    assert reparsed_one_ball.get_name() == one_ball.get_name()

    assert domain.get_name() == "gripper"
    assert domain.get_num_actions() == 3
    assert domain.get_num_predicates() == 4
    assert one_ball.get_num_objects() == 5
    assert one_ball.get_num_initial_literals() == 4
    assert two_ball.get_num_objects() == 6
    assert two_ball.get_num_initial_literals() == 5


if __name__ == "__main__":
    from direct_runner import run_tests

    run_tests(globals())
