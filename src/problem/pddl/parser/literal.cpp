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

#include "literal.hpp"

#include "objects.hpp"
#include "common.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../../include/loki/problem/pddl/exceptions.hpp"
#include "../../../domain/pddl/parser/common.hpp"


namespace loki {

pddl::Predicate parse_predicate_reference(const domain::ast::Name& node, problem::Context& context) {
    const auto name = parse(node);
    const auto binding = context.domain_context->get_current_scope().get<pddl::PredicateImpl>(name);
    if (!binding.has_value()) {
        context.error_handler(node, "");
        throw UndefinedPredicateError(name, context.error_stream->str());
    }
    return binding.value().object;
}

AtomicFormulaOfNamesVisitor::AtomicFormulaOfNamesVisitor(problem::Context& context_) : context(context_) { }

pddl::Atom parse(const problem::ast::AtomicFormulaOfNamesEquality& node, problem::Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.error_stream->str());
    }
    const auto& equal_predicate = context.domain_context->equal_predicate;
    const auto term_left = context.cache.get_or_create<pddl::TermObjectImpl>(parse_object_reference(node.name_left, context));
    const auto term_right = context.cache.get_or_create<pddl::TermObjectImpl>(parse_object_reference(node.name_right, context));
    return context.cache.get_or_create<pddl::AtomImpl>(equal_predicate, pddl::TermList{term_left, term_right});
}

pddl::Atom parse(const problem::ast::AtomicFormulaOfNamesPredicate& node, problem::Context& context) {
    const auto predicate = parse_predicate_reference(node.predicate.name, context);
    pddl::TermList term_list;
    for (const auto& name_node : node.names) {
        term_list.push_back(context.cache.get_or_create<pddl::TermObjectImpl>(parse_object_reference(name_node, context)));
    }
    return context.cache.get_or_create<pddl::AtomImpl>(predicate, term_list);
}


pddl::Atom parse(const problem::ast::AtomicFormulaOfNames& node, problem::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfNamesVisitor(context), node);
}


LiteralVisitor::LiteralVisitor(problem::Context& context_) : context(context_) { }


pddl::Literal parse(const problem::ast::Atom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(false, parse(node.atomic_formula_of_names, context));
}

pddl::Literal parse(const problem::ast::NegatedAtom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::LiteralImpl>(true, parse(node.atomic_formula_of_names, context));
}

pddl::Literal parse(const problem::ast::Literal& node, problem::Context& context) {
    return boost::apply_visitor(LiteralVisitor(context), node);
}

}