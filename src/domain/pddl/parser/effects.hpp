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

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/effects.hpp"

#include <variant>


namespace loki {

extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorAssign& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleUp& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleDown& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorIncrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorDecrease& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::AssignOperatorEnum parse(const domain::ast::AssignOperator& node, const error_handler_type& error_handler, domain::Context& context);

struct AssignOperatorVisitor : boost::static_visitor<pddl::AssignOperatorEnum> {
    const error_handler_type& error_handler;
    domain::Context& context;

    AssignOperatorVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::AssignOperatorEnum operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};


extern pddl::Effect parse(const domain::ast::Effect& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionNumericFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProductionObjectFluent& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectProduction& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalForall& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, const error_handler_type& error_handler, domain::Context& context);
extern pddl::Effect parse(const domain::ast::EffectConditional& node, const error_handler_type& error_handler, domain::Context& context);

struct EffectProductionVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectProductionVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

struct EffectConditionalVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectConditionalVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    template<typename Node>
    pddl::Effect operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};

struct EffectVisitor : boost::static_visitor<pddl::Effect> {
    const error_handler_type& error_handler;
    domain::Context& context;

    EffectVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Effect operator()(const std::vector<domain::ast::Effect>& effect_nodes) const;

    pddl::Effect operator()(const domain::ast::EffectConditional& effect_node) const;

    pddl::Effect operator()(const domain::ast::EffectProduction& effect_node) const;

};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_EFFECTS_HPP_
