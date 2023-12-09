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

#include "ground_conditions.hpp"

#include "ground_literal.hpp"

#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../../include/loki/problem/pddl/ground_conditions.hpp"


namespace loki {
// parse a vector of goal descriptors
template<typename T>
static pddl::ConditionList parse(const std::vector<T>& nodes, problem::Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& node : nodes) {
        condition_list.push_back(parse(node, context));
    }
    return condition_list;
}

pddl::Condition parse(const domain::ast::GoalDescriptor& node, problem::Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, problem::Context& context) {
    return context.cache.get_or_create<pddl::ConditionGroundLiteralImpl>(parse(node.atom, context));
}

pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, problem::Context& context) {
    // requires :negative-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS, context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::ConditionGroundLiteralImpl>(parse(node.literal, context));
}

pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, problem::Context& context) {
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, problem::Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.error_stream->str());
    }
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    return context.cache.get_or_create<pddl::ConditionOrImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, problem::Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        context.error_handler(node, "");
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.error_stream->str());
    }
    pddl::Condition condition = parse(node.goal_descriptor, context);
    return context.cache.get_or_create<pddl::ConditionNotImpl>(condition);
}

pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorImply");
}

pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorExists");
}

pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorFunctionComparison");
}


pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, problem::Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, problem::Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& child_node : node.constraint_goal_descriptors) {
        condition_list.push_back(parse(child_node, context));
    }
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtEnd");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlways");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometime");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtMostOnce");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeAfter");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeBefore");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlwaysWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldDuring");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldAfter");
}


pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, problem::Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, problem::Context& context) {
    return parse(node.goal_descriptor, context);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, problem::Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& child_node : node.precondition_goal_descriptors) {
        condition_list.push_back(parse(child_node, context));
    }
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorPreference");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, problem::Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorForall");
}



ConditionVisitor::ConditionVisitor(problem::Context& context_)
    : context(context_) { }

}
