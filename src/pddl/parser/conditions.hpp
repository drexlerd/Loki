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

#include "loki/ast/ast.hpp"
#include "loki/pddl/conditions.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/parser.hpp"

#include <variant>

namespace loki
{

extern Condition parse(const ast::GoalDescriptor& node, Context& context);
extern Condition parse(const ast::GoalDescriptorAtom& node, Context& context);
extern Condition parse(const ast::GoalDescriptorLiteral& node, Context& context);
extern Condition parse(const ast::GoalDescriptorAnd& node, Context& context);
extern Condition parse(const ast::GoalDescriptorOr& node, Context& context);
extern Condition parse(const ast::GoalDescriptorNot& node, Context& context);
extern Condition parse(const ast::GoalDescriptorImply& node, Context& context);
extern Condition parse(const ast::GoalDescriptorExists& node, Context& context);
extern Condition parse(const ast::GoalDescriptorForall& node, Context& context);
extern Condition parse(const ast::GoalDescriptorFunctionComparison& node, Context& context);

extern Condition parse(const ast::ConstraintGoalDescriptor& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorAnd& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorForall& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorAtEnd& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorAlways& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorSometime& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorWithin& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorHoldDuring& node, Context& context);
extern Condition parse(const ast::ConstraintGoalDescriptorHoldAfter& node, Context& context);

extern Condition parse(const ast::PreconditionGoalDescriptor& node, Context& context);
extern Condition parse(const ast::PreconditionGoalDescriptorSimple& node, Context& context);
extern Condition parse(const ast::PreconditionGoalDescriptorAnd& node, Context& context);
extern Condition parse(const ast::PreconditionGoalDescriptorPreference& node, Context& context);
extern Condition parse(const ast::PreconditionGoalDescriptorForall& node, Context& context);

struct ConditionVisitor : boost::static_visitor<Condition>
{
    Context& context;

    ConditionVisitor(Context& context_);

    template<typename Node>
    Condition operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
