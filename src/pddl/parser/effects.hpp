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

#ifndef LOKI_SRC_PDDL_PARSER_EFFECTS_HPP_
#define LOKI_SRC_PDDL_PARSER_EFFECTS_HPP_

#include "loki/ast/ast.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/parser.hpp"

#include <variant>

namespace loki
{

/* AssignOperator */
extern pddl::AssignOperatorEnum parse(const ast::AssignOperatorAssign& node);
extern pddl::AssignOperatorEnum parse(const ast::AssignOperatorScaleUp& node);
extern pddl::AssignOperatorEnum parse(const ast::AssignOperatorScaleDown& node);
extern pddl::AssignOperatorEnum parse(const ast::AssignOperatorIncrease& node);
extern pddl::AssignOperatorEnum parse(const ast::AssignOperatorDecrease& node);
extern pddl::AssignOperatorEnum parse(const ast::AssignOperator& node);

struct AssignOperatorVisitor : boost::static_visitor<pddl::AssignOperatorEnum>
{
    template<typename Node>
    pddl::AssignOperatorEnum operator()(const Node& node) const
    {
        return parse(node);
    }
};

/* Effects */
extern pddl::Effect parse(const std::vector<ast::Effect>& effect_nodes, Context& context);
extern pddl::Effect parse(const ast::Effect& node, Context& context);
extern pddl::Effect parse(const ast::EffectProductionLiteral& node, Context& context);
extern pddl::Effect parse(const ast::EffectProductionNumericFluentTotalCost& node, Context& context);
extern pddl::Effect parse(const ast::EffectProductionNumericFluentGeneral& node, Context& context);
extern pddl::Effect parse(const ast::EffectProduction& node, Context& context);
extern pddl::Effect parse(const ast::EffectConditionalForall& node, Context& context);
extern pddl::Effect parse(const ast::EffectConditionalWhen& node, Context& context);
extern pddl::Effect parse(const ast::EffectConditional& node, Context& context);

struct EffectVisitor : boost::static_visitor<pddl::Effect>
{
    Context& context;

    EffectVisitor(Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
