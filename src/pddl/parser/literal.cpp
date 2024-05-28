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
#include "error_handling.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "reference_utils.hpp"

namespace loki
{

Atom parse(const ast::AtomicFormulaOfTermsPredicate& node, Context& context)
{
    auto predicate_name = parse(node.predicate.name);
    test_undefined_predicate(predicate_name, node.predicate, context);
    auto term_list = TermList();
    auto positions = PositionList();
    for (const auto& term_node : node.terms)
    {
        term_list.push_back(boost::apply_visitor(TermReferenceTermVisitor(context), term_node));
        positions.push_back(term_node);
    }
    auto binding = context.scopes.top().get_predicate(predicate_name);
    const auto [predicate, _position, _error_handler] = binding.value();
    test_arity_compatibility(predicate->get_parameters().size(), term_list.size(), node, context);
    test_incompatible_grounding(predicate->get_parameters(), term_list, positions, context);
    context.references.untrack(predicate);
    const auto atom = context.factories.get_or_create_atom(predicate, term_list);
    context.positions.push_back(atom, node);
    return atom;
}

Atom parse(const ast::AtomicFormulaOfTermsEquality& node, Context& context)
{
    test_undefined_requirement(RequirementEnum::EQUALITY, node, context);
    context.references.untrack(RequirementEnum::EQUALITY);
    assert(context.scopes.top().get_predicate("=").has_value());
    const auto [equal_predicate, _position, _error_handler] = context.scopes.top().get_predicate("=").value();
    auto left_term = boost::apply_visitor(TermReferenceTermVisitor(context), node.term_left);
    auto right_term = boost::apply_visitor(TermReferenceTermVisitor(context), node.term_right);
    const auto atom = context.factories.get_or_create_atom(equal_predicate, TermList { left_term, right_term });
    context.positions.push_back(atom, node);
    return atom;
}

Atom parse(const ast::AtomicFormulaOfTerms& node, Context& context) { return boost::apply_visitor(AtomicFormulaOfTermsVisitor(context), node); }

AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(Context& context_) : context(context_) {}

Literal parse(const ast::Atom& node, Context& context)
{
    const auto literal = context.factories.get_or_create_literal(false, parse(node.atomic_formula_of_terms, context));
    context.positions.push_back(literal, node);
    return literal;
}

Literal parse(const ast::NegatedAtom& node, Context& context)
{
    const auto literal = context.factories.get_or_create_literal(true, parse(node.atomic_formula_of_terms, context));
    context.positions.push_back(literal, node);
    return literal;
}

Literal parse(const ast::Literal& node, Context& context) { return boost::apply_visitor(LiteralVisitor(context), node); }

LiteralVisitor::LiteralVisitor(Context& context_) : context(context_) {}

}