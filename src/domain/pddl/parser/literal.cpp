#include "literal.hpp"


namespace loki {


pddl::Literal parse(const domain::ast::Atom& literal_node, const error_handler_type& error_handler, domain::Context& context) {

}

pddl::Literal parse(const domain::ast::NegatedAtom& literal_node, const error_handler_type& error_handler, domain::Context& context) {
    // pddl::Predicate predicate = parse(literal_node.atomic_formula_of_terms)
}

pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context) {

}

}