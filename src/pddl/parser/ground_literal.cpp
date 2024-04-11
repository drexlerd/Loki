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
#include "loki/pddl/exceptions.hpp"
#include "objects.hpp"

namespace loki
{

/* Atom */
pddl::GroundAtom parse(const ast::AtomicFormulaOfNamesPredicate& node, Context& context)
{
    const auto name = parse(node.predicate.name);
    const auto binding = context.scopes.get_predicate(name);
    if (!binding.has_value())
    {
        throw UndefinedPredicateError(name, context.scopes.get_error_handler()(node, ""));
    }
    auto object_list = pddl::ObjectList();
    for (const auto& name_node : node.names)
    {
        object_list.push_back(parse_object_reference(name_node, context));
    }
    const auto [predicate, _position, _error_handler] = binding.value();
    if (predicate->get_parameters().size() != object_list.size())
    {
        throw MismatchedPredicateObjectListError(predicate, object_list, context.scopes.get_error_handler()(node, ""));
    }
    const auto atom = context.factories.ground_atoms.get_or_create<pddl::GroundAtomImpl>(predicate, object_list);
    context.positions.push_back(atom, node);
    return atom;
}

pddl::GroundAtom parse(const ast::AtomicFormulaOfNamesEquality& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.scopes.get_error_handler()(node, ""));
    }
    assert(context.scopes.get_predicate("=").has_value());
    const auto [equal_predicate, _position, _error_handler] = context.scopes.get_predicate("=").value();
    const auto object_left = parse_object_reference(node.name_left, context);
    const auto object_right = parse_object_reference(node.name_right, context);
    const auto atom = context.factories.ground_atoms.get_or_create<pddl::GroundAtomImpl>(equal_predicate, pddl::ObjectList { object_left, object_right });
    context.positions.push_back(atom, node);
    return atom;
}

pddl::GroundAtom parse(const ast::AtomicFormulaOfNames& node, Context& context)
{
    return boost::apply_visitor(GroundAtomicFormulaOfNamesVisitor(context), node);
}

GroundAtomicFormulaOfNamesVisitor::GroundAtomicFormulaOfNamesVisitor(Context& context_) : context(context_) {}

/* Literal */
pddl::GroundLiteral parse(const ast::GroundAtom& node, Context& context)
{
    const auto literal = context.factories.ground_literals.get_or_create<pddl::GroundLiteralImpl>(false, parse(node.atomic_formula_of_names, context));
    context.positions.push_back(literal, node);
    return literal;
}

pddl::GroundLiteral parse(const ast::NegatedGroundAtom& node, Context& context)
{
    const auto literal = context.factories.ground_literals.get_or_create<pddl::GroundLiteralImpl>(true, parse(node.atomic_formula_of_names, context));
    context.positions.push_back(literal, node);
    return literal;
}

pddl::GroundLiteral parse(const ast::GroundLiteral& node, Context& context) { return boost::apply_visitor(GroundLiteralVisitor(context), node); }

GroundLiteralVisitor::GroundLiteralVisitor(Context& context_) : context(context_) {}

}