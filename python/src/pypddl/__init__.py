from importlib.metadata import PackageNotFoundError, version
from pathlib import Path

from . import formalism as formalism


def _source_version() -> str:
    for parent in Path(__file__).resolve().parents:
        pyproject = parent / "pyproject.toml"
        if not pyproject.exists():
            continue

        for line in pyproject.read_text(encoding="utf-8").splitlines():
            if line.startswith("version"):
                return line.split("=", maxsplit=1)[1].strip().strip('"')

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
        if (parent / "include" / "loki").is_dir():
            return parent
    return native_dir
