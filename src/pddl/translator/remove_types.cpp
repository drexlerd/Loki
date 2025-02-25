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

#include "remove_types.hpp"

#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

namespace loki
{

static void collect_types_from_type_hierarchy_recursively(Type type, TypeSet& ref_type_list)
{
    // Base case: Skip object types since they do not contribute to give any additional information
    if (type->get_name() != "object")
    {
        ref_type_list.insert(type);
    }
    // Inductive case:
    for (const auto& base_type : type->get_bases())
    {
        collect_types_from_type_hierarchy_recursively(base_type, ref_type_list);
    }
}

static TypeList collect_types_from_type_hierarchy(const TypeList& type_list)
{
    TypeSet flat_type_set;
    for (const auto& type : type_list)
    {
        collect_types_from_type_hierarchy_recursively(type, flat_type_set);
    }
    return TypeList(flat_type_set.begin(), flat_type_set.end());
}

static Predicate type_to_predicate(Type type, Repositories& repositories)
{
    auto predicate =
        repositories.get_or_create_predicate(type->get_name(),
                                             ParameterList { repositories.get_or_create_parameter(repositories.get_or_create_variable("?arg"), TypeList {}) });

    return predicate;
}

static Object typed_object_to_untyped_object(Object object, Repositories& repositories)
{
    return repositories.get_or_create_object(object->get_name(), TypeList {});
}

static LiteralList typed_object_to_literals(Object object, Repositories& repositories)
{
    auto additional_literals = LiteralList {};
    auto translated_term = repositories.get_or_create_term(typed_object_to_untyped_object(object, repositories));
    auto types = collect_types_from_type_hierarchy(object->get_bases());
    for (const auto& type : types)
    {
        auto additional_literal =
            repositories.get_or_create_literal(false, repositories.get_or_create_atom(type_to_predicate(type, repositories), TermList { translated_term }));
        additional_literals.push_back(additional_literal);
    }
    return additional_literals;
}

static ConditionList typed_parameter_to_condition_literals(Parameter parameter, Repositories& repositories)
{
    auto conditions = ConditionList {};
    auto types = collect_types_from_type_hierarchy(parameter->get_bases());
    for (const auto& type : types)
    {
        auto condition = repositories.get_or_create_condition(repositories.get_or_create_condition_literal(repositories.get_or_create_literal(
            false,
            repositories.get_or_create_atom(
                type_to_predicate(type, repositories),
                TermList { repositories.get_or_create_term(repositories.get_or_create_variable(parameter->get_variable()->get_name())) }))));
        conditions.push_back(condition);
    }
    return conditions;
}

Object RemoveTypesTranslator::translate_level_2(Object object, Repositories& repositories) { return typed_object_to_untyped_object(object, repositories); }

Condition RemoveTypesTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};
    for (const auto& parameter : condition->get_parameters())
    {
        translated_parameters.push_back(repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {}));
        auto additional_conditions = typed_parameter_to_condition_literals(parameter, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), additional_conditions.begin(), additional_conditions.end());
    }

    // Translate condition
    translated_part_conditions.push_back(this->translate_level_0(condition->get_condition(), repositories));
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    return repositories.get_or_create_condition(repositories.get_or_create_condition_exists(translated_parameters, translated_condition));
}

Condition RemoveTypesTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};
    for (const auto& parameter : condition->get_parameters())
    {
        translated_parameters.push_back(repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {}));
        auto additional_conditions = typed_parameter_to_condition_literals(parameter, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), additional_conditions.begin(), additional_conditions.end());
    }

    // Translate condition
    translated_part_conditions.push_back(this->translate_level_0(condition->get_condition(), repositories));
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    return repositories.get_or_create_condition(repositories.get_or_create_condition_forall(translated_parameters, translated_condition));
}

Effect RemoveTypesTranslator::translate_level_2(EffectCompositeForall effect, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};
    for (const auto& parameter : effect->get_parameters())
    {
        translated_parameters.push_back(repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {}));
        auto additional_conditions = typed_parameter_to_condition_literals(parameter, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), additional_conditions.begin(), additional_conditions.end());
    }

    // Instantiate a condition to use in "when"
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    // Translate effect
    auto translated_effect = this->translate_level_0(effect->get_effect(), repositories);

    return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(
        translated_parameters,
        repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(translated_condition, translated_effect))));
}

Axiom RemoveTypesTranslator::translate_level_2(Axiom axiom, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};
    for (const auto& parameter : axiom->get_parameters())
    {
        translated_parameters.push_back(repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {}));
        auto additional_conditions = typed_parameter_to_condition_literals(parameter, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), additional_conditions.begin(), additional_conditions.end());
    }
    translated_part_conditions.push_back(this->translate_level_0(axiom->get_condition(), repositories));

    // Instantiate the condition
    auto translated_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions));

    // Translate effect
    auto translated_literal = this->translate_level_0(axiom->get_literal(), repositories);

    return repositories.get_or_create_axiom(translated_parameters, translated_literal, translated_condition);
}

Action RemoveTypesTranslator::translate_level_2(Action action, Repositories& repositories)
{
    // Translate parameters
    auto translated_parameters = ParameterList {};
    auto translated_part_conditions = ConditionList {};
    for (const auto& parameter : action->get_parameters())
    {
        translated_parameters.push_back(repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories), TypeList {}));
        auto additional_conditions = typed_parameter_to_condition_literals(parameter, repositories);
        translated_part_conditions.insert(translated_part_conditions.end(), additional_conditions.begin(), additional_conditions.end());
    }
    if (action->get_condition().has_value())
    {
        translated_part_conditions.push_back(this->translate_level_0(action->get_condition().value(), repositories));
    }
    auto translated_condition =
        translated_part_conditions.empty() ?
            std::nullopt :
            std::optional<Condition>(repositories.get_or_create_condition(repositories.get_or_create_condition_and(translated_part_conditions)));
    auto translated_effect = this->translate_level_0(action->get_effect(), repositories);

    return repositories.get_or_create_action(action->get_name(), action->get_original_arity(), translated_parameters, translated_condition, translated_effect);
}

Domain RemoveTypesTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();

    // Remove :typing requirement
    auto requirements_enum_set = domain->get_requirements()->get_requirements();
    requirements_enum_set.erase(RequirementEnum::TYPING);
    builder.get_requirements() = repositories.get_or_create_requirements(requirements_enum_set);

    // Translate types into predicates.
    auto translated_types = collect_types_from_hierarchy(this->translate_level_0(domain->get_types(), repositories));
    for (const auto& type : translated_types)
    {
        builder.get_predicates().insert(builder.get_predicates().end(), type_to_predicate(type, repositories));
    }

    // Make constants untyped, introduce static initial literals.
    auto translated_constants = ObjectList {};
    translated_constants.reserve(domain->get_constants().size());
    for (const auto& object : domain->get_constants())
    {
        builder.get_constants().push_back(typed_object_to_untyped_object(object, repositories));
        auto additional_literals = typed_object_to_literals(object, repositories);
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

Problem RemoveTypesTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto& repositories = builder.get_repositories();

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();

    // Remove :typing requirement
    auto requirements_enum_set = problem->get_requirements()->get_requirements();
    requirements_enum_set.erase(RequirementEnum::TYPING);
    builder.get_requirements() = repositories.get_or_create_requirements(requirements_enum_set);

    // Make objects untyped
    for (const auto& object : problem->get_objects())
    {
        builder.get_objects().push_back(typed_object_to_untyped_object(object, repositories));
        auto additional_literals = typed_object_to_literals(object, repositories);
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

}
