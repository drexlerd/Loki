"""PEP 517 backend for the pypddl wheel; machinery lives in pyyggdrasil.build_support."""

from pyyggdrasil.build_support import ProviderBackend

ProviderBackend(package="pypddl", providers=("pyyggdrasil",)).install_hooks(globals())
