from ._downstream_loki import multiply, parses_domain
from pypddl import formalism as _formalism


def describe_pypddl_imports(domain_source):
    parser = _formalism.Parser(domain_source)
    return {
        "parser": type(parser).__name__,
        "domain_name": parser.domain().get_name(),
    }
