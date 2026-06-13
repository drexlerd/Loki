"""PEP 517 backend for the pypddl wheel; machinery lives in pyyggdrasil.build_support."""

from pyyggdrasil.build_support import ProviderBackend

_BACKEND = ProviderBackend(
    package="pypddl",
    providers=("pyyggdrasil",),
    cmake_defines=(
        "-DLOKI_BUILD_PYPDDL=ON",
        "-DLOKI_BUILD_TESTS=OFF",
        "-DLOKI_BUILD_PROFILING=OFF",
        "-DLOKI_BUILD_EXECUTABLES=OFF",
        "-DCMAKE_INSTALL_LIBDIR=lib",
    ),
    rename_packages=("pypddl", "pyyggdrasil"),
    jobs_env="PYPDDL_JOBS",
    strip_env="PYPDDL_STRIP_WHEEL",
)

_num_jobs = _BACKEND._num_jobs
_prepare_native_build = _BACKEND._prepare_native_build
_fix_wheel_stubs = _BACKEND._fix_wheel_stubs
_strip_wheel_native_libraries = _BACKEND._strip_wheel_native_libraries

_BACKEND.install_hooks(globals())
