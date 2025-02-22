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
#include "error_handling.hpp"
#include "functions.hpp"
#include "literal.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "parameters.hpp"
#include "reference_utils.hpp"

namespace loki
{

/**
 * AssignOperatorEnum
 */

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorAssign&) const { return AssignOperatorEnum::ASSIGN; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorScaleUp&) const { return AssignOperatorEnum::SCALE_UP; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorScaleDown&) const { return AssignOperatorEnum::SCALE_DOWN; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorIncrease&) const { return AssignOperatorEnum::INCREASE; }

AssignOperatorEnum AssignOperatorVisitor::operator()(const ast::AssignOperatorDecrease&) const { return AssignOperatorEnum::DECREASE; }

AssignOperatorEnum parse(const ast::AssignOperator& node) { return boost::apply_visitor(AssignOperatorVisitor(), node); }

/**
 * Effect
 */

template<ParsingContext C>
EffectVisitor<C>::EffectVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const std::vector<ast::Effect>& effect_nodes)
{
    auto effect_list = EffectList();
    for (const auto& effect_node : effect_nodes)
    {
        effect_list.push_back(this->operator()(effect_node));
    }
    return context.builder.get_or_create_effect(context.builder.get_or_create_effect_and(effect_list));
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::Effect& node)
{
    return parse(node, context);
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectProductionLiteral& node)
{
    auto literal = parse(node.literal, context);
    const auto effect = context.builder.get_or_create_effect(context.builder.get_or_create_effect_literal(literal));

    context.positions.push_back(effect, node);
    return effect;
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectProductionNumeric& node)
{
    // "total-cost" explicitly requires :action-costs to be the specified in the requirements section.
    if (node.function_head.function_symbol.name.characters == "total-cost")
    {
        test_undefined_requirement(RequirementEnum::ACTION_COSTS, node, context);
        context.references.untrack(RequirementEnum::ACTION_COSTS);
    }
    // all remaining function heads require :numeric-fluents to be specified in the requirements section.
    else
    {
        test_undefined_requirement(RequirementEnum::NUMERIC_FLUENTS, node, context);
        context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);
    }
    const auto assign_operator = parse(node.assign_operator);
    const auto function = parse(node.function_head, context);
    context.references.untrack(function->get_function_skeleton());
    auto fexpr_visitor = FunctionExpressionVisitor(context);
    const auto function_expression = boost::apply_visitor(fexpr_visitor, node.function_expression);
    const auto effect = context.builder.get_or_create_effect(context.builder.get_or_create_effect_numeric(assign_operator, function, function_expression));
    context.positions.push_back(effect, node);
    return effect;
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectProduction& node)
{
    auto visitor = EffectVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectCompositeForall& node)
{
    test_undefined_requirement(RequirementEnum::CONDITIONAL_EFFECTS, node, context);
    context.references.untrack(RequirementEnum::CONDITIONAL_EFFECTS);
    context.scopes.open_scope();
    auto parameter_list_visitor = ParameterListVisitor(context);
    const auto parameter_list = boost::apply_visitor(parameter_list_visitor, node.typed_list_of_variables);
    track_variable_references(parameter_list, context);
    const auto child_effect = this->operator()(node.effect);
    test_variable_references(parameter_list, context);
    context.scopes.close_scope();
    const auto effect = context.builder.get_or_create_effect(context.builder.get_or_create_effect_composite_forall(parameter_list, child_effect));
    context.positions.push_back(effect, node);
    return effect;
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectCompositeWhen& node)
{
    test_undefined_requirement(RequirementEnum::CONDITIONAL_EFFECTS, node, context);
    context.references.untrack(RequirementEnum::CONDITIONAL_EFFECTS);
    context.scopes.open_scope();
    const auto condition = parse(node.goal_descriptor, context);
    const auto child_effect = this->operator()(node.effect);
    context.scopes.close_scope();
    const auto effect = context.builder.get_or_create_effect(context.builder.get_or_create_effect_composite_when(condition, child_effect));
    context.positions.push_back(effect, node);
    return effect;
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectCompositeOneof& node)
{
    test_undefined_requirement(RequirementEnum::NON_DETERMINISTIC, node, context);
    context.references.untrack(RequirementEnum::NON_DETERMINISTIC);
    auto effect_list = EffectList();
    for (const auto& effect_node : node.possibilities)
    {
        effect_list.push_back(this->operator()(effect_node));
    }
    return context.builder.get_or_create_effect(context.builder.get_or_create_effect_composite_oneof(effect_list));
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectCompositeProbabilistic& node)
{
    test_undefined_requirement(RequirementEnum::NON_DETERMINISTIC, node, context);
    context.references.untrack(RequirementEnum::NON_DETERMINISTIC);

    auto effect_distribution = EffectDistribution();
    for (const auto& number_and_effect : node.possibilities)
    {
        effect_distribution.emplace_back(parse(number_and_effect.number), this->operator()(number_and_effect.effect));
    }
    return context.builder.get_or_create_effect(context.builder.get_or_create_effect_composite_probabilistic(effect_distribution));
}

template<ParsingContext C>
Effect EffectVisitor<C>::operator()(const ast::EffectComposite& node)
{
    auto visitor = EffectVisitor(context);
    const auto effect = boost::apply_visitor(visitor, node);
    context.positions.push_back(effect, node);
    return effect;
}

template struct EffectVisitor<DomainParsingContext>;
template struct EffectVisitor<ProblemParsingContext>;

template<ParsingContext C>
Effect parse(const ast::Effect& node, C& context)
{
    auto visitor = EffectVisitor(context);
    return boost::apply_visitor(visitor, node);
}

template Effect parse(const ast::Effect& node, DomainParsingContext& context);
template Effect parse(const ast::Effect& node, ProblemParsingContext& context);

}
