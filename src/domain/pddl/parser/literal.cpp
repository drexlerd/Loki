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
    auto predicate = it->second;
    auto term_list = parse(atomic_formula_of_terms_node.terms, error_handler, context);
    if (predicate->get_parameters().size() != term_list.size()) {
        throw MismatchedArgumentError("parameter", "term", predicate->get_parameters().size(), term_list.size());
    }
    return context.cache.get_or_create<pddl::AtomImpl>(predicate, term_list).object;
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        // TODO: improve error
        error_handler(atomic_formula_of_terms_node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY);
    }
    assert(context.predicates_by_name.count("="));
    auto equal_predicate = context.predicates_by_name.at("=");
    auto left_term = parse(atomic_formula_of_terms_node.term_left, error_handler, context);
    auto right_term = parse(atomic_formula_of_terms_node.term_right, error_handler, context);
    return context.cache.get_or_create<pddl::AtomImpl>(equal_predicate, pddl::TermList{left_term, right_term}).object;
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfTermsVisitor(error_handler, context), atomic_formula_of_terms_node);
}


AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }


pddl::Literal parse(const domain::ast::Atom& atom_node, const error_handler_type& error_handler, domain::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(false, parse(atom_node.atomic_formula_of_terms, error_handler, context)).object;
}

pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, const error_handler_type& error_handler, domain::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(true, parse(negated_atom_node.atomic_formula_of_terms, error_handler, context)).object;
}

pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(LiteralVisitor(error_handler, context), literal_node);
}


LiteralVisitor::LiteralVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

}