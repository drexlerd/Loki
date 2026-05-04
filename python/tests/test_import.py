import pypddl


def test_native_prefix_layout():
    native_prefix = pypddl.native_prefix()

    assert pypddl.__version__ != ""
    assert (native_prefix / "include" / "loki").is_dir()
    assert (native_prefix / "lib").is_dir()
    assert (native_prefix / "lib" / "cmake" / "loki").is_dir()
