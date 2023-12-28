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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_

#include "../../../../include/domain/ast/ast.hpp"
#include "../../../../include/domain/pddl/parser.hpp"
#include "../../../../include/domain/pddl/declarations.hpp"
#include "../../../../include/domain/pddl/effects.hpp"

#include <variant>


namespace loki {

extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorAssign& node);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleUp& node);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleDown& node);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorIncrease& node);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorDecrease& node);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperator& node);

struct AssignOperatorVisitor : boost::static_visitor<pddl::AssignOperatorEnum> {
    template<typename Node>
    pddl::AssignOperatorEnum operator()(const Node& node) const {
        return parse(node);
    }
};

extern pddl::Effect parse(const domain::ast::Effect& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionNumericFluentTotalCost& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionNumericFluentGeneral& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectProduction& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalForall& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditional& node, Context& context);

struct EffectProductionVisitor : boost::static_visitor<pddl::Effect> {
    Context& context;

    EffectProductionVisitor(Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, context);
    }
};

struct EffectConditionalVisitor : boost::static_visitor<pddl::Effect> {
    Context& context;

    EffectConditionalVisitor(Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, context);
    }
};

struct EffectVisitor : boost::static_visitor<pddl::Effect> {
    Context& context;

    EffectVisitor(Context& context_);

    pddl::Effect operator()(const std::vector<domain::ast::Effect>& effect_nodes) const;

    pddl::Effect operator()(const domain::ast::EffectConditional& effect_node) const;

    pddl::Effect operator()(const domain::ast::EffectProduction& effect_node) const;

};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
