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
                                       repositories.get_or_create_condition(repositories.get_or_create_condition_not(condition->get_left_condition())),
                                       condition->get_right_condition() })),
                                   repositories);
}

Condition ToNegationNormalFormTranslator::translate_level_2(ConditionNot condition, Repositories& repositories)
{
    const auto translated_nested = this->translate_level_0(condition->get_condition(), repositories);

    return std::visit(
        [&](auto&& arg) -> Condition
        {
            using V = std::decay_t<decltype(arg)>;

            if constexpr (std::is_same_v<V, ConditionLiteral>)
            {
                // not p  =>  (flip polarity)
                const auto lit = arg->get_literal();
                return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_literal(
                                                   repositories.get_or_create_literal(!lit->get_polarity(), lit->get_atom()))),
                                               repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionNumericConstraint>)
            {
                // not (x ~ y)  =>  (negate comparator)
                return this->translate_level_0(
                    repositories.get_or_create_condition(repositories.get_or_create_condition_numeric_constraint(negate(arg->get_binary_comparator()),
                                                                                                                 arg->get_left_function_expression(),
                                                                                                                 arg->get_right_function_expression())),
                    repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionNot>)
            {
                // not (not phi) => phi
                return this->translate_level_0(arg->get_condition(), repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionImply>)
            {
                // not (a -> b)  =>  a and not b
                return this->translate_level_0(
                    repositories.get_or_create_condition(repositories.get_or_create_condition_and(
                        ConditionList { arg->get_left_condition(),
                                        repositories.get_or_create_condition(repositories.get_or_create_condition_not(arg->get_right_condition())) })),
                    repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionAnd>)
            {
                // not (and_i phi_i) => or_i not phi_i
                ConditionList parts;
                const auto& cs = arg->get_conditions();
                parts.reserve(cs.size());
                for (const auto& c : cs)
                    parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_not(c)));

                return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_or(parts)), repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionOr>)
            {
                // not (or_i phi_i) => and_i not phi_i
                ConditionList parts;
                const auto& cs = arg->get_conditions();
                parts.reserve(cs.size());
                for (const auto& c : cs)
                    parts.push_back(repositories.get_or_create_condition(repositories.get_or_create_condition_not(c)));

                return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_and(parts)), repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionExists>)
            {
                // not (exists x. phi) => forall x. not phi
                return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_forall(
                                                   arg->get_parameters(),
                                                   repositories.get_or_create_condition(repositories.get_or_create_condition_not(arg->get_condition())))),
                                               repositories);
            }
            else if constexpr (std::is_same_v<V, ConditionForall>)
            {
                // not (forall x. phi) => exists x. not phi
                return this->translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_exists(
                                                   arg->get_parameters(),
                                                   repositories.get_or_create_condition(repositories.get_or_create_condition_not(arg->get_condition())))),
                                               repositories);
            }
            else
            {
                throw std::runtime_error("Missing implementation to push negations inwards.");
            }
        },
        translated_nested->get_condition());
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
