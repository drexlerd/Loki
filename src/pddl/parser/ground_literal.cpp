/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "ground_literal.hpp"

#include "common.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "objects.hpp"

namespace loki
{

/* Atom */
Atom parse(const ast::AtomicFormulaOfNamesPredicate& node, Context& context)
{
    const auto name = parse(node.predicate.name);
    const auto binding = context.scopes.get<PredicateImpl>(name);
    if (!binding.has_value())
    {
        throw UndefinedPredicateError(name, context.scopes.get_error_handler()(node, ""));
    }
    auto term_list = TermList();
    for (const auto& name_node : node.names)
    {
        term_list.push_back(context.factories.get_or_create_term_object(parse_object_reference(name_node, context)));
    }
    const auto [predicate, _position, _error_handler] = binding.value();
    if (predicate->get_parameters().size() != term_list.size())
    {
        throw MismatchedPredicateTermListError(predicate, term_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto atom = context.factories.get_or_create_atom(predicate, term_list);
    context.positions.push_back(atom, node);
    return atom;
}

Atom parse(const ast::AtomicFormulaOfNamesEquality& node, Context& context)
{
    if (!context.requirements->test(RequirementEnum::EQUALITY))
    {
        throw UndefinedRequirementError(RequirementEnum::EQUALITY, context.scopes.get_error_handler()(node, ""));
    }
    assert(context.scopes.get<PredicateImpl>("=").has_value());
    const auto [equal_predicate, _position, _error_handler] = context.scopes.get<PredicateImpl>("=").value();
    const auto term_left = context.factories.get_or_create_term_object(parse_object_reference(node.name_left, context));
    const auto term_right = context.factories.get_or_create_term_object(parse_object_reference(node.name_right, context));
    const auto atom = context.factories.get_or_create_atom(equal_predicate, TermList { term_left, term_right });
    context.positions.push_back(atom, node);
    return atom;
}

Atom parse(const ast::AtomicFormulaOfNames& node, Context& context) { return boost::apply_visitor(GroundAtomicFormulaOfNamesVisitor(context), node); }

GroundAtomicFormulaOfNamesVisitor::GroundAtomicFormulaOfNamesVisitor(Context& context_) : context(context_) {}

/* Literal */
Literal parse(const ast::GroundAtom& node, Context& context)
{
    const auto literal = context.factories.get_or_create_literal(false, parse(node.atomic_formula_of_names, context));
    context.positions.push_back(literal, node);
    return literal;
}

Literal parse(const ast::NegatedGroundAtom& node, Context& context)
{
    const auto literal = context.factories.get_or_create_literal(true, parse(node.atomic_formula_of_names, context));
    context.positions.push_back(literal, node);
    return literal;
}

Literal parse(const ast::GroundLiteral& node, Context& context) { return boost::apply_visitor(GroundLiteralVisitor(context), node); }

GroundLiteralVisitor::GroundLiteralVisitor(Context& context_) : context(context_) {}

}