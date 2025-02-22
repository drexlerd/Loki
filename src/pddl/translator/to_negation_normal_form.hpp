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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{
class ToNegationNormalFormTranslator : public RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>
{
private:
    friend class RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>::prepare_level_2;
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>::translate_level_2;

    /**
     * Translate
     */

    /**
     * Eliminate implication
     *
     * 1. A -> B   =>  not A or B
     * 2. A <-> B  =>  (not A or B) and (A or not B)
     */

    // Condition translate_level_2(ConditionImply condition);
    /**
     * Push negation inwards.
     *
     * 3. not (A or B)    =>  not A and not B
     * 4. not (A and B)   =>  not A or not B
     * 5. not (not A)     =>  A
     * 6. not exists x A  =>  forall x not A
     * 7. not forall x A  =>  exists x not A
     * 8. not (A -> B)    =>  not (not A or B)  =>  A and not B
     */

    // Condition translate_level_2(ConditionNot condition);
    /**
     * Flatten conjunctions.
     *
     * 9. A and (B and C)  =>  A and B and C
     */

    // Condition translate_level_2(ConditionAnd condition);
    /**
     * Flatten disjunctions.
     *
     * 10. A or (B or C)  =>  A or B or C
     */

    // Condition translate_level_2(ConditionOr condition);
    /**
     * Flatten existential quantifiers.
     *
     * 11. exists(vars1, exists(vars2, A))  =>  exists(vars1+vars2, A)
     */

    // Condition translate_level_2(ConditionExists condition);
    /**
     * Flatten universal quantifiers.
     *
     * 12. forall(vars1, forall(vars2, A))  =>  forall(vars1+vars2, A)
     */

    // Condition translate_level_2(ConditionForall condition);
};

/**
 * Implementations
 */

/*
template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_level_2(ConditionImply condition)
{
    return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_or(ConditionList {
        this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_not(condition->get_condition_left())),
        condition->get_condition_right() })));
}

template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_impl(ConditionNot condition)
{
    auto translated_nested_condition = this->translate(condition->get_condition());

    if (const auto condition_lit = std::get_if<ConditionLiteral>(&translated_nested_condition->get_condition()))
    {
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_literal(
            this->m_pddl_repositories.get_or_create_literal(!(*condition_lit)->get_literal()->is_negated(), (*condition_lit)->get_literal()->get_atom()))));
    }
    else if (const auto condition_numeric = std::get_if<ConditionNumericConstraint>(&translated_nested_condition->get_condition()))
    {
        return this->translate(this->m_pddl_repositories.get_or_create_condition(
            this->m_pddl_repositories.get_or_create_condition_numeric_constraint((*condition_numeric)->get_binary_comparator(),
                                                                                 (*condition_numeric)->get_function_expression_left(),
                                                                                 (*condition_numeric)->get_function_expression_right())));
    }
    else if (const auto condition_not = std::get_if<ConditionNot>(&translated_nested_condition->get_condition()))
    {
        return this->translate((*condition_not)->get_condition());
    }
    else if (const auto condition_imply = std::get_if<ConditionImply>(&translated_nested_condition->get_condition()))
    {
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_and(
            ConditionList { (*condition_imply)->get_condition_left(),
                            this->m_pddl_repositories.get_or_create_condition(
                                this->m_pddl_repositories.get_or_create_condition_not((*condition_imply)->get_condition_right())) })));
    }
    else if (const auto condition_and = std::get_if<ConditionAnd>(&translated_nested_condition->get_condition()))
    {
        auto nested_parts = ConditionList {};
        nested_parts.reserve((*condition_and)->get_conditions().size());
        for (const auto& nested_condition : (*condition_and)->get_conditions())
        {
            nested_parts.push_back(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_not(nested_condition)));
        }
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_or(nested_parts)));
    }
    else if (const auto condition_or = std::get_if<ConditionOr>(&translated_nested_condition->get_condition()))
    {
        auto nested_parts = ConditionList {};
        nested_parts.reserve((*condition_or)->get_conditions().size());
        for (const auto& nested_condition : (*condition_or)->get_conditions())
        {
            nested_parts.push_back(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_not(nested_condition)));
        }
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_and(nested_parts)));
    }
    else if (const auto condition_exists = std::get_if<ConditionExists>(&translated_nested_condition->get_condition()))
    {
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_forall(
            (*condition_exists)->get_parameters(),
            this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_not((*condition_exists)->get_condition())))));
    }
    else if (const auto condition_forall = std::get_if<ConditionForall>(&translated_nested_condition->get_condition()))
    {
        return this->translate(this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_exists(
            (*condition_forall)->get_parameters(),
            this->m_pddl_repositories.get_or_create_condition(this->m_pddl_repositories.get_or_create_condition_not((*condition_forall)->get_condition())))));
    }
    throw std::runtime_error("Missing implementation to push negations inwards.");
}

template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_impl(ConditionAnd condition)
{
    return flatten(this->m_pddl_repositories.get_or_create_condition_and(this->translate(condition->get_conditions())), this->m_pddl_repositories);
}

template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_impl(ConditionOr condition)
{
    return flatten(this->m_pddl_repositories.get_or_create_condition_or(this->translate(condition->get_conditions())), this->m_pddl_repositories);
}

template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_impl(ConditionExists condition)
{
    return flatten(
        this->m_pddl_repositories.get_or_create_condition_exists(this->translate(condition->get_parameters()), this->translate(condition->get_condition())),
        this->m_pddl_repositories);
}

template<typename Builder>
Condition ToNegationNormalFormTranslator::translate_impl(ConditionForall condition)
{
    return flatten(
        this->m_pddl_repositories.get_or_create_condition_forall(this->translate(condition->get_parameters()), this->translate(condition->get_condition())),
        this->m_pddl_repositories);
}
*/
}

#endif
