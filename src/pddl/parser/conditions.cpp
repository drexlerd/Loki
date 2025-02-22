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

#include "conditions.hpp"

#include "error_handling.hpp"
#include "functions.hpp"
#include "literal.hpp"
#include "loki/details/ast/printer.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "parameters.hpp"
#include "reference_utils.hpp"

namespace loki
{

template<ParsingContext C>
ConditionVisitor<C>::ConditionVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptor& node)
{
    return parse(node, context);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorAtom& node)
{
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_literal(parse(node.atom, context)));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorLiteral& node)
{
    // requires :negative-preconditions
    test_undefined_requirement(RequirementEnum::NEGATIVE_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::NEGATIVE_PRECONDITIONS);
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_literal(parse(node.literal, context)));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorAnd& node)
{
    auto condition_list = ConditionList {};
    for (const auto& child_node : node.goal_descriptors)
    {
        condition_list.push_back(this->operator()(child_node));
    }
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_and(condition_list));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorOr& node)
{
    // requires :disjunctive-preconditions
    test_undefined_requirement(RequirementEnum::DISJUNCTIVE_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    auto condition_list = ConditionList {};
    for (const auto& child_node : node.goal_descriptors)
    {
        condition_list.push_back(this->operator()(child_node));
    }
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_or(condition_list));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorNot& node)
{
    // requires :disjunctive-preconditions
    test_undefined_requirement(RequirementEnum::NEGATIVE_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::NEGATIVE_PRECONDITIONS);
    auto child_condition = this->operator()(node.goal_descriptor);
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_not(child_condition));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorImply& node)
{
    test_undefined_requirement(RequirementEnum::DISJUNCTIVE_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    auto condition_left = this->operator()(node.goal_descriptor_left);
    auto condition_right = this->operator()(node.goal_descriptor_right);
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_imply(condition_left, condition_right));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorExists& node)
{
    test_undefined_requirement(RequirementEnum::EXISTENTIAL_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.scopes.open_scope();
    auto parameter_list_visitor = ParameterListVisitor(context);
    auto parameter_list = boost::apply_visitor(parameter_list_visitor, node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto child_condition = this->operator()(node.goal_descriptor);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_exists(parameter_list, child_condition));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C, typename ConditionNode>
static Condition parse_condition_forall(const ast::TypedListOfVariables& parameters_node, const ConditionNode& condition_node, C& context)
{
    context.scopes.open_scope();
    auto parameter_list_visitor = ParameterListVisitor(context);
    auto parameter_list = boost::apply_visitor(parameter_list_visitor, parameters_node);
    track_variable_references(parameter_list, context);
    auto condition_visitor = ConditionVisitor(context);
    auto child_condition = boost::apply_visitor(condition_visitor, condition_node);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_forall(parameter_list, child_condition));
    context.positions.push_back(condition, condition_node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorForall& node)
{
    test_undefined_requirement(RequirementEnum::UNIVERSAL_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.goal_descriptor, context);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::GoalDescriptorFunctionComparison& node)
{
    if (!context.requirements->test(RequirementEnum::NUMERIC_FLUENTS))
    {
        throw UndefinedRequirementError(RequirementEnum::NUMERIC_FLUENTS, context.scopes.top().get_error_handler()(node, ""));
    }
    context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);

    auto binary_comparator = boost::apply_visitor(BinaryComparatorVisitor(), node.binary_comparator);
    auto fexpr_visitor = FunctionExpressionVisitor(context);
    auto function_expression_left = boost::apply_visitor(fexpr_visitor, node.function_expression_left);
    auto function_expression_right = boost::apply_visitor(fexpr_visitor, node.function_expression_right);
    auto condition = context.builder.get_or_create_condition(
        context.builder.get_or_create_condition_numeric_constraint(binary_comparator, function_expression_left, function_expression_right));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptor& node)
{
    return parse(node, context);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorAnd& node)
{
    auto condition_list = ConditionList();
    for (const auto& child_node : node.constraint_goal_descriptors)
    {
        condition_list.push_back(this->operator()(child_node));
    }
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_and(condition_list));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorForall& node)
{
    test_undefined_requirement(RequirementEnum::UNIVERSAL_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.constraint_goal_descriptor, context);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorAtEnd& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAtEnd& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorAlways& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAlways& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorSometime& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometime& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorWithin& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorWithin& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorAtMostOnce& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAtMostOnce& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorSometimeAfter& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometimeAfter& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorSometimeBefore& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometimeBefore& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorAlwaysWithin& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAlwaysWithin& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorHoldDuring& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorHoldDuring& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::ConstraintGoalDescriptorHoldAfter& /*node*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorHoldAfter& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::PreconditionGoalDescriptor& node)
{
    return parse(node, context);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::PreconditionGoalDescriptorSimple& node)
{
    return this->operator()(node.goal_descriptor);
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::PreconditionGoalDescriptorAnd& node)
{
    auto condition_list = ConditionList();
    for (const auto& child_node : node.precondition_goal_descriptors)
    {
        condition_list.push_back(this->operator()(child_node));
    }
    const auto condition = context.builder.get_or_create_condition(context.builder.get_or_create_condition_and(condition_list));
    context.positions.push_back(condition, node);
    return condition;
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::PreconditionGoalDescriptorPreference& node)
{
    test_undefined_requirement(RequirementEnum::PREFERENCES, node, context);
    throw NotImplementedError("parse(const ast::PreconditionGoalDescriptorPreference& node, C& context)");
}

template<ParsingContext C>
Condition ConditionVisitor<C>::operator()(const ast::PreconditionGoalDescriptorForall& node)
{
    test_undefined_requirement(RequirementEnum::UNIVERSAL_PRECONDITIONS, node, context);
    context.references.untrack(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.precondition_goal_descriptor, context);
}

template struct ConditionVisitor<DomainParsingContext>;
template struct ConditionVisitor<ProblemParsingContext>;

template<ParsingContext C>
Condition parse(const ast::GoalDescriptor& node, C& context)
{
    auto visitor = ConditionVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template Condition parse(const ast::GoalDescriptor& node, DomainParsingContext& context);
template Condition parse(const ast::GoalDescriptor& node, ProblemParsingContext& context);

template<ParsingContext C>
Condition parse(const ast::ConstraintGoalDescriptor& node, C& context)
{
    auto visitor = ConditionVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template Condition parse(const ast::ConstraintGoalDescriptor& node, DomainParsingContext& context);
template Condition parse(const ast::ConstraintGoalDescriptor& node, ProblemParsingContext& context);

template<ParsingContext C>
Condition parse(const ast::PreconditionGoalDescriptor& node, C& context)
{
    auto visitor = ConditionVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template Condition parse(const ast::PreconditionGoalDescriptor& node, DomainParsingContext& context);
template Condition parse(const ast::PreconditionGoalDescriptor& node, ProblemParsingContext& context);
}
