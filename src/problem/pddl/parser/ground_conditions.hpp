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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_

#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/problem/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/conditions.hpp"

namespace loki {

extern pddl::Condition parse(const domain::ast::GoalDescriptor& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, problem::Context& context);

extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, problem::Context& context);

extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, problem::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, problem::Context& context);

struct ConditionVisitor : boost::static_visitor<pddl::Condition> {
    problem::Context& context;

    ConditionVisitor(problem::Context& context_);

    template<typename Node>
    pddl::Condition operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
