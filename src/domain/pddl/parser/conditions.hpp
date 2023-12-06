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
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/conditions.hpp"

#include <variant>


namespace loki {

extern pddl::Condition parse(const domain::ast::GoalDescriptor& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAtom& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorLiteral& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorAnd& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorOr& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorNot& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorImply& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorExists& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorForall& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::GoalDescriptorFunctionComparison& node, domain::Context& context);

extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptor& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAnd& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorForall& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtEnd& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlways& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometime& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorWithin& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAtMostOnce& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeAfter& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorSometimeBefore& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorAlwaysWithin& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldDuring& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::ConstraintGoalDescriptorHoldAfter& node, domain::Context& context);

extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptor& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorSimple& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorAnd& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorPreference& node, domain::Context& context);
extern pddl::Condition parse(const domain::ast::PreconditionGoalDescriptorForall& node, domain::Context& context);

struct ConditionVisitor : boost::static_visitor<pddl::Condition> {
    domain::Context& context;

    ConditionVisitor(domain::Context& context_);

    template<typename Node>
    pddl::Condition operator()(const Node& node) const {
        return parse(node, context);
    }
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONDITIONS_HPP_
