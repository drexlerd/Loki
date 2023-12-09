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

pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNamesEquality& node, problem::Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.error_stream->str());
    }
    const auto& equal_predicate = context.domain_context->equal_predicate;
    const auto object_left = parse_object_reference(node.name_left, context);
    const auto object_right = parse_object_reference(node.name_right, context);
    return context.cache.get_or_create<pddl::GroundAtomImpl>(equal_predicate, pddl::ObjectList{object_left, object_right});
}

pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNamesPredicate& node, problem::Context& context) {
    const auto predicate = parse_predicate_reference(node.predicate.name, context);
    const auto object_list = parse_object_references(node.names, context);
    return context.cache.get_or_create<pddl::GroundAtomImpl>(predicate, object_list);
}


pddl::GroundAtom parse(const problem::ast::AtomicFormulaOfNames& node, problem::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfNamesVisitor(context), node);
}


GroundLiteralVisitor::GroundLiteralVisitor(problem::Context& context_) : context(context_) { }


pddl::GroundLiteral parse(const problem::ast::Atom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::GroundLiteralImpl>(false, parse(node.atomic_formula_of_names, context));
}

pddl::GroundLiteral parse(const problem::ast::NegatedAtom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::GroundLiteralImpl>(true, parse(node.atomic_formula_of_names, context));
}

pddl::GroundLiteral parse(const problem::ast::Literal& node, problem::Context& context) {
    return boost::apply_visitor(LiteralVisitor(context), node);
}


pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTermsPredicate& node, problem::Context& context) {
    auto predicate_name = parse(node.predicate.name);
    auto binding = context.get_current_scope().get<pddl::PredicateImpl>(predicate_name);
    if (!binding.has_value()) {
        context.error_handler(node.predicate, "");
        throw UndefinedPredicateError(predicate_name, context.error_stream->str());
    }
    pddl::ObjectList object_list;
    for (const auto& term_node : node.terms) {
        object_list.push_back(boost::apply_visitor(ObjectReferenceTermVisitor(context), term_node));
    }
    auto predicate = binding.value().object;
    if (predicate->get_parameters().size() != object_list.size()) {
        context.error_handler(node, "");
        throw MismatchedPredicateObjectListError(predicate, object_list, context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::GroundAtomImpl>(predicate, object_list);
}

pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTermsEquality& node, problem::Context& context) {
    // requires :equality
    if (!context.requirements->test(pddl::RequirementEnum::EQUALITY)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::EQUALITY, context.error_stream->str());
    }
    auto left_object = boost::apply_visitor(ObjectReferenceTermVisitor(context), node.term_left);
    auto right_object = boost::apply_visitor(ObjectReferenceTermVisitor(context), node.term_right);
    return context.cache.get_or_create<pddl::GroundAtomImpl>(context.domain_context->equal_predicate, pddl::ObjectList{left_object, right_object});
}

pddl::GroundAtom parse(const domain::ast::AtomicFormulaOfTerms& node, problem::Context& context) {
    return boost::apply_visitor(AtomicFormulaOfTermsVisitor(context), node);
}

AtomicFormulaOfTermsVisitor::AtomicFormulaOfTermsVisitor(problem::Context& context_) : context(context_) { }

pddl::GroundLiteral parse(const domain::ast::Atom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::GroundLiteralImpl>(false, parse(node.atomic_formula_of_terms, context));
}

pddl::GroundLiteral parse(const domain::ast::NegatedAtom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::GroundLiteralImpl>(true, parse(node.atomic_formula_of_terms, context));
}

pddl::GroundLiteral parse(const domain::ast::Literal& node, problem::Context& context) {
    return boost::apply_visitor(LiteralVisitor(context), node);
}

LiteralVisitor::LiteralVisitor(problem::Context& context_) : context(context_) { }

}