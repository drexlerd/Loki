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

#include "rename_quantified_variables.hpp"

using namespace std::string_literals;

namespace loki
{

static void increment_num_quantifications(const ParameterList& parameters, std::unordered_map<Variable, size_t>& ref_num_quantifications)
{
    for (const auto& parameter : parameters)
    {
        const auto variable = parameter->get_variable();

        ref_num_quantifications.contains(variable) ? ++ref_num_quantifications.at(variable) : ref_num_quantifications[variable] = 0;
    }
}

RenameQuantifiedVariablesTranslator::RenameQuantifiedVariablesTranslator() : m_renaming_enabled(true) {}

Variable RenameQuantifiedVariablesTranslator::translate_level_2(Variable variable, Repositories& repositories)
{
    if (m_renaming_enabled)
    {
        /* If variable is not quantified, then we have a bug somewhere. */
        assert(m_num_quantifications.contains(variable));

        return repositories.get_or_create_variable(variable->get_name() + "_" + std::to_string(variable->get_index()) + "_"
                                                   + std::to_string(m_num_quantifications.at(variable)));
    }

    return repositories.get_or_create_variable(variable->get_name());
}

Predicate RenameQuantifiedVariablesTranslator::translate_level_2(Predicate predicate, Repositories& repositories)
{
    // Disallow renaming of nested variables
    m_renaming_enabled = false;

    auto result = repositories.get_or_create_predicate(predicate->get_name(), this->translate_level_0(predicate->get_parameters(), repositories));

    // Allow renaming again
    m_renaming_enabled = true;

    return result;
}

FunctionSkeleton RenameQuantifiedVariablesTranslator::translate_level_2(FunctionSkeleton function_skeleton, Repositories& repositories)
{
    // Disallow renaming of nested variables
    m_renaming_enabled = false;

    auto result = repositories.get_or_create_function_skeleton(function_skeleton->get_name(),
                                                               this->translate_level_0(function_skeleton->get_parameters(), repositories),
                                                               this->translate_level_0(function_skeleton->get_type(), repositories));

    // Allow renaming again
    m_renaming_enabled = true;

    return result;
}

Action RenameQuantifiedVariablesTranslator::translate_level_2(Action action, Repositories& repositories)
{
    // Clear quantifications as we enter a new top-level scope.
    m_num_quantifications.clear();
    increment_num_quantifications(action->get_parameters(), m_num_quantifications);

    const auto translated_parameters = this->translate_level_0(action->get_parameters(), repositories);
    const auto translated_conditions = this->translate_level_0(action->get_condition(), repositories);
    const auto translated_effect = this->translate_level_0(action->get_effect(), repositories);

    auto translated_action =
        repositories.get_or_create_action(action->get_name(), action->get_original_arity(), translated_parameters, translated_conditions, translated_effect);

    return translated_action;
}

Axiom RenameQuantifiedVariablesTranslator::translate_level_2(Axiom axiom, Repositories& repositories)
{
    // Clear quantifications as we enter a new top-level scope.
    m_num_quantifications.clear();
    increment_num_quantifications(axiom->get_parameters(), m_num_quantifications);

    const auto translated_parameters = this->translate_level_0(axiom->get_parameters(), repositories);
    const auto translated_literal = this->translate_level_0(axiom->get_literal(), repositories);
    const auto translated_conditions = this->translate_level_0(axiom->get_condition(), repositories);

    auto translated_axiom = repositories.get_or_create_axiom(translated_parameters, translated_literal, translated_conditions);

    return translated_axiom;
}

Condition RenameQuantifiedVariablesTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    increment_num_quantifications(condition->get_parameters(), m_num_quantifications);

    const auto translated_parameters = this->translate_level_0(condition->get_parameters(), repositories);
    const auto translated_nested_condition = this->translate_level_0(condition->get_condition(), repositories);

    auto translated_condition =
        repositories.get_or_create_condition(repositories.get_or_create_condition_exists(translated_parameters, translated_nested_condition));

    return translated_condition;
}

Condition RenameQuantifiedVariablesTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    increment_num_quantifications(condition->get_parameters(), m_num_quantifications);

    const auto translated_parameters = this->translate_level_0(condition->get_parameters(), repositories);
    const auto translated_nested_condition = this->translate_level_0(condition->get_condition(), repositories);

    auto translated_condition =
        repositories.get_or_create_condition(repositories.get_or_create_condition_forall(translated_parameters, translated_nested_condition));

    return translated_condition;
}

Effect RenameQuantifiedVariablesTranslator::translate_level_2(EffectCompositeForall effect, Repositories& repositories)
{
    increment_num_quantifications(effect->get_parameters(), m_num_quantifications);

    const auto translated_parameters = this->translate_level_0(effect->get_parameters(), repositories);
    const auto translated_nested_effect = this->translate_level_0(effect->get_effect(), repositories);

    auto translated_effect =
        repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(translated_parameters, translated_nested_effect));

    return translated_effect;
}

}
