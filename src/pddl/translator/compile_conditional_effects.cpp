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

#include "compile_conditional_effects.hpp"

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

namespace loki
{

ActionList CompileConditionalEffectsTranslator::compile_conditional_effects(const Action action, Repositories& repositories)
{
    if (!action->get_effect())
        return ActionList { action };

    auto result_actions = ActionList {};

    auto result_parts = ConditionList {};
    if (action->get_condition())
        result_parts.push_back(*action->get_condition());

    const auto effect = *action->get_effect();

    if (const auto and_effect = std::get_if<EffectAnd>(&effect->get_effect()))
    {
        auto result_parts = ConditionList {};
        if (action->get_condition())
            result_parts.push_back(*action->get_condition());

        for (const auto part : (*and_effect)->get_effects())
        {
            auto tmp_effect = part;
            auto parameters = action->get_parameters();

            if (const auto forall_effect = std::get_if<EffectCompositeForall>(&tmp_effect->get_effect()))
            {
                const auto add_parameters = (*forall_effect)->get_parameters();
                parameters.insert(parameters.end(), add_parameters.begin(), add_parameters.end());
                tmp_effect = (*forall_effect)->get_effect();
            }

            if (const auto when_effect = std::get_if<EffectCompositeWhen>(&tmp_effect->get_effect()))
            {
                const auto cond_precondition = (*when_effect)->get_condition();
                const auto cond_effect = (*when_effect)->get_effect();

                result_parts.push_back(cond_precondition);

                result_actions.push_back(repositories.get_or_create_action(
                    action->get_name(),
                    action->get_original_arity(),
                    this->translate_level_0(parameters, repositories),
                    this->translate_level_0(flatten(repositories.get_or_create_condition_and(result_parts), repositories), repositories),
                    this->translate_level_0(cond_effect, repositories)));
            }
            else
            {
                result_actions.push_back(repositories.get_or_create_action(action->get_name(),
                                                                           action->get_original_arity(),
                                                                           this->translate_level_0(parameters, repositories),
                                                                           this->translate_level_0(action->get_condition(), repositories),
                                                                           this->translate_level_0(part, repositories)));
            }
        }
    }

    return result_actions;
}

ActionList CompileConditionalEffectsTranslator::compile_conditional_effects(const ActionList& actions, Repositories& repositories)
{
    auto action_set = ActionSet {};
    for (const auto& action : actions)
    {
        auto additional_actions = compile_conditional_effects(action, repositories);
        action_set.insert(additional_actions.begin(), additional_actions.end());
    }
    return ActionList(action_set.begin(), action_set.end());
}

Domain CompileConditionalEffectsTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
    builder.get_types() = this->translate_level_0(domain->get_types(), repositories);
    builder.get_constants() = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_static_initial_literals() = this->translate_level_0(domain->get_static_initial_literals(), repositories);
    builder.get_predicates() = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_function_skeletons() = this->translate_level_0(domain->get_function_skeletons(), repositories);
    builder.get_actions() = compile_conditional_effects(domain->get_actions(), repositories);
    builder.get_axioms() = this->translate_level_0(domain->get_axioms(), repositories);

    return builder.get_result();
}

}
