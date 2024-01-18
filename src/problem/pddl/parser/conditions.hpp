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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_GROUND_CONDITIONS_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_GROUND_CONDITIONS_HPP_

#include <loki/domain/ast/ast.hpp>
#include <loki/problem/pddl/declarations.hpp>
#include <loki/problem/pddl/parser.hpp>


namespace loki {

/* This is mostly copied from the domain part for simplicity.
   We want a common interface in the future to avoid code duplication... */

struct GroundConditionVisitor : boost::static_visitor<pddl::GroundCondition> {
    Context& context;

    GroundConditionVisitor(Context& context_);

public:
    pddl::GroundCondition parse(const domain::ast::GoalDescriptor& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorAtom& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorLiteral& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorAnd& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorOr& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorNot& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorImply& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorExists& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorForall& node);
    pddl::GroundCondition parse(const domain::ast::GoalDescriptorFunctionComparison& node);

    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptor& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorAnd& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorForall& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorAlways& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorSometime& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorWithin& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node);
    pddl::GroundCondition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node);

    pddl::GroundCondition parse(const domain::ast::PreconditionGoalDescriptor& node);
    pddl::GroundCondition parse(const domain::ast::PreconditionGoalDescriptorSimple& node);
    pddl::GroundCondition parse(const domain::ast::PreconditionGoalDescriptorAnd& node);
    pddl::GroundCondition parse(const domain::ast::PreconditionGoalDescriptorPreference& node);
    pddl::GroundCondition parse(const domain::ast::PreconditionGoalDescriptorForall& node);

};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
