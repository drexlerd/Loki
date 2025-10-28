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

#include "add_type_predicates.hpp"

#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

namespace loki
{

static Predicate create_type_predicate(Type translated_type, Repositories& repositories)
{
    return repositories.get_or_create_predicate(
        translated_type->get_name(),
        ParameterList { repositories.get_or_create_parameter(repositories.get_or_create_variable("?arg"), TypeList { translated_type }) });
}

static LiteralList create_literals_for_object_types(Object translated_object, const TypeList& translated_types, Repositories& repositories)
{
    auto additional_literals = LiteralList {};
    auto translated_term = repositories.get_or_create_term(translated_object);
    auto translated_types_hierarchy = collect_types_from_hierarchy(translated_types);

    for (const auto& type : translated_types_hierarchy)
    {
        auto additional_literal =
            repositories.get_or_create_literal(true, repositories.get_or_create_atom(create_type_predicate(type, repositories), TermList { translated_term }));
        additional_literals.push_back(additional_literal);
    }
    return additional_literals;
}

static ConditionList create_condition_literals_for_parameter_types(Parameter translated_parameter, const TypeList& translated_types, Repositories& repositories)
{
    auto conditions = ConditionList {};
    auto translated_type_hierarchy = collect_types_from_hierarchy(translated_types);

    for (const auto& type : translated_type_hierarchy)
    {
        auto condition = repositories.get_or_create_condition(repositories.get_or_create_condition_literal(repositories.get_or_create_literal(
            true,
            repositories.get_or_create_atom(
                create_type_predicate(type, repositories),
                TermList { repositories.get_or_create_term(repositories.get_or_create_variable(translated_parameter->get_variable()->get_name())) }))));
        conditions.push_back(condition);
    }
    return conditions;
}

Requirements AddTypePredicatesTranslator::translate_level_2(Requirements requirements, Repositories& repositories)
{
    auto requirements_set = requirements->get_requirements();
    if (m_remove_typing)
        requirements_set.erase(loki::RequirementEnum::TYPING);
    return repositories.get_or_create_requirements(requirements_set);
}

Object AddTypePredicatesTranslator::translate_level_2(Object object, Repositories& repositories)
{
    return repositories.get_or_create_object(object->get_name(), m_remove_typing ? TypeList {} : this->translate_level_0(object->get_bases(), repositories));
}

Parameter AddTypePredicatesTranslator::translate_level_2(Parameter parameter, Repositories& repositories)
{
    return repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories),
                                                m_remove_typing ? TypeList {} : this->translate_level_0(parameter->get_bases(), repositories));
}

Condition AddTypePredicatesTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};

    for (const auto& parameter : condition->get_parameters())
    {
        auto translated_parameter = this->translate_level_0(parameter, repositories);
        translated_parameters.push_back(translated_parameter);
        auto translated_types = this->translate_level_0(parameter->get_bases(), repositories);
        auto translated_condition_literals = create_condition_literals_for_parameter_types(translated_parameter, translated_types, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), translated_condition_literals.begin(), translated_condition_literals.end());
    }

    // Translate condition
    translated_part_conditions.push_back(this->translate_level_0(condition->get_condition(), repositories));
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(translated_parameters, translated_condition));
}

Condition AddTypePredicatesTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};

    for (const auto& parameter : condition->get_parameters())
    {
        auto translated_parameter = this->translate_level_0(parameter, repositories);
        translated_parameters.push_back(translated_parameter);
        auto translated_types = this->translate_level_0(parameter->get_bases(), repositories);
        auto translated_condition_literals = create_condition_literals_for_parameter_types(translated_parameter, translated_types, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), translated_condition_literals.begin(), translated_condition_literals.end());
    }

    // Translate condition
    translated_part_conditions.push_back(this->translate_level_0(condition->get_condition(), repositories));
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(translated_parameters, translated_condition));
}

Effect AddTypePredicatesTranslator::translate_level_2(EffectCompositeForall effect, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};

    for (const auto& parameter : effect->get_parameters())
    {
        auto translated_parameter = this->translate_level_0(parameter, repositories);
        translated_parameters.push_back(translated_parameter);
        auto translated_types = this->translate_level_0(parameter->get_bases(), repositories);
        auto translated_condition_literals = create_condition_literals_for_parameter_types(translated_parameter, translated_types, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), translated_condition_literals.begin(), translated_condition_literals.end());
    }

    // Instantiate a condition to use in "when"
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    // Translate effect
    auto translated_effect = this->translate_level_0(effect->get_effect(), repositories);

    return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(
        translated_parameters,
        repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(translated_condition, translated_effect))));
}

Axiom AddTypePredicatesTranslator::translate_level_2(Axiom axiom, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};

    for (const auto& parameter : axiom->get_parameters())
    {
        auto translated_parameter = this->translate_level_0(parameter, repositories);
        translated_parameters.push_back(translated_parameter);
        auto translated_types = this->translate_level_0(parameter->get_bases(), repositories);
        auto translated_condition_literals = create_condition_literals_for_parameter_types(translated_parameter, translated_types, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), translated_condition_literals.begin(), translated_condition_literals.end());
    }

    translated_part_conditions.push_back(this->translate_level_0(axiom->get_condition(), repositories));

    // Instantiate the condition
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    // Translate effect
    auto translated_literal = this->translate_level_0(axiom->get_literal(), repositories);

    return repositories.get_or_create_axiom(translated_parameters, translated_literal, translated_condition);
}

Action AddTypePredicatesTranslator::translate_level_2(Action action, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};

    for (const auto& parameter : action->get_parameters())
    {
        auto translated_parameter = this->translate_level_0(parameter, repositories);
        translated_parameters.push_back(translated_parameter);
        auto translated_types = this->translate_level_0(parameter->get_bases(), repositories);
        auto translated_condition_literals = create_condition_literals_for_parameter_types(translated_parameter, translated_types, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), translated_condition_literals.begin(), translated_condition_literals.end());
    }

    if (action->get_condition().has_value())
        translated_part_conditions.push_back(this->translate_level_0(action->get_condition().value(), repositories));

    auto translated_condition =
        translated_part_conditions.empty() ?
            std::nullopt :
            std::optional<Condition>(repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions)));
    auto translated_effect = this->translate_level_0(action->get_effect(), repositories);

    return repositories.get_or_create_action(action->get_name(), action->get_original_arity(), translated_parameters, translated_condition, translated_effect);
}

Domain AddTypePredicatesTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();

    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);

    // Introduce static type predicates.
    for (const auto& type : collect_types_from_hierarchy(this->translate_level_0(domain->get_types(), repositories)))
    {
        builder.get_predicates().insert(builder.get_predicates().end(), create_type_predicate(type, repositories));
    }

    if (!m_remove_typing)
        builder.get_types() = this->translate_level_0(domain->get_types(), repositories);

    // Introduce static initial literals for constant types.
    for (const auto& object : domain->get_constants())
    {
        auto translated_object = this->translate_level_0(object, repositories);
        auto translated_types = this->translate_level_0(object->get_bases(), repositories);
        builder.get_constants().push_back(translated_object);
        auto additional_literals = create_literals_for_object_types(translated_object, translated_types, repositories);
        builder.get_static_initial_literals().insert(builder.get_static_initial_literals().end(), additional_literals.begin(), additional_literals.end());
    }

    // Translate predicates
    auto translated_predicates = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());

    // Translate function skeletons
    const auto translated_function_skeletons = this->translate_level_0(domain->get_function_skeletons(), repositories);
    builder.get_function_skeletons().insert(builder.get_function_skeletons().end(), translated_function_skeletons.begin(), translated_function_skeletons.end());

    // Translate actions
    const auto translated_actions = this->translate_level_0(domain->get_actions(), repositories);
    builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());

    // Translated axioms
    const auto translated_axioms = this->translate_level_0(domain->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result();
}

Problem AddTypePredicatesTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();

    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);

    // Introduce static initial literals for object types
    for (const auto& object : problem->get_objects())
    {
        auto translated_object = this->translate_level_0(object, repositories);
        auto translated_types = this->translate_level_0(object->get_bases(), repositories);
        builder.get_objects().push_back(translated_object);
        auto additional_literals = create_literals_for_object_types(translated_object, translated_types, repositories);
        builder.get_initial_literals().insert(builder.get_initial_literals().end(), additional_literals.begin(), additional_literals.end());
    }
    // Make constants untyped
    auto translated_domain_static_initial_literals = this->translate_level_0(problem->get_domain()->get_static_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(),
                                          translated_domain_static_initial_literals.begin(),
                                          translated_domain_static_initial_literals.end());

    const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());

    // Translate other initial literals and add additional literals
    const auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());

    builder.get_initial_function_values() = this->translate_level_0(problem->get_initial_function_values(), repositories);

    builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition(), repositories);

    builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric(), repositories);

    builder.get_axioms() = this->translate_level_0(problem->get_axioms(), repositories);

    // Types are define in the domain and copied into ProblemBuilder during its contruction

    return builder.get_result(problem->get_index());
}

AddTypePredicatesTranslator::AddTypePredicatesTranslator(bool remove_typing) : m_remove_typing(remove_typing) {}

}
