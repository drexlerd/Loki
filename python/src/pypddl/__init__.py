from importlib import metadata
from pathlib import Path


try:
    __version__ = metadata.version("pypddl")
except metadata.PackageNotFoundError:
    __version__ = "0.0.1"


def native_prefix() -> Path:
    return Path(__file__).resolve().parent / "native"
