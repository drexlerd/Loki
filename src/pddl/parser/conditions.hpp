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

#ifndef LOKI_SRC_PDDL_PARSER_CONDITIONS_HPP_
#define LOKI_SRC_PDDL_PARSER_CONDITIONS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/scope.hpp"

#include <variant>

namespace loki
{

template<ParsingContext C>
struct ConditionVisitor : boost::static_visitor<Condition>
{
    C& context;

    ConditionVisitor(C& context_);

    Condition operator()(const ast::GoalDescriptor& node);
    Condition operator()(const ast::GoalDescriptorAtom& node);
    Condition operator()(const ast::GoalDescriptorLiteral& node);
    Condition operator()(const ast::GoalDescriptorAnd& node);
    Condition operator()(const ast::GoalDescriptorOr& node);
    Condition operator()(const ast::GoalDescriptorNot& node);
    Condition operator()(const ast::GoalDescriptorImply& node);
    Condition operator()(const ast::GoalDescriptorExists& node);
    Condition operator()(const ast::GoalDescriptorForall& node);
    Condition operator()(const ast::GoalDescriptorFunctionComparison& node);

    Condition operator()(const ast::ConstraintGoalDescriptor& node);
    Condition operator()(const ast::ConstraintGoalDescriptorAnd& node);
    Condition operator()(const ast::ConstraintGoalDescriptorForall& node);
    Condition operator()(const ast::ConstraintGoalDescriptorAtEnd& node);
    Condition operator()(const ast::ConstraintGoalDescriptorAlways& node);
    Condition operator()(const ast::ConstraintGoalDescriptorSometime& node);
    Condition operator()(const ast::ConstraintGoalDescriptorWithin& node);
    Condition operator()(const ast::ConstraintGoalDescriptorAtMostOnce& node);
    Condition operator()(const ast::ConstraintGoalDescriptorSometimeAfter& node);
    Condition operator()(const ast::ConstraintGoalDescriptorSometimeBefore& node);
    Condition operator()(const ast::ConstraintGoalDescriptorAlwaysWithin& node);
    Condition operator()(const ast::ConstraintGoalDescriptorHoldDuring& node);
    Condition operator()(const ast::ConstraintGoalDescriptorHoldAfter& node);

    Condition operator()(const ast::PreconditionGoalDescriptor& node);
    Condition operator()(const ast::PreconditionGoalDescriptorSimple& node);
    Condition operator()(const ast::PreconditionGoalDescriptorAnd& node);
    Condition operator()(const ast::PreconditionGoalDescriptorPreference& node);
    Condition operator()(const ast::PreconditionGoalDescriptorForall& node);
};

template<ParsingContext C>
extern Condition parse(const ast::GoalDescriptor& node, C& context);

template<ParsingContext C>
extern Condition parse(const ast::ConstraintGoalDescriptor& node, C& context);

template<ParsingContext C>
extern Condition parse(const ast::PreconditionGoalDescriptor& node, C& context);

}

#endif