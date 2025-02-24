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

#include "move_existential_quantifiers.hpp"

#include "utils.hpp"

#include <algorithm>

namespace loki
{

Condition MoveExistentialQuantifiersTranslator::translate_level_2(ConditionAnd condition, Repositories& repositories)
{
    const auto translated_nested_conditions = this->translate_level_0(condition->get_conditions(), repositories);

    auto translated_parameters = std::unordered_set<Parameter> {};
    auto translated_parts = ConditionList {};
    for (const auto translated_nested_condition : translated_nested_conditions)
    {
        if (const auto translated_nested_condition_exists = std::get_if<ConditionExists>(&translated_nested_condition->get_condition()))
        {
            translated_parameters.insert((*translated_nested_condition_exists)->get_parameters().begin(),
                                         (*translated_nested_condition_exists)->get_parameters().end());

            translated_parts.push_back((*translated_nested_condition_exists)->get_condition());
        }
        else
        {
            translated_parts.push_back(translated_nested_condition);
        }
    }

    const auto translated_parts_conjunction = flatten(repositories.get_or_create_condition_and(translated_parts), repositories);

    if (translated_parameters.empty())
    {
        return translated_parts_conjunction;
    }

    return this->translate_level_0(repositories.get_or_create_condition(
                                       repositories.get_or_create_condition_exists(ParameterList { translated_parameters.begin(), translated_parameters.end() },
                                                                                   translated_parts_conjunction)),
                                   repositories);
}

Condition MoveExistentialQuantifiersTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_exists(this->translate_level_0(condition->get_parameters(), repositories),
                                                               this->translate_level_0(condition->get_condition(), repositories)),
                   repositories);
}

Action MoveExistentialQuantifiersTranslator::translate_level_2(Action action, Repositories& repositories)
{
    auto translated_parameters = this->translate_level_0(action->get_parameters(), repositories);
    auto translated_condition = this->translate_level_0(action->get_condition(), repositories);

    if (translated_condition.has_value())
    {
        if (const auto translated_condition_exists = std::get_if<ConditionExists>(&translated_condition.value()->get_condition()))
        {
            for (const auto& translated_parameter : (*translated_condition_exists)->get_parameters())
            {
                if (std::find(std::begin(translated_parameters), std::end(translated_parameters), translated_parameter) == translated_parameters.end())
                {
                    translated_parameters.push_back(translated_parameter);
                }
            }

            translated_condition = (*translated_condition_exists)->get_condition();
        }
    }

    return repositories.get_or_create_action(action->get_name(),
                                             action->get_original_arity(),
                                             translated_parameters,
                                             translated_condition,
                                             this->translate_level_0(action->get_effect(), repositories));
}

Axiom MoveExistentialQuantifiersTranslator::translate_level_2(Axiom axiom, Repositories& repositories)
{
    auto translated_parameters = this->translate_level_0(axiom->get_parameters(), repositories);
    auto translated_literal = this->translate_level_0(axiom->get_literal(), repositories);
    auto translated_condition = this->translate_level_0(axiom->get_condition(), repositories);

    if (const auto translated_condition_exists = std::get_if<ConditionExists>(&translated_condition->get_condition()))
    {
        for (const auto& translated_parameter : (*translated_condition_exists)->get_parameters())
        {
            if (std::find(std::begin(translated_parameters), std::end(translated_parameters), translated_parameter) == translated_parameters.end())
            {
                translated_parameters.push_back(translated_parameter);
            }
        }

        translated_condition = (*translated_condition_exists)->get_condition();
    }

    return repositories.get_or_create_axiom(translated_parameters, translated_literal, translated_condition);
}

}