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

#include "effects.hpp"

#include "conditions.hpp"
#include "literal.hpp"
#include "parameters.hpp"
#include "functions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"


namespace loki {

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorAssign&) {
    return pddl::AssignOperatorEnum::ASSIGN;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleUp&) {
    return pddl::AssignOperatorEnum::SCALE_UP;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorScaleDown&) {
    return pddl::AssignOperatorEnum::SCALE_DOWN;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorIncrease&) {
    return pddl::AssignOperatorEnum::INCREASE;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperatorDecrease&) {
    return pddl::AssignOperatorEnum::DECREASE;
}

pddl::AssignOperatorEnum parse(const domain::ast::AssignOperator& node) {
    return boost::apply_visitor(AssignOperatorVisitor(), node);
}


pddl::Effect parse(const domain::ast::Effect& node, Context& context) {
    return boost::apply_visitor(EffectVisitor(context), node);
}

pddl::Effect parse(const domain::ast::EffectProductionLiteral& node, Context& context) {
    auto literal = parse(node.literal, context);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectLiteralImpl>(literal);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluentTotalCost& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::ACTION_COSTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::ACTION_COSTS, context.scopes.get_error_handler()(node, ""));
    }
    const auto assign_operator_increase = parse(node.assign_operator_increase);
    auto function_name = "total-cost";
    auto binding = context.scopes.get<pddl::FunctionSkeletonImpl>(function_name);
    if (!binding.has_value()) {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.get_error_handler()(node.function_symbol_total_cost, ""));
    }
    const auto& [function_skeleton, _position, _error_handler] = binding.value();
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, pddl::TermList{});
    const auto function_expression = boost::apply_visitor(FunctionExpressionVisitor(context), node.numeric_term);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectNumericImpl>(assign_operator_increase, function, function_expression);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}

pddl::Effect parse(const domain::ast::EffectProductionNumericFluentGeneral& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    const auto assign_operator = parse(node.assign_operator);
    const auto function = parse(node.function_head, context);
    const auto function_expression = parse(node.function_expression, context);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectNumericImpl>(assign_operator, function, function_expression);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}

pddl::Effect parse(const domain::ast::EffectProduction& node, Context& context) {
    return boost::apply_visitor(EffectProductionVisitor(context), node);
}

pddl::Effect parse(const domain::ast::EffectConditionalForall& node, Context& context) {
    context.scopes.open_scope();
    const auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    const auto child_effect = parse(node.effect, context);
    context.scopes.close_scope();
    const auto effect = context.factories.effects.get_or_create<pddl::EffectConditionalForallImpl>(parameter_list, child_effect);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}

pddl::Effect parse(const domain::ast::EffectConditionalWhen& node, Context& context) {
    context.scopes.open_scope();
    const auto condition = parse(node.goal_descriptor, context);
    const auto child_effect = parse(node.effect, context);
    context.scopes.close_scope();
    const auto effect = context.factories.effects.get_or_create<pddl::EffectConditionalWhenImpl>(condition, child_effect);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}

pddl::Effect parse(const domain::ast::EffectConditional& node, Context& context) {
    // requires :conditional-effects
    if (!context.requirements->test(pddl::RequirementEnum::CONDITIONAL_EFFECTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::CONDITIONAL_EFFECTS, context.scopes.get_error_handler()(node, ""));
    }
    const auto effect = boost::apply_visitor(EffectConditionalVisitor(context), node);
    context.positions.push_back<pddl::EffectImpl>(effect, node);
    return effect;
}


EffectProductionVisitor::EffectProductionVisitor(Context& context_)
    : context(context_) { }


EffectConditionalVisitor::EffectConditionalVisitor(Context& context_)
    : context(context_) { }


EffectVisitor::EffectVisitor(Context& context_)
    : context(context_) { }

pddl::Effect EffectVisitor::operator()(const std::vector<domain::ast::Effect>& effect_nodes) const {
    pddl::EffectList effect_list;
    for (const auto& effect_node : effect_nodes) {
        effect_list.push_back(parse(effect_node, context));
    }
    return context.factories.effects.get_or_create<pddl::EffectAndImpl>(effect_list);
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectConditional& node) const {
    const auto effect = boost::apply_visitor(EffectConditionalVisitor(context), node);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect EffectVisitor::operator()(const domain::ast::EffectProduction& node) const {
    const auto effect = boost::apply_visitor(EffectProductionVisitor(context), node);
    context.positions.push_back(effect, node);
    return effect;
}

}