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

#include "../../../../include/loki/domain/pddl/exceptions.hpp"


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
    return context.cache.get_or_create<pddl::ConditionLiteralImpl>(parse(node.atom, context));
}
pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, Context& context) {
    // requires :negative-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    return context.cache.get_or_create<pddl::ConditionLiteralImpl>(parse(node.literal, context));
}

pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, Context& context) {
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    pddl::ConditionList condition_list = parse(node.goal_descriptors, context);
    return context.cache.get_or_create<pddl::ConditionOrImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, Context& context) {
    // requires :disjunctive-preconditions
    if (!context.requirements->test(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS, context.scopes.get_error_handler()(node, ""));
    }
    pddl::Condition condition = parse(node.goal_descriptor, context);
    return context.cache.get_or_create<pddl::ConditionNotImpl>(condition);
}

pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorImply");
}

pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorExists");
}

pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, Context& context) {
    throw NotImplementedError("parse domain::ast::GoalDescriptorFunctionComparison");
}


pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, Context& context) {
    return boost::apply_visitor(ConditionVisitor(context), node);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, Context& context) {
    pddl::ConditionList condition_list;
    for (const auto& child_node : node.constraint_goal_descriptors) {
        condition_list.push_back(parse(child_node, context));
    }
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorForall");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtEnd");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlways");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometime");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAtMostOnce");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeAfter");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorSometimeBefore");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorAlwaysWithin");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldDuring");
}

pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, Context& context) {
    throw NotImplementedError("parse domain::ast::ConstraintGoalDescriptorHoldAfter");
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
    return context.cache.get_or_create<pddl::ConditionAndImpl>(condition_list);
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorPreference");
}

pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, Context& context) {
    throw NotImplementedError("parse domain::ast::PreconditionGoalDescriptorForall");
}



ConditionVisitor::ConditionVisitor(Context& context_)
    : context(context_) { }

}
