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

#include "literal.hpp"
#include "loki/pddl/exceptions.hpp"
#include "parameters.hpp"
#include "reference_utils.hpp"

namespace loki
{
// parse a vector of goal descriptors
template<typename T>
static pddl::ConditionList parse(const std::vector<T>& nodes, Context& context)
{
    auto condition_list = pddl::ConditionList();
    for (const auto& node : nodes)
    {
        condition_list.push_back(parse(node, context));
    }
    return condition_list;
}

pddl::Condition parse(const ast::GoalDescriptor& node, Context& context) { return boost::apply_visitor(ConditionVisitor(context), node); }

pddl::Condition parse(const ast::GoalDescriptorAtom& node, Context& context)
{
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionLiteralImpl>(parse(node.atom, context));
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorLiteral& node, Context& context)
{
    // requires :negative-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionLiteralImpl>(parse(node.literal, context));
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorAnd& node, Context& context)
{
    auto condition_list = parse(node.goal_descriptors, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorOr& node, Context& context)
{
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    auto condition_list = parse(node.goal_descriptors, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionOrImpl>(condition_list);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorNot& node, Context& context)
{
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS);
    auto child_condition = parse(node.goal_descriptor, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionNotImpl>(child_condition);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorImply& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    auto condition_left = parse(node.goal_descriptor_left, context);
    auto condition_right = parse(node.goal_descriptor_right, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionImplyImpl>(condition_left, condition_right);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorExists& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.scopes.open_scope();
    auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    auto child_condition = parse(node.goal_descriptor, context);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    auto condition = context.factories.conditions.get_or_create<pddl::ConditionExistsImpl>(parameter_list, child_condition);
    context.positions.push_back(condition, node);
    return condition;
}

template<typename ConditionNode>
pddl::Condition parse_condition_forall(const ast::TypedListOfVariables& parameters_node, const ConditionNode& condition_node, Context& context)
{
    context.scopes.open_scope();
    auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), parameters_node);
    track_variable_references(parameter_list, context);
    auto child_condition = parse(condition_node, context);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    auto condition = context.factories.conditions.get_or_create<pddl::ConditionForallImpl>(parameter_list, child_condition);
    context.positions.push_back(condition, condition_node);
    return condition;
}

pddl::Condition parse(const ast::GoalDescriptorForall& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.goal_descriptor, context);
}

pddl::Condition parse(const ast::GoalDescriptorFunctionComparison& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    throw NotImplementedError("parse(const ast::GoalDescriptorFunctionComparison& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptor& node, Context& context) { return boost::apply_visitor(ConditionVisitor(context), node); }

pddl::Condition parse(const ast::ConstraintGoalDescriptorAnd& node, Context& context)
{
    auto condition_list = pddl::ConditionList();
    for (const auto& child_node : node.constraint_goal_descriptors)
    {
        condition_list.push_back(parse(child_node, context));
    }
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorForall& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.constraint_goal_descriptor, context);
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorAtEnd& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAtEnd& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorAlways& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAlways& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorSometime& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometime& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorWithin& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorWithin& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorAtMostOnce& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorSometimeAfter& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorSometimeBefore& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorAlwaysWithin& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorHoldDuring& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorHoldDuring& node, Context& context)");
}

pddl::Condition parse(const ast::ConstraintGoalDescriptorHoldAfter& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("parse(const ast::ConstraintGoalDescriptorHoldAfter& node, Context& context)");
}

pddl::Condition parse(const ast::PreconditionGoalDescriptor& node, Context& context) { return boost::apply_visitor(ConditionVisitor(context), node); }

pddl::Condition parse(const ast::PreconditionGoalDescriptorSimple& node, Context& context) { return parse(node.goal_descriptor, context); }

pddl::Condition parse(const ast::PreconditionGoalDescriptorAnd& node, Context& context)
{
    auto condition_list = pddl::ConditionList();
    for (const auto& child_node : node.precondition_goal_descriptors)
    {
        condition_list.push_back(parse(child_node, context));
    }
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const ast::PreconditionGoalDescriptorPreference& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::PREFERENCES))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::PREFERENCES, context.scopes.get_error_handler()(node, ""));
    }
    throw NotImplementedError("parse(const ast::PreconditionGoalDescriptorPreference& node, Context& context)");
}

pddl::Condition parse(const ast::PreconditionGoalDescriptorForall& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.precondition_goal_descriptor, context);
}

ConditionVisitor::ConditionVisitor(Context& context_) : context(context_) {}

}
