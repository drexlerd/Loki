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

#include "objects.hpp"
#include "predicate.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../domain/pddl/parser/common.hpp"


namespace loki {

AtomicFormulaOfNamesVisitor::AtomicFormulaOfNamesVisitor(problem::Context& context_) : context(context_) { }

pddl::GroundAtom AtomicFormulaOfNamesVisitor::operator()(const problem::ast::AtomicFormulaOfNamesEquality& equality_node) {
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        context.error_handler(equality_node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.error_stream->str());
    }
    const auto& equal_predicate = context.domain_context->equal_predicate;
    const auto object_left = parse_object_reference(equality_node.name_left, context);
    const auto object_right = parse_object_reference(equality_node.name_right, context);
    return context.cache.get_or_create<pddl::GroundAtomImpl>(equal_predicate, pddl::ObjectList{object_left, object_right});
}

pddl::GroundAtom AtomicFormulaOfNamesVisitor::operator()(const problem::ast::AtomicFormulaOfNamesPredicate& predicate_node) {
    const auto predicate = parse(predicate_node.predicate, context);

}

pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNames& atomic_formula_of_names_node, problem::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfNamesVisitor(context), atomic_formula_of_names_node);
}

pddl::GroundAtom parse(const problem::ast::Atom& atom_node, problem::Context& context) {
    return parse(atom_node.atomic_formula_of_names, context);
}

pddl::GroundLiteral parse(const problem::ast::NegatedAtom& negated_atom_node, problem::Context& context) {

}

pddl::GroundLiteral parse(const problem::ast::Literal& literal_node, problem::Context& context) {

}

}