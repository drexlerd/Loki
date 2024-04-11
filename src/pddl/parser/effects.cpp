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

#include "common.hpp"
#include "conditions.hpp"
#include "functions.hpp"
#include "literal.hpp"
#include "loki/pddl/exceptions.hpp"
#include "parameters.hpp"
#include "reference_utils.hpp"

namespace loki
{

pddl::AssignOperatorEnum parse(const ast::AssignOperatorAssign&) { return pddl::AssignOperatorEnum::ASSIGN; }

pddl::AssignOperatorEnum parse(const ast::AssignOperatorScaleUp&) { return pddl::AssignOperatorEnum::SCALE_UP; }

pddl::AssignOperatorEnum parse(const ast::AssignOperatorScaleDown&) { return pddl::AssignOperatorEnum::SCALE_DOWN; }

pddl::AssignOperatorEnum parse(const ast::AssignOperatorIncrease&) { return pddl::AssignOperatorEnum::INCREASE; }

pddl::AssignOperatorEnum parse(const ast::AssignOperatorDecrease&) { return pddl::AssignOperatorEnum::DECREASE; }

pddl::AssignOperatorEnum parse(const ast::AssignOperator& node) { return boost::apply_visitor(AssignOperatorVisitor(), node); }

pddl::Effect parse(const std::vector<ast::Effect>& effect_nodes, Context& context)
{
    auto effect_list = pddl::EffectList();
    for (const auto& effect_node : effect_nodes)
    {
        effect_list.push_back(parse(effect_node, context));
    }
    return context.factories.effects.get_or_create<pddl::EffectAndImpl>(effect_list);
}

pddl::Effect parse(const ast::Effect& node, Context& context) { return boost::apply_visitor(EffectVisitor(context), node); }

pddl::Effect parse(const ast::EffectProductionLiteral& node, Context& context)
{
    auto literal = parse(node.literal, context);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectLiteralImpl>(literal);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect parse(const ast::EffectProductionNumericFluentTotalCost& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::ACTION_COSTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::ACTION_COSTS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::ACTION_COSTS);
    const auto assign_operator_increase = parse(node.assign_operator_increase);
    auto function_name = parse(node.function_symbol_total_cost.name);
    assert(function_name == "total-cost");
    auto binding = context.scopes.get_function_skeleton(function_name);
    if (!binding.has_value())
    {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.get_error_handler()(node.function_symbol_total_cost, ""));
    }
    const auto [function_skeleton, _position, _error_handler] = binding.value();
    const auto function = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, pddl::TermList {});
    context.references.untrack(function->get_function_skeleton());
    const auto function_expression = boost::apply_visitor(FunctionExpressionVisitor(context), node.numeric_term);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectNumericImpl>(assign_operator_increase, function, function_expression);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect parse(const ast::EffectProductionNumericFluentGeneral& node, Context& context)
{
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::NUMERIC_FLUENTS);
    const auto assign_operator = parse(node.assign_operator);
    const auto function = parse(node.function_head, context);
    context.references.untrack(function->get_function_skeleton());
    const auto function_expression = parse(node.function_expression, context);
    const auto effect = context.factories.effects.get_or_create<pddl::EffectNumericImpl>(assign_operator, function, function_expression);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect parse(const ast::EffectProduction& node, Context& context) { return boost::apply_visitor(EffectVisitor(context), node); }

pddl::Effect parse(const ast::EffectConditionalForall& node, Context& context)
{
    context.scopes.open_scope();
    const auto parameter_list = boost::apply_visitor(ParameterListVisitor(context), node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    const auto child_effect = parse(node.effect, context);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    const auto effect = context.factories.effects.get_or_create<pddl::EffectConditionalForallImpl>(parameter_list, child_effect);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect parse(const ast::EffectConditionalWhen& node, Context& context)
{
    context.scopes.open_scope();
    const auto condition = parse(node.goal_descriptor, context);
    const auto child_effect = parse(node.effect, context);
    context.scopes.close_scope();
    const auto effect = context.factories.effects.get_or_create<pddl::EffectConditionalWhenImpl>(condition, child_effect);
    context.positions.push_back(effect, node);
    return effect;
}

pddl::Effect parse(const ast::EffectConditional& node, Context& context)
{
    // requires :conditional-effects
    if (!context.requirements->test(pddl::RequirementEnum::CONDITIONAL_EFFECTS))
    {
        throw UndefinedRequirementError(pddl::RequirementEnum::CONDITIONAL_EFFECTS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::CONDITIONAL_EFFECTS);
    const auto effect = boost::apply_visitor(EffectVisitor(context), node);
    context.positions.push_back(effect, node);
    return effect;
}

EffectVisitor::EffectVisitor(Context& context_) : context(context_) {}

}