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

#include "initialize_metric.hpp"

#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

namespace loki
{

static FunctionSkeleton get_or_create_total_cost_function_skeleton(Repositories& repositories)
{
    return repositories.get_or_create_function_skeleton("total-cost", ParameterList {}, repositories.get_or_create_type("number", TypeList {}));
}

static Function get_or_create_total_cost_function(Repositories& repositories)
{
    return repositories.get_or_create_function(get_or_create_total_cost_function_skeleton(repositories), TermList {});
}

static FunctionExpression get_or_create_total_cost_function_expression(Repositories& repositories)
{
    return repositories.get_or_create_function_expression(
        repositories.get_or_create_function_expression_function(get_or_create_total_cost_function(repositories)));
}

static FunctionExpression get_or_create_constant_one_function_expression(Repositories& repositories)
{
    return repositories.get_or_create_function_expression(repositories.get_or_create_function_expression_number(1));
}

static OptimizationMetric get_or_create_total_cost_metric(Repositories& repositories)
{
    return repositories.get_or_create_optimization_metric(OptimizationMetricEnum::MINIMIZE, get_or_create_total_cost_function_expression(repositories));
}

static Effect get_or_create_total_cost_numeric_effect(Repositories& repositories)
{
    return repositories.get_or_create_effect(repositories.get_or_create_effect_numeric(AssignOperatorEnum::INCREASE,
                                                                                       get_or_create_total_cost_function(repositories),
                                                                                       get_or_create_constant_one_function_expression(repositories)));
}

static FunctionValue get_or_create_initial_total_cost_function_value(Repositories& repositories)
{
    return repositories.get_or_create_function_value(get_or_create_total_cost_function(repositories), 0);
}

Action InitializeMetricTranslator::translate_level_2(Action action, Repositories& repositories)
{
    const auto translated_parameters = this->translate_level_0(action->get_parameters(), repositories);
    const auto translated_condition =
        (action->get_condition().has_value() ? std::optional<loki::Condition>(this->translate_level_0(action->get_condition().value(), repositories)) :
                                               std::nullopt);
    auto translated_effect =
        (action->get_effect().has_value() ? std::optional<loki::Effect>(this->translate_level_0(action->get_effect().value(), repositories)) : std::nullopt);

    // Note: if metric is defined and :action-costs is disabled, then :numeric-fluents is enabled and we must not add numeric effects.
    if (!m_has_metric_defined && !m_action_costs_enabled)
    {
        if (translated_effect.has_value())
        {
            translated_effect =
                flatten(repositories.get_or_create_effect_and(EffectList { translated_effect.value(), get_or_create_total_cost_numeric_effect(repositories) }),
                        repositories);
        }
        else
        {
            translated_effect = get_or_create_total_cost_numeric_effect(repositories);
        }
    }

    return repositories.get_or_create_action(action->get_name(), action->get_original_arity(), translated_parameters, translated_condition, translated_effect);
}

Domain InitializeMetricTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    /* Initialize total-cost function skeleton */
    auto translated_function_skeletons = this->translate_level_0(domain->get_function_skeletons(), repositories);
    translated_function_skeletons.push_back(get_or_create_total_cost_function_skeleton(repositories));

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
    const auto translated_types = this->translate_level_0(domain->get_types(), repositories);
    builder.get_types().insert(builder.get_types().end(), translated_types.begin(), translated_types.end());
    const auto translated_constants = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_constants().insert(builder.get_constants().end(), translated_constants.begin(), translated_constants.end());
    const auto translated_predicates = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());

    builder.get_function_skeletons().insert(builder.get_function_skeletons().end(), translated_function_skeletons.begin(), translated_function_skeletons.end());
    const auto translated_actions = this->translate_level_0(domain->get_actions(), repositories);
    builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());
    const auto translated_axioms = this->translate_level_0(domain->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result();
}

Problem InitializeMetricTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    /* Initialize initial total-cost function value. */
    auto translated_initial_function_values = this->translate_level_0(problem->get_initial_function_values(), repositories);
    if (!problem->get_optimization_metric().has_value())
    {
        translated_initial_function_values.push_back(get_or_create_initial_total_cost_function_value(repositories));
    }

    /* Initialize total-cost metric. */
    builder.get_optimization_metric() = problem->get_optimization_metric().has_value() ?
                                            this->translate_level_0(problem->get_optimization_metric().value(), repositories) :
                                            get_or_create_total_cost_metric(repositories);

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();
    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
    const auto translated_objects = this->translate_level_0(problem->get_objects(), repositories);
    builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
    const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());

    builder.get_initial_function_values().insert(builder.get_initial_function_values().end(),
                                                 translated_initial_function_values.begin(),
                                                 translated_initial_function_values.end());
    if (problem->get_goal_condition().has_value())
        builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition().value(), repositories);

    const auto translated_axioms = this->translate_level_0(problem->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result(problem->get_index());
}
}