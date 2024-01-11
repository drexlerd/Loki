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

#include <loki/domain/ast/ast.hpp>
#include <loki/domain/pddl/parser.hpp>
#include <loki/domain/pddl/declarations.hpp>
#include <loki/domain/pddl/conditions.hpp>

#include <variant>


namespace loki {

extern pddl::Condition parse(const domain::ast::GoalDescriptor& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, Context& context);

extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, Context& context);

extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, Context& context);

struct ConditionVisitor : boost::static_visitor<pddl::Condition> {
    Context& context;

    ConditionVisitor(Context& context_);

    template<typename Node>
    pddl::Condition operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
