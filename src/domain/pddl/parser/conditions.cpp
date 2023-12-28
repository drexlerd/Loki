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
#include "parameters.hpp"

#include "../../../../include/domain/pddl/exceptions.hpp"


namespace loki {
// parse a vector of goal descriptors
template<typename T>
static pddl::ConditionList parse(const std::vector<T>& nodes, Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& node : nodes) {
        condition_list.push_back(parse(node, context));
    }
    return condition_list;
}

pddl::Condition parse(const domain::ast::GoalDescriptor& node, Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, Context& context) {
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionLiteralImpl>(parse(node.atom, context));
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, Context& context) {
    // requires :negative-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionLiteralImpl>(parse(node.literal, context));
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, Context& context) {
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionOrImpl>(condition_list);
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    pddl::Condition child_condition = parse(node.goal_descriptor, context);
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionNotImpl>(child_condition);
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    auto condition_left = parse(node.goal_descriptor_left, context); 
    auto condition_right = parse(node.goal_descriptor_right, context); 
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionImplyImpl>(condition_left, condition_right);
    context.positions.push_back(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.scopes.open_scope();
    auto parameters = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    for (const auto& parameter : parameters) {
        context.referenced_pointers.track(parameter->get_variable());
    }
    auto child_condition = parse(node.goal_descriptor, context);
    // Check referenced_pointers
    for (const auto& parameter : parameters) {
        if (context.referenced_pointers.exists(parameter->get_variable())) {
            const auto& [variable, position, error_handler] = context.scopes.get<pddl::VariableImpl>(parameter->get_variable()->get_name()).value();
            throw UnusedVariableError(variable->get_name(), error_handler(position.value(), ""));
        }
    }

    context.scopes.close_scope();
    auto condition = context.factories.conditions.get_or_create<pddl::ConditionExistsImpl>(parameters, child_condition);
    context.positions.push_back(condition, node);
    return condition;
}

template<typename ConditionNode>
pddl::Condition parse_condition_forall(const domain::ast::TypedListOfVariables& parameters_node, const ConditionNode& condition_node, Context& context) {
    context.scopes.open_scope();
    auto parameters = boost::apply_visitor(ParameterListVisitor(context), parameters_node);
    for (const auto& parameter : parameters) {
        context.referenced_pointers.track(parameter->get_variable());
    }
    auto child_condition = parse(condition_node, context);
    // Check referenced_pointers
    for (const auto& parameter : parameters) {
        if (context.referenced_pointers.exists(parameter->get_variable())) {
            const auto& [variable, position, error_handler] = context.scopes.get<pddl::VariableImpl>(parameter->get_variable()->get_name()).value();
            throw UnusedVariableError(variable->get_name(), error_handler(position.value(), ""));
        }
    }

    context.scopes.close_scope();
    auto condition = context.factories.conditions.get_or_create<pddl::ConditionForallImpl>(parameters, child_condition);
    context.positions.push_back(condition, condition_node);
    return condition;
}

pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.goal_descriptor, context);
}

pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    throw NotImplementedError("parse(const domain::ast::GoalDescriptorFunctionComparison& node, Context& context)");
}


pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& child_node : node.constraint_goal_descriptors) {
        condition_list.push_back(parse(child_node, context));
    }
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.constraint_goal_descriptor, context);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorAlways& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorSometime& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorWithin& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, Context& context)");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, Context& context) {
    throw NotImplementedError("parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, Context& context)");
}


pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, Context& context) {
    return parse(node.goal_descriptor, context);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& child_node : node.precondition_goal_descriptors) {
        condition_list.push_back(parse(child_node, context));
    }
    const auto condition = context.factories.conditions.get_or_create<pddl::ConditionAndImpl>(condition_list);
    context.positions.push_back<pddl::ConditionImpl>(condition, node);
    return condition;
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::PREFERENCES)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::PREFERENCES, context.scopes.get_error_handler()(node, ""));
    }
    throw NotImplementedError("parse(const domain::ast::PreconditionGoalDescriptorPreference& node, Context& context)");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS)) {
        throw UnsupportedRequirementError(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return parse_condition_forall(node.typed_list_of_variables, node.precondition_goal_descriptor, context);
}



ConditionVisitor::ConditionVisitor(Context& context_)
    : context(context_) { }

}
