#include "literal.hpp"

#include "common.hpp"
#include "../../../../include/loki/common/exceptions.hpp"


namespace loki {

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    auto predicate_name = parse(atomic_formula_of_terms_node.predicate.name, error_handler, context);
    auto it = context.predicates_by_name.find(predicate_name);
    if (it == context.predicates_by_name.end()) {
        error_handler(atomic_formula_of_terms_node.predicate, "Undefined predicate.");
        throw UndefinedError(predicate_name, "predicate");
    }
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {

}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {

}


AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Atom AtomicFormulaOfTermsVisitor::operator()(const domain::ast::AtomicFormulaOfTermsPredicate& predicate_node) const {

}
pddl::Atom AtomicFormulaOfTermsVisitor::operator()(const domain::ast::AtomicFormulaOfTermsEquality& equality_node) const {

}


pddl::Literal parse(const domain::ast::Atom& atom_node, const error_handler_type& error_handler, domain::Context& context) {
    // auto predicate = context.cache.get_or_create<pddl::AtomImpl>(atom_node.atomic_formula_of_terms)
}

pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, const error_handler_type& error_handler, domain::Context& context) {
    // pddl::Predicate predicate = parse(literal_node.atomic_formula_of_terms)
}

pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context) {

}

}