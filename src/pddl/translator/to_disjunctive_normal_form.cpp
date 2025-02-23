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

#include "to_disjunctive_normal_form.hpp"

#include "loki/details/utils/collections.hpp"
#include "to_negation_normal_form.hpp"
#include "utils.hpp"

namespace loki
{

Condition ToDisjunctiveNormalFormTranslator::translate_level_2(ConditionAnd condition, Repositories& repositories)
{
    auto translated_nested_conditions = this->translate_level_0(condition->get_conditions(), repositories);

    auto disjunctive_parts = ConditionList {};
    auto other_parts = ConditionList {};
    for (const auto translated_part : translated_nested_conditions)
    {
        if (const auto disjunctive_part = std::get_if<ConditionOr>(&translated_part->get_condition()))
        {
            disjunctive_parts.push_back(translated_part);
        }
        else
        {
            other_parts.push_back(translated_part);
        }
    }

    if (disjunctive_parts.empty())
    {
        // No disjunctive parts to distribute
        return repositories.get_or_create_condition(repositories.get_or_create_condition_and(uniquify_elements(other_parts)));
    }

    auto result_parts = ConditionList {};
    if (other_parts.empty())
    {
        // Immediately start with first disjunctive part
        const auto it = disjunctive_parts.begin();
        disjunctive_parts.erase(it);
        result_parts = ConditionList { *it };
    }
    else
    {
        // Start with conjunctive part
        result_parts = ConditionList { repositories.get_or_create_condition(repositories.get_or_create_condition_and(uniquify_elements(other_parts))) };
    }

    while (!disjunctive_parts.empty())
    {
        auto previous_result_parts = std::move(result_parts);
        result_parts = ConditionList {};
        const auto it = disjunctive_parts.begin();
        disjunctive_parts.erase(it);
        const auto& current_parts = std::get<ConditionOr>((**it).get_condition())->get_conditions();
        for (const auto& part1 : previous_result_parts)
        {
            for (const auto& part2 : current_parts)
            {
                result_parts.push_back(flatten(repositories.get_or_create_condition_and(ConditionList { part1, part2 }), repositories));
            }
        }
    }

    return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(uniquify_elements(result_parts))),
                                   repositories);
}

Condition ToDisjunctiveNormalFormTranslator::translate_level_2(ConditionOr condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_or(this->translate_level_0(condition->get_conditions(), repositories)), repositories);
}

Condition ToDisjunctiveNormalFormTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    const auto translated_parameters = this->translate_level_0(condition->get_parameters(), repositories);
    const auto translated_condition = this->translate_level_0(condition->get_condition(), repositories);

    if (const auto translated_disjunctive_condition = std::get_if<ConditionOr>(&translated_condition->get_condition()))
    {
        auto result_parts = ConditionList {};
        for (const auto& part : (*translated_disjunctive_condition)->get_conditions())
        {
            result_parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_exists(translated_parameters, part)));
        }
        return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(result_parts)), repositories);
    }
    return flatten(repositories.get_or_create_condition_exists(translated_parameters, translated_condition), repositories);
}

Condition ToDisjunctiveNormalFormTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    const auto translated_parameters = this->translate_level_0(condition->get_parameters(), repositories);
    const auto translated_condition = this->translate_level_0(condition->get_condition(), repositories);

    if (const auto translated_disjunctive_condition = std::get_if<ConditionOr>(&translated_condition->get_condition()))
    {
        auto result_parts = ConditionList {};
        for (const auto& part : (*translated_disjunctive_condition)->get_conditions())
        {
            result_parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_forall(translated_parameters, part)));
        }
        return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(result_parts)), repositories);
    }
    return flatten(repositories.get_or_create_condition_forall(translated_parameters, translated_condition), repositories);
}

Domain ToDisjunctiveNormalFormTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto nnf_builder = DomainBuilder();
    const auto nnf_domain = ToNegationNormalFormTranslator().translate_level_0(domain, nnf_builder);

    auto& repositories = builder.get_repositories();

    builder.get_name() = nnf_domain->get_name();
    builder.get_filepath() = nnf_domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(nnf_domain->get_requirements(), repositories);
    const auto translated_types = this->translate_level_0(nnf_domain->get_types(), repositories);
    builder.get_types().insert(builder.get_types().end(), translated_types.begin(), translated_types.end());
    const auto translated_constants = this->translate_level_0(nnf_domain->get_constants(), repositories);
    builder.get_constants().insert(builder.get_constants().end(), translated_constants.begin(), translated_constants.end());
    const auto translated_predicates = this->translate_level_0(nnf_domain->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_function_skeletons = this->translate_level_0(nnf_domain->get_function_skeletons(), repositories);
    builder.get_function_skeletons().insert(builder.get_function_skeletons().end(), translated_function_skeletons.begin(), translated_function_skeletons.end());
    const auto translated_actions = this->translate_level_0(nnf_domain->get_actions(), repositories);
    builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());
    const auto translated_axioms = this->translate_level_0(nnf_domain->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result();
}

Problem ToDisjunctiveNormalFormTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto nnf_builder = ProblemBuilder(problem->get_domain());
    const auto nnf_problem = ToNegationNormalFormTranslator().translate_level_0(problem, nnf_builder);

    auto& repositories = builder.get_repositories();

    builder.get_filepath() = nnf_problem->get_filepath();
    builder.get_name() = nnf_problem->get_name();
    builder.get_requirements() = this->translate_level_0(nnf_problem->get_requirements(), repositories);
    const auto translated_objects = this->translate_level_0(nnf_problem->get_objects(), repositories);
    builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
    const auto translated_predicates = this->translate_level_0(nnf_problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_initial_literals = this->translate_level_0(nnf_problem->get_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());
    const auto translated_initial_function_values = this->translate_level_0(nnf_problem->get_initial_function_values(), repositories);
    builder.get_initial_function_values().insert(builder.get_initial_function_values().end(),
                                                 translated_initial_function_values.begin(),
                                                 translated_initial_function_values.end());
    if (nnf_problem->get_goal_condition().has_value())
        builder.get_goal_condition() = this->translate_level_0(nnf_problem->get_goal_condition().value(), repositories);
    if (nnf_problem->get_optimization_metric().has_value())
        builder.get_optimization_metric() = this->translate_level_0(nnf_problem->get_optimization_metric().value(), repositories);
    const auto translated_axioms = this->translate_level_0(nnf_problem->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result(nnf_problem->get_index());
}

}
