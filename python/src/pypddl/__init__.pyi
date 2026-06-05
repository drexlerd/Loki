from pathlib import Path

# Load public native dependency packages before this package loads native extensions.
import pyyggdrasil as pyyggdrasil

from . import formalism as formalism

__version__: str

def native_prefix() -> Path: ...
