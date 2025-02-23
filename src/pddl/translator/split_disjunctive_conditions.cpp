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

#include "split_disjunctive_conditions.hpp"

#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

using namespace std::string_literals;

namespace loki
{

ActionList SplitDisjunctiveConditionsTranslator::split_actions_at_disjunction(const ActionList& actions, Repositories& repositories)
{
    auto translated_split_actions = ActionList {};

    for (const auto& action : actions)
    {
        auto condition = action->get_condition();
        if (condition.has_value() && std::holds_alternative<ConditionOr>(condition.value()->get_condition()))
        {
            for (const auto& part : std::get<ConditionOr>(condition.value()->get_condition())->get_conditions())
            {
                translated_split_actions.push_back(repositories.get_or_create_action(
                    action->get_name(),
                    action->get_original_arity(),
                    this->translate_level_0(action->get_parameters(), repositories),
                    this->translate_level_0(part, repositories),
                    action->get_effect().has_value() ? std::optional<Effect>(this->translate_level_0(action->get_effect().value(), repositories)) :
                                                       std::nullopt));
            }
        }
        else
        {
            translated_split_actions.push_back(this->translate_level_0(action, repositories));
        }
    }
    return uniquify_elements(translated_split_actions);
}

AxiomList SplitDisjunctiveConditionsTranslator::split_axioms_at_disjunction(const AxiomList& axioms, Repositories& repositories)
{
    auto translated_split_axioms = AxiomList {};

    for (const auto& axiom : axioms)
    {
        auto condition = axiom->get_condition();
        if (condition && std::holds_alternative<ConditionOr>(condition->get_condition()))
        {
            for (const auto& part : std::get<ConditionOr>(condition->get_condition())->get_conditions())
            {
                translated_split_axioms.push_back(repositories.get_or_create_axiom(this->translate_level_0(axiom->get_parameters(), repositories),
                                                                                   this->translate_level_0(axiom->get_literal(), repositories),
                                                                                   this->translate_level_0(part, repositories)));
            }
        }
        else
        {
            translated_split_axioms.push_back(this->translate_level_0(axiom, repositories));
        }
    }
    return uniquify_elements(translated_split_axioms);
}

Effect SplitDisjunctiveConditionsTranslator::translate_level_2(EffectCompositeWhen effect, Repositories& repositories)
{
    const auto& condition = effect->get_condition();
    if (condition && std::holds_alternative<ConditionOr>(condition->get_condition()))
    {
        auto split_effects = EffectList {};
        for (const auto part : std::get<ConditionOr>(condition->get_condition())->get_conditions())
        {
            split_effects.push_back(repositories.get_or_create_effect(
                repositories.get_or_create_effect_composite_when(this->translate_level_0(part, repositories),
                                                                 this->translate_level_0(effect->get_effect(), repositories))));
        }
        return repositories.get_or_create_effect(repositories.get_or_create_effect_and(split_effects));
    }
    else
    {
        return repositories.get_or_create_effect(
            repositories.get_or_create_effect_composite_when(this->translate_level_0(effect->get_condition(), repositories),
                                                             this->translate_level_0(effect->get_effect(), repositories)));
    }
}

Domain SplitDisjunctiveConditionsTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    // Split actions
    auto translated_actions = split_actions_at_disjunction(domain->get_actions(), repositories);

    // Split axioms
    auto translated_axioms = split_axioms_at_disjunction(domain->get_axioms(), repositories);

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
    const auto translated_types = this->translate_level_0(domain->get_types(), repositories);
    builder.get_types().insert(builder.get_types().end(), translated_types.begin(), translated_types.end());
    const auto translated_constants = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_constants().insert(builder.get_constants().end(), translated_constants.begin(), translated_constants.end());
    const auto translated_predicates = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_function_skeletons = this->translate_level_0(domain->get_functions(), repositories);
    builder.get_function_skeletons().insert(builder.get_function_skeletons().end(), translated_function_skeletons.begin(), translated_function_skeletons.end());

    builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());

    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result();
}

Problem SplitDisjunctiveConditionsTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    // Split axioms
    auto translated_axioms = split_axioms_at_disjunction(problem->get_axioms(), repositories);

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();
    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
    const auto translated_objects = this->translate_level_0(problem->get_objects(), repositories);
    builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
    const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());
    const auto translated_initial_function_values = this->translate_level_0(problem->get_function_values(), repositories);
    builder.get_function_values().insert(builder.get_function_values().end(),
                                         translated_initial_function_values.begin(),
                                         translated_initial_function_values.end());
    if (problem->get_goal_condition().has_value())
        builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition().value(), repositories);
    if (problem->get_optimization_metric().has_value())
        builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric().value(), repositories);

    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result(problem->get_index());
}

}
