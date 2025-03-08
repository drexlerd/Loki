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

#include "to_negation_normal_form.hpp"

#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

namespace loki
{

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionImply condition, Repositories& repositories)
{
    return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(ConditionList {
                                       repositories.get_or_create_condition(repositories.get_or_create_condition_not(condition->get_condition_left())),
                                       condition->get_condition_right() })),
                                   repositories);
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionNot condition, Repositories& repositories)
{
    auto translated_nested_condition = this->translate_level_0(condition->get_condition(), repositories);

    if (const auto condition_lit = std::get_if<ConditionLiteral>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0(
            repositories.get_or_create_condition(repositories.get_or_create_condition_literal(
                repositories.get_or_create_literal(!(*condition_lit)->get_literal()->is_negated(), (*condition_lit)->get_literal()->get_atom()))),
            repositories);
    }
    else if (const auto condition_numeric = std::get_if<ConditionNumericConstraint>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0(repositories.get_or_create_condition(
                                           repositories.get_or_create_condition_numeric_constraint((*condition_numeric)->get_binary_comparator(),
                                                                                                   (*condition_numeric)->get_left_function_expression(),
                                                                                                   (*condition_numeric)->get_right_function_expression())),
                                       repositories);
    }
    else if (const auto condition_not = std::get_if<ConditionNot>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0((*condition_not)->get_condition(), repositories);
    }
    else if (const auto condition_imply = std::get_if<ConditionImply>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0(
            repositories.get_or_create_condition(repositories.get_or_create_condition_and(
                ConditionList { (*condition_imply)->get_condition_left(),
                                repositories.get_or_create_condition(repositories.get_or_create_condition_not((*condition_imply)->get_condition_right())) })),
            repositories);
    }
    else if (const auto condition_and = std::get_if<ConditionAnd>(&translated_nested_condition->get_condition()))
    {
        auto nested_parts = ConditionList {};
        nested_parts.reserve((*condition_and)->get_conditions().size());
        for (const auto& nested_condition : (*condition_and)->get_conditions())
        {
            nested_parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_not(nested_condition)));
        }
        return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(nested_parts)), repositories);
    }
    else if (const auto condition_or = std::get_if<ConditionOr>(&translated_nested_condition->get_condition()))
    {
        auto nested_parts = ConditionList {};
        nested_parts.reserve((*condition_or)->get_conditions().size());
        for (const auto& nested_condition : (*condition_or)->get_conditions())
        {
            nested_parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_not(nested_condition)));
        }
        return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_and(nested_parts)), repositories);
    }
    else if (const auto condition_exists = std::get_if<ConditionExists>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0(
            repositories.get_or_create_condition(repositories.get_or_create_condition_forall(
                (*condition_exists)->get_parameters(),
                repositories.get_or_create_condition(repositories.get_or_create_condition_not((*condition_exists)->get_condition())))),
            repositories);
    }
    else if (const auto condition_forall = std::get_if<ConditionForall>(&translated_nested_condition->get_condition()))
    {
        return this->translate_level_0(
            repositories.get_or_create_condition(repositories.get_or_create_condition_exists(
                (*condition_forall)->get_parameters(),
                repositories.get_or_create_condition(repositories.get_or_create_condition_not((*condition_forall)->get_condition())))),
            repositories);
    }
    throw std::runtime_error("Missing implementation to push negations inwards.");
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionAnd condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_and(this->translate_level_0(condition->get_conditions(), repositories)), repositories);
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionOr condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_or(this->translate_level_0(condition->get_conditions(), repositories)), repositories);
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_exists(this->translate_level_0(condition->get_parameters(), repositories),
                                                               this->translate_level_0(condition->get_condition(), repositories)),
                   repositories);
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    return flatten(repositories.get_or_create_condition_forall(this->translate_level_0(condition->get_parameters(), repositories),
                                                               this->translate_level_0(condition->get_condition(), repositories)),
                   repositories);
}

}
