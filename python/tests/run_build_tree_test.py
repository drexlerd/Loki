import importlib.machinery
import runpy
import sys


if __name__ == "__main__":
    # An editable install may register a finder ahead of the normal build-tree path.
    sys.meta_path.insert(0, importlib.machinery.PathFinder)
    runpy.run_path(sys.argv[1], run_name="__main__")
