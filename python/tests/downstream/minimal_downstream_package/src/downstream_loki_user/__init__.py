from ._downstream_loki import multiply, parses_domain
from pypddl import formalism as _formalism


def describe_pypddl_imports():
    parser = _formalism.Parser(
        """
(define (domain downstream-check)
  (:predicates (at ?x))
  (:action noop
    :parameters (?x)
    :precondition (at ?x)
    :effect (at ?x))
)
"""
    )
    return {
        "parser": type(parser).__name__,
        "domain_name": parser.domain().get_name(),
    }
