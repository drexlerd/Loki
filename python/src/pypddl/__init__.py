from importlib.metadata import PackageNotFoundError, version
from pathlib import Path
from typing import Optional

try:
    import tomllib
except ModuleNotFoundError:  # pragma: no cover - Python < 3.11
    tomllib = None

# Load public native dependency packages before this package loads native extensions.
import pyyggdrasil as pyyggdrasil

from . import formalism as formalism


def _generated_version():
    try:
        from ._version import __version__ as generated_version
    except ImportError:
        return None
    return generated_version


def _version_from_pyproject(pyproject: Path) -> Optional[str]:
    if tomllib is not None:
        data = tomllib.loads(pyproject.read_text(encoding="utf-8"))
        project = data.get("project", {})
        parsed_version = project.get("version")
        return parsed_version if isinstance(parsed_version, str) else None

    current_section = None
    for raw_line in pyproject.read_text(encoding="utf-8").splitlines():
        line = raw_line.split("#", maxsplit=1)[0].strip()
        if not line:
            continue
        if line.startswith("[") and line.endswith("]"):
            current_section = line.strip("[]")
            continue
        if current_section == "project":
            key, separator, value = line.partition("=")
            if separator and key.strip() == "version":
                return value.strip().strip("\"").strip(chr(39))
    return None


def _source_version() -> str:
    for parent in Path(__file__).resolve().parents:
        pyproject = parent / "pyproject.toml"
        if not pyproject.exists():
            continue

        parsed_version = _version_from_pyproject(pyproject)
        if parsed_version is not None:
            return parsed_version

    return "0.0.0"


__version__ = _generated_version() or _source_version()
if __version__ == "0.0.0":
    try:
        __version__ = version("pypddl")
    except PackageNotFoundError:
        pass


__all__ = (
    "__version__",
    "cmake_dir",
    "cmake_prefix",
    "formalism",
    "native_include_dir",
    "native_lib_dir",
    "native_prefix",
    "pyyggdrasil",
)


def native_prefix() -> Path:
    package_dir = Path(__file__).resolve().parent
    native_dir = package_dir / "native"
    if (native_dir / "include" / "loki").is_dir():
        return native_dir
    for parent in package_dir.parents:
        if (parent / "include" / "loki").is_dir():
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
