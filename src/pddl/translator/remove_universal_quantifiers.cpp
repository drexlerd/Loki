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

#include "remove_universal_quantifiers.hpp"

#include "loki/details/utils/collections.hpp"
#include "to_negation_normal_form.hpp"
#include "utils.hpp"

using namespace std::string_literals;

namespace loki
{
RemoveUniversalQuantifiersTranslator::Scope::Scope(std::unordered_map<Variable, Parameter> variable_to_parameter, const Scope* parent_scope) :
    m_variable_to_parameter(std::move(variable_to_parameter)),
    m_parent_scope(parent_scope)
{
}

std::optional<Parameter> RemoveUniversalQuantifiersTranslator::Scope::get_parameter(Variable variable) const
{
    auto it = m_variable_to_parameter.find(variable);
    if (it != m_variable_to_parameter.end())
    {
        return it->second;
    }
    if (m_parent_scope)
    {
        return m_parent_scope->get_parameter(variable);
    }
    return std::nullopt;
}

const RemoveUniversalQuantifiersTranslator::Scope& RemoveUniversalQuantifiersTranslator::ScopeStack::open_scope(const ParameterList& parameters)
{
    auto variable_to_parameter = std::unordered_map<Variable, Parameter> {};
    for (const auto& parameter : parameters)
    {
        variable_to_parameter.emplace(parameter->get_variable(), parameter);
    }
    m_stack.empty() ? m_stack.push_back(std::make_unique<Scope>(std::move(variable_to_parameter))) :
                      m_stack.push_back(std::make_unique<Scope>(std::move(variable_to_parameter), &get()));
    return get();
}

void RemoveUniversalQuantifiersTranslator::ScopeStack::close_scope()
{
    assert(!m_stack.empty());
    m_stack.pop_back();
}

const RemoveUniversalQuantifiersTranslator::Scope& RemoveUniversalQuantifiersTranslator::ScopeStack::get() const { return *m_stack.back(); }

Condition RemoveUniversalQuantifiersTranslator::translate_level_2(ConditionExists condition, Repositories& repositories)
{
    const auto translated_condition_parameters = this->translate_level_0(condition->get_parameters(), repositories);

    m_scopes.open_scope(translated_condition_parameters);

    const auto translated_condition = translate_level_0(condition->get_condition(), repositories);
    auto result = repositories.get_or_create_condition(repositories.get_or_create_condition_exists(translated_condition_parameters, translated_condition));

    m_scopes.close_scope();

    return result;
}

Condition RemoveUniversalQuantifiersTranslator::translate_level_2(ConditionForall condition, Repositories& repositories)
{
    const auto translated_condition_parameters = this->translate_level_0(condition->get_parameters(), repositories);

    const auto& scope = m_scopes.open_scope(translated_condition_parameters);

    /* Free(exists(vars, phi)) become parameters. We obtain their types from the parameters in the parent scope. */
    auto translated_head_parameters = ParameterList {};
    auto translated_terms = TermList {};

    /* Collect free variables and translate them. */
    const auto free_variables_set = collect_free_variables(
        *repositories.get_or_create_condition(repositories.get_or_create_condition_forall(condition->get_parameters(), condition->get_condition())));
    const auto free_variables_vec = VariableList { free_variables_set.begin(), free_variables_set.end() };
    const auto translated_free_variables = this->translate_level_0(free_variables_vec, repositories);

    /* Instantiate parameters. */
    for (const auto translated_free_variable : translated_free_variables)
    {
        const auto translated_optional_parameter = scope.get_parameter(translated_free_variable);
        assert(translated_optional_parameter.has_value());  // We require all variables in axioms or actions to be bound to some quantifier

        const auto translated_bases = translated_optional_parameter.value()->get_bases();

        const auto parameter = repositories.get_or_create_parameter(translated_free_variable, translated_bases);
        translated_head_parameters.push_back(parameter);
        translated_terms.push_back(repositories.get_or_create_term(translated_free_variable));
    }

    /* Important: all other parameters are appended to the axiom parameters. */
    auto translated_axiom_parameters = translated_head_parameters;
    translated_axiom_parameters.insert(translated_axiom_parameters.end(), translated_condition_parameters.begin(), translated_condition_parameters.end());
    translated_head_parameters.shrink_to_fit();
    translated_axiom_parameters.shrink_to_fit();

    const auto translated_condition = this->translate_level_0(condition->get_condition(), repositories);

    auto to_nnf_translator = ToNegationNormalFormTranslator();

    const auto axiom_condition = translate_level_0(
        repositories.get_or_create_condition(repositories.get_or_create_condition_exists(
            translated_axiom_parameters,
            to_nnf_translator.translate_level_0(repositories.get_or_create_condition(repositories.get_or_create_condition_not(translated_condition)),
                                                repositories))),
        repositories);

    const auto axiom_name = create_unique_axiom_name(this->m_next_axiom_index, this->m_existing_predicate_names);
    const auto translated_predicate = repositories.get_or_create_predicate(axiom_name, translated_head_parameters);
    m_instantiated_predicates.insert(translated_predicate);
    const auto translated_atom = repositories.get_or_create_atom(translated_predicate, translated_terms);
    const auto translated_substituted_condition =
        repositories.get_or_create_condition(repositories.get_or_create_condition_literal(repositories.get_or_create_literal(true, translated_atom)));
    const auto translated_axiom_literal = repositories.get_or_create_literal(false, translated_atom);

    const auto translated_axiom = repositories.get_or_create_axiom(translated_axiom_parameters, translated_axiom_literal, axiom_condition);

    m_instantiated_axioms.insert(translated_axiom);

    m_scopes.close_scope();

    return translated_substituted_condition;
}

Action RemoveUniversalQuantifiersTranslator::translate_level_2(Action action, Repositories& repositories)
{
    auto translated_parameters = translate_level_0(action->get_parameters(), repositories);

    this->m_scopes.open_scope(translated_parameters);

    auto translated_condition = translate_level_0(action->get_condition(), repositories);
    auto translated_effect = translate_level_0(action->get_effect(), repositories);

    auto translated_action =
        repositories.get_or_create_action(action->get_name(), action->get_original_arity(), translated_parameters, translated_condition, translated_effect);

    this->m_scopes.close_scope();

    return translated_action;
}

Axiom RemoveUniversalQuantifiersTranslator::translate_level_2(Axiom axiom, Repositories& repositories)
{
    auto translated_parameters = translate_level_0(axiom->get_parameters(), repositories);

    this->m_scopes.open_scope(translated_parameters);

    auto translated_literal = translate_level_0(axiom->get_literal(), repositories);
    auto translated_condition = translate_level_0(axiom->get_condition(), repositories);

    auto translated_axiom = repositories.get_or_create_axiom(translated_parameters, translated_literal, translated_condition);

    this->m_scopes.close_scope();

    return translated_axiom;
}

Domain RemoveUniversalQuantifiersTranslator::translate_level_2(const Domain& domain, DomainBuilder& builder)
{
    for (const auto& predicate : domain->get_predicates())
    {
        m_existing_predicate_names.insert(predicate->get_name());
    }

    auto& repositories = builder.get_repositories();

    builder.get_name() = domain->get_name();
    builder.get_filepath() = domain->get_filepath();
    builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
    const auto translated_types = this->translate_level_0(domain->get_types(), repositories);
    builder.get_types().insert(builder.get_types().end(), translated_types.begin(), translated_types.end());
    const auto translated_constants = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_constants().insert(builder.get_constants().end(), translated_constants.begin(), translated_constants.end());
    const auto translated_predicates = this->translate_level_0(domain->get_predicates(), repositories);
    const auto translated_static_initial_literals = this->translate_level_0(domain->get_static_initial_literals(), repositories);
    builder.get_static_initial_literals().insert(builder.get_static_initial_literals().end(),
                                                 translated_static_initial_literals.begin(),
                                                 translated_static_initial_literals.end());
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_function_skeletons = this->translate_level_0(domain->get_function_skeletons(), repositories);
    builder.get_function_skeletons().insert(builder.get_function_skeletons().end(), translated_function_skeletons.begin(), translated_function_skeletons.end());
    const auto translated_actions = this->translate_level_0(domain->get_actions(), repositories);
    builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());
    const auto translated_axioms = this->translate_level_0(domain->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    for (const auto& predicate : m_instantiated_predicates)
    {
        builder.get_predicates().push_back(predicate);
    }
    for (const auto& axiom : m_instantiated_axioms)
    {
        builder.get_axioms().push_back(axiom);
    }

    return builder.get_result();
}

Problem RemoveUniversalQuantifiersTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    for (const auto& predicate : problem->get_problem_and_domain_predicates())
    {
        m_existing_predicate_names.insert(predicate->get_name());
    }

    auto& repositories = builder.get_repositories();

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();
    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
    const auto translated_objects = this->translate_level_0(problem->get_objects(), repositories);
    builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
    const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
    const auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());
    const auto translated_initial_function_values = this->translate_level_0(problem->get_initial_function_values(), repositories);
    builder.get_initial_function_values().insert(builder.get_initial_function_values().end(),
                                                 translated_initial_function_values.begin(),
                                                 translated_initial_function_values.end());
    builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition(), repositories);
    builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric(), repositories);
    const auto translated_axioms = this->translate_level_0(problem->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    for (const auto& predicate : m_instantiated_predicates)
    {
        builder.get_predicates().push_back(predicate);
    }
    for (const auto& axiom : m_instantiated_axioms)
    {
        builder.get_axioms().push_back(axiom);
    }

    return builder.get_result(problem->get_index());
}

}