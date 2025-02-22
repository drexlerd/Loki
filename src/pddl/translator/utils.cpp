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

#include "utils.hpp"

#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/collections.hpp"

#include <iomanip>
#include <loki/details/utils/hash.hpp>
#include <sstream>
#include <string>
#include <unordered_set>

using namespace std::string_literals;

namespace loki
{

/**
 * Conditions
 */

Condition flatten(ConditionAnd condition, Repositories& repositories)
{
    auto parts = ConditionList {};
    for (const auto& part : condition->get_conditions())
    {
        if (const auto and_condition = std::get_if<ConditionAnd>(&part->get_condition()))
        {
            const auto& nested_parts = *std::get<ConditionAnd>(flatten(*and_condition, repositories)->get_condition());

            parts.insert(parts.end(), nested_parts.get_conditions().begin(), nested_parts.get_conditions().end());
        }
        else
        {
            parts.push_back(part);
        }
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_and(uniquify_elements(parts)));
}

Condition flatten(ConditionOr condition, Repositories& repositories)
{
    auto parts = ConditionList {};
    for (const auto& part : condition->get_conditions())
    {
        if (const auto or_condition = std::get_if<ConditionOr>(&part->get_condition()))
        {
            const auto& nested_parts = *std::get<ConditionOr>(flatten(*or_condition, repositories)->get_condition());

            parts.insert(parts.end(), nested_parts.get_conditions().begin(), nested_parts.get_conditions().end());
        }
        else
        {
            parts.push_back(part);
        }
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_or(uniquify_elements(parts)));
}

Condition flatten(ConditionExists condition, Repositories& repositories)
{
    if (const auto condition_exists = std::get_if<ConditionExists>(&condition->get_condition()->get_condition()))
    {
        const auto& nested_condition = *std::get<ConditionExists>(flatten(*condition_exists, repositories)->get_condition());
        auto parameters = condition->get_parameters();
        const auto additional_parameters = nested_condition.get_parameters();
        parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());
        return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(parameters, nested_condition.get_condition()));
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(condition->get_parameters(), condition->get_condition()));
}

Condition flatten(ConditionForall condition, Repositories& repositories)
{
    if (const auto condition_forall = std::get_if<ConditionForall>(&condition->get_condition()->get_condition()))
    {
        const auto& nested_condition = *std::get<ConditionForall>(flatten(*condition_forall, repositories)->get_condition());
        auto parameters = condition->get_parameters();
        const auto additional_parameters = nested_condition.get_parameters();
        parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());
        return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(parameters, nested_condition.get_condition()));
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(condition->get_parameters(), condition->get_condition()));
}

Effect flatten(EffectAnd effect, Repositories& repositories)
{
    auto function_to_numeric_effects =
        std::unordered_map<std::pair<AssignOperatorEnum, Function>, EffectList, Hash<std::pair<AssignOperatorEnum, Function>>> {};
    auto other_effects = EffectList {};

    for (const auto& part : effect->get_effects())
    {
        if (const auto and_effect = std::get_if<EffectAnd>(&part->get_effect()))
        {
            const auto& nested_parts = *std::get<EffectAnd>(flatten(*and_effect, repositories)->get_effect());

            for (const auto& nested_part : nested_parts.get_effects())
            {
                if (const auto numeric_effect = std::get_if<EffectNumeric>(&nested_part->get_effect()))
                {
                    function_to_numeric_effects[std::make_pair((*numeric_effect)->get_assign_operator(), (*numeric_effect)->get_function())].push_back(
                        nested_part);
                }
                else
                {
                    other_effects.push_back(nested_part);
                }
            }
        }
        else
        {
            if (const auto numeric_effect = std::get_if<EffectNumeric>(&part->get_effect()))
            {
                function_to_numeric_effects[std::make_pair((*numeric_effect)->get_assign_operator(), (*numeric_effect)->get_function())].push_back(part);
            }
            else
            {
                other_effects.push_back(part);
            }
        }
    }

    // Sum aggregate function expressions with same (assign_operator and function).
    for (const auto& [key, numeric_effects] : function_to_numeric_effects)
    {
        const auto& [assign_operator, function] = key;

        auto function_expressions = FunctionExpressionList {};
        for (const auto& numeric_effect : numeric_effects)
        {
            function_expressions.push_back(std::get<EffectNumeric>(numeric_effect->get_effect())->get_function_expression());
        }
        other_effects.push_back(repositories.get_or_create_effect(repositories.get_or_create_effect_numeric(
            assign_operator,
            function,
            repositories.get_or_create_function_expression(
                repositories.get_or_create_function_expression_multi_operator(MultiOperatorEnum::PLUS, function_expressions)))));
    }

    return repositories.get_or_create_effect(repositories.get_or_create_effect_and(uniquify_elements(other_effects)));
}

Effect flatten(EffectCompositeWhen effect, Repositories& repositories)
{
    if (const auto effect_when = std::get_if<EffectCompositeWhen>(&effect->get_effect()->get_effect()))
    {
        const auto& nested_effect = *std::get<EffectCompositeWhen>(flatten(*effect_when, repositories)->get_effect());

        return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(
            flatten(repositories.get_or_create_condition_and(uniquify_elements(ConditionList { effect->get_condition(), nested_effect.get_condition() })),
                    repositories),
            nested_effect.get_effect()));
    }
    return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(effect->get_condition(), effect->get_effect()));
}

Effect flatten(EffectCompositeForall effect, Repositories& repositories)
{
    if (const auto effect_forall = std::get_if<EffectCompositeForall>(&effect->get_effect()->get_effect()))
    {
        const auto& nested_effect = *std::get<EffectCompositeForall>(flatten(*effect_forall, repositories)->get_effect());
        auto parameters = effect->get_parameters();
        const auto additional_parameters = nested_effect.get_parameters();
        parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());
        return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(parameters, nested_effect.get_effect()));
    }
    return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(effect->get_parameters(), effect->get_effect()));
}

std::string create_unique_axiom_name(size_t& next_axiom_index, std::unordered_set<std::string>& existing_predicate_names)
{
    auto axiom_name = std::string {};

    do
    {
        axiom_name = "axiom_"s + std::to_string(next_axiom_index++);
    } while (existing_predicate_names.count(axiom_name));
    existing_predicate_names.insert(axiom_name);

    return axiom_name;
}

}