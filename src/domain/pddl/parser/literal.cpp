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

#include <loki/domain/pddl/exceptions.hpp>


namespace loki {

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsPredicate& node, Context& context) {
    auto predicate_name = parse(node.predicate.name);
    auto binding = context.scopes.get<pddl::PredicateImpl>(predicate_name);
    if (!binding.has_value()) {
        throw UndefinedPredicateError(predicate_name, context.scopes.get_error_handler()(node.predicate, ""));
    }
    auto term_list = pddl::TermList();
    for (const auto& term_node : node.terms) {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
    }
    const auto& [predicate, _position, _error_handler] = binding.value();
    if (predicate->get_parameters().size() != term_list.size()) {
        throw MismatchedPredicateTermListError(predicate, term_list, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(predicate);
    const auto atom = context.factories.atoms.get_or_create<pddl::AtomImpl>(predicate, term_list);
    context.positions.push_back(atom, node);
    return atom;
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTermsEquality& node, Context& context) {
    // requires :equality
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::EQUALITY);
    assert(context.scopes.get<pddl::PredicateImpl>("=").has_value());
    const auto& [equal_predicate, _position, _error_handler] = context.scopes.get<pddl::PredicateImpl>("=").value();
    auto left_term = boost::apply_visitor(TermReferenceTermVisitor(context), node.term_left);
    auto right_term = boost::apply_visitor(TermReferenceTermVisitor(context), node.term_right);
    const auto atom = context.factories.atoms.get_or_create<pddl::AtomImpl>(
        equal_predicate,
        pddl::TermList{left_term, right_term});
    context.positions.push_back(atom, node);
    return atom;
}

pddl::Atom parse(const domain::ast::AtomicFormulaOfTerms& node, Context& context) {
    return boost::apply_visitor(AtomicFormulaOfTermsVisitor(context), node);
}


AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(Context& context_)
    : context(context_) { }


pddl::Literal parse(const domain::ast::Atom& node, Context& context) {
    const auto literal = context.factories.literals.get_or_create<pddl::LiteralImpl>(false, parse(node.atomic_formula_of_terms, context));
    context.positions.push_back(literal, node);
    return literal;
}

pddl::Literal parse(const domain::ast::NegatedAtom& node, Context& context) {
    const auto literal = context.factories.literals.get_or_create<pddl::LiteralImpl>(true, parse(node.atomic_formula_of_terms, context));
    context.positions.push_back(literal, node);
    return literal;
}

pddl::Literal parse(const domain::ast::Literal& node, Context& context) {
    return boost::apply_visitor(LiteralVisitor(context), node);
}


LiteralVisitor::LiteralVisitor(Context& context_)
    : context(context_) { }

}