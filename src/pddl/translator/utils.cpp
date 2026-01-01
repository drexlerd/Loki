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

static Condition flatten(ConditionLiteral condition, Repositories& repositories) { return repositories.get_or_create_condition(condition); }

static Condition flatten(ConditionNot condition, Repositories& repositories) { return repositories.get_or_create_condition(condition); }

static Condition flatten(ConditionImply condition, Repositories& repositories) { return repositories.get_or_create_condition(condition); }

static Condition flatten(ConditionNumericConstraint condition, Repositories& repositories) { return repositories.get_or_create_condition(condition); }

Condition flatten(ConditionAnd condition, Repositories& repositories)
{
    auto parts = ConditionList {};
    for (const auto& part : condition->get_conditions())
    {
        const auto flat = flatten(part, repositories);

        std::visit(
            [&](auto&& arg)
            {
                using Variant = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Variant, ConditionAnd>)
                {
                    parts.insert(parts.end(), arg->get_conditions().begin(), arg->get_conditions().end());
                }
                else
                {
                    parts.push_back(flat);
                }
            },
            flat->get_condition());
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_and(uniquify_elements(parts)));
}

Condition flatten(ConditionOr condition, Repositories& repositories)
{
    auto parts = ConditionList {};
    for (const auto& part : condition->get_conditions())
    {
        const auto flat = flatten(part, repositories);

        std::visit(
            [&](auto&& arg)
            {
                using Variant = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Variant, ConditionOr>)
                {
                    parts.insert(parts.end(), arg->get_conditions().begin(), arg->get_conditions().end());
                }
                else
                {
                    parts.push_back(flat);
                }
            },
            flat->get_condition());
    }
    return repositories.get_or_create_condition(repositories.get_or_create_condition_or(uniquify_elements(parts)));
}

Condition flatten(ConditionExists condition, Repositories& repositories)
{
    const auto flat = flatten(condition->get_condition(), repositories);

    return std::visit(
        [&](auto&& arg)
        {
            using Variant = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Variant, ConditionExists>)
            {
                auto parameters = condition->get_parameters();
                const auto additional_parameters = arg->get_parameters();
                parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());
                return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(parameters, arg->get_condition()));
            }
            else
            {
                return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(condition->get_parameters(), flat));
            }
        },
        flat->get_condition());
}

Condition flatten(ConditionForall condition, Repositories& repositories)
{
    const auto flat = flatten(condition->get_condition(), repositories);

    return std::visit(
        [&](auto&& arg)
        {
            using Variant = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Variant, ConditionForall>)
            {
                auto parameters = condition->get_parameters();
                const auto additional_parameters = arg->get_parameters();
                parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());
                return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(parameters, arg->get_condition()));
            }
            else
            {
                return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(condition->get_parameters(), flat));
            }
        },
        flat->get_condition());
}

Condition flatten(Condition condition, Repositories& pddl_repositories)
{
    return std::visit([&](auto&& node) -> Condition { return flatten(node, pddl_repositories); }, condition->get_condition());
}

static Effect flatten(EffectLiteral effect, Repositories& repositories) { return repositories.get_or_create_effect(effect); }

static Effect flatten(EffectNumeric effect, Repositories& repositories) { return repositories.get_or_create_effect(effect); }

static Effect flatten(EffectCompositeOneof effect, Repositories& repositories) { return repositories.get_or_create_effect(effect); }

static Effect flatten(EffectCompositeProbabilistic effect, Repositories& repositories) { return repositories.get_or_create_effect(effect); }

Effect flatten(EffectAnd effect, Repositories& repositories)
{
    using Key = std::pair<AssignOperatorEnum, Function>;
    std::unordered_map<Key, EffectList, Hash<Key>> function_to_numeric_effects;
    EffectList other_effects;

    auto handle_part = [&](const Effect& e)
    {
        // Aggregate numerics by (assign_operator, function)
        std::visit(
            [&](auto&& arg)
            {
                using V = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<V, EffectNumeric>)
                {
                    const auto key = std::make_pair(arg->get_assign_operator(), arg->get_function());
                    function_to_numeric_effects[key].push_back(e);
                }
                else
                {
                    other_effects.push_back(e);
                }
            },
            e->get_effect());
    };

    for (const auto& part : effect->get_effects())
    {
        const auto flat = flatten(part, repositories);

        std::visit(
            [&](auto&& arg)
            {
                using V = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<V, EffectAnd>)
                {
                    const auto& cs = arg->get_effects();
                    for (const auto& nested : cs)
                        handle_part(nested);
                }
                else
                {
                    handle_part(flat);
                }
            },
            flat->get_effect());
    }

    // Sum aggregate numeric effects with same (assign_operator, function).
    for (const auto& [key, numeric_effects] : function_to_numeric_effects)
    {
        const auto& [assign_operator, function] = key;

        FunctionExpressionList function_expressions;
        function_expressions.reserve(numeric_effects.size());

        for (const auto& part : numeric_effects)
        {
            function_expressions.push_back(std::get<EffectNumeric>(part->get_effect())->get_function_expression());
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
    const auto flat = flatten(effect->get_effect(), repositories);

    return std::visit(
        [&](auto&& arg) -> Effect
        {
            using Variant = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Variant, EffectCompositeWhen>)
            {
                // when(c1, when(c2, e))  =>  when(and(c1,c2), e)
                const auto merged_condition = flatten(repositories.get_or_create_condition(repositories.get_or_create_condition_and(
                                                          uniquify_elements(ConditionList { effect->get_condition(), arg->get_condition() }))),
                                                      repositories);

                return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(merged_condition, arg->get_effect()));
            }
            else
            {
                return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(effect->get_condition(), flat));
            }
        },
        flat->get_effect());
}

Effect flatten(EffectCompositeForall effect, Repositories& repositories)
{
    const auto flat = flatten(effect->get_effect(), repositories);

    return std::visit(
        [&](auto&& arg) -> Effect
        {
            using Variant = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Variant, EffectCompositeForall>)
            {
                auto parameters = effect->get_parameters();
                const auto& additional_parameters = arg->get_parameters();
                parameters.insert(parameters.end(), additional_parameters.begin(), additional_parameters.end());

                return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(parameters, arg->get_effect()));
            }
            else
            {
                return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(effect->get_parameters(), flat));
            }
        },
        flat->get_effect());
}

Effect flatten(Effect effect, Repositories& pddl_repositories)
{
    return std::visit([&](auto&& node) -> Effect { return flatten(node, pddl_repositories); }, effect->get_effect());
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