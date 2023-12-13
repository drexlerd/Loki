/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "literal.hpp"

#include "common.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& atomic_formula_of_terms_node, Context& context) {
    auto predicate_name = parse(atomic_formula_of_terms_node.predicate.name);
    auto binding = context.scopes.get<pddl::PredicateImpl>(predicate_name);
    if (!binding.has_value()) {
        throw UndefinedPredicateError(predicate_name, context.scopes.get_error_handler()(atomic_formula_of_terms_node.predicate, ""));
    }
    pddl::TermList term_list;
    for (const auto& term_node : atomic_formula_of_terms_node.terms) {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
    }
    const auto& [predicate, _position, _error_handler] = binding.value();
    if (predicate->get_parameters().size() != term_list.size()) {
        throw MismatchedPredicateTermListError(predicate, term_list, context.scopes.get_error_handler()(atomic_formula_of_terms_node, ""));
    }
    return context.cache.get_or_create<pddl::AtomImpl>(predicate, term_list);
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& atomic_formula_of_terms_node, Context& context) {
    // requires :equality
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.scopes.get_error_handler()(atomic_formula_of_terms_node, ""));
    }
    assert(context.scopes.get<pddl::PredicateImpl>("=").has_value());
    const auto& [equal_predicate, _position, _error_handler] = context.scopes.get<pddl::PredicateImpl>("=").value();
    auto left_term = boost::apply_visitor(TermReferenceTermVisitor(context), atomic_formula_of_terms_node.term_left);
    auto right_term = boost::apply_visitor(TermReferenceTermVisitor(context), atomic_formula_of_terms_node.term_right);
    assert(context.scopes.get<pddl::PredicateImpl>("=").has_value());
    return context.cache.get_or_create<pddl::AtomImpl>(
        equal_predicate,
        pddl::TermList{left_term, right_term});
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& atomic_formula_of_terms_node, Context& context) {
    return boost::apply_visitor(AtomicFormulaOfTermsVisitor(context), atomic_formula_of_terms_node);
}


AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(Context& context_)
    : context(context_) { }


pddl::Literal parse(const domain::ast::Atom& atom_node, Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(false, parse(atom_node.atomic_formula_of_terms, context));
}

pddl::Literal parse(const domain::ast::NegatedAtom& negated_atom_node, Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(true, parse(negated_atom_node.atomic_formula_of_terms, context));
}

pddl::Literal parse(const domain::ast::Literal& literal_node, Context& context) {
    return boost::apply_visitor(LiteralVisitor(context), literal_node);
}


LiteralVisitor::LiteralVisitor(Context& context_)
    : context(context_) { }

}