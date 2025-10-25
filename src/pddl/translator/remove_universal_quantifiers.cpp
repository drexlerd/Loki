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

    /* Translate (exists(vars, not phi)) to NNF, then translate recursively. */
    const auto translated_condition_exists_not_phi =
        this->translate_level_0(ToNegationNormalFormTranslator().translate_level_0(
                                    repositories.get_or_create_condition(repositories.get_or_create_condition_exists(
                                        translated_condition_parameters,
                                        repositories.get_or_create_condition(repositories.get_or_create_condition_not(condition->get_condition())))),
                                    repositories),
                                repositories);

    /* The free variables in the translation of (exists(vars, not phi)) become parameters. We obtain their types from the parameters in the parent scope. */
    const auto translated_free_variables = collect_free_variables(*translated_condition_exists_not_phi);

    /* Instantiate the parameters. */
    auto translated_parameters = ParameterList {};
    auto translated_terms = TermList {};
    for (const auto translated_free_variable : translated_free_variables)
    {
        const auto translated_optional_parameter = scope.get_parameter(translated_free_variable);
        assert(translated_optional_parameter.has_value());  // We require all variables in axioms or actions to be bound to some quantifier

        const auto translated_bases = translated_optional_parameter.value()->get_bases();

        const auto translated_parameter = repositories.get_or_create_parameter(translated_free_variable, translated_bases);
        translated_parameters.push_back(translated_parameter);
        translated_terms.push_back(repositories.get_or_create_term(translated_free_variable));
    }

    const auto axiom_name = create_unique_axiom_name(this->m_next_axiom_index, this->m_existing_predicate_names);
    const auto translated_predicate = repositories.get_or_create_predicate(axiom_name, translated_parameters);
    m_instantiated_predicates.insert(translated_predicate);
    const auto translated_atom = repositories.get_or_create_atom(translated_predicate, translated_terms);
    const auto translated_substituted_condition =
        repositories.get_or_create_condition(repositories.get_or_create_condition_literal(repositories.get_or_create_literal(false, translated_atom)));
    const auto translated_axiom_literal = repositories.get_or_create_literal(true, translated_atom);

    const auto translated_axiom = repositories.get_or_create_axiom(translated_parameters, translated_axiom_literal, translated_condition_exists_not_phi);

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
    builder.get_types() = this->translate_level_0(domain->get_types(), repositories);
    builder.get_constants() = this->translate_level_0(domain->get_constants(), repositories);
    builder.get_predicates() = this->translate_level_0(domain->get_predicates(), repositories);
    builder.get_static_initial_literals() = this->translate_level_0(domain->get_static_initial_literals(), repositories);
    builder.get_function_skeletons() = this->translate_level_0(domain->get_function_skeletons(), repositories);
    builder.get_actions() = this->translate_level_0(domain->get_actions(), repositories);
    builder.get_axioms() = this->translate_level_0(domain->get_axioms(), repositories);

    for (const auto& predicate : m_instantiated_predicates)
    {
        builder.get_predicates().push_back(predicate);
    }
    for (const auto& axiom : m_instantiated_axioms)
    {
        builder.get_axioms().push_back(axiom);
    }

    auto requirements = domain->get_requirements()->get_requirements();
    if (!builder.get_axioms().empty())
        requirements.insert(loki::RequirementEnum::DERIVED_PREDICATES);
    builder.get_requirements() = repositories.get_or_create_requirements(std::move(requirements));

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
    builder.get_objects() = this->translate_level_0(problem->get_objects(), repositories);
    builder.get_predicates() = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_initial_literals() = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_function_values() = this->translate_level_0(problem->get_initial_function_values(), repositories);
    builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition(), repositories);
    builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric(), repositories);
    builder.get_axioms() = this->translate_level_0(problem->get_axioms(), repositories);

    for (const auto& predicate : m_instantiated_predicates)
    {
        builder.get_predicates().push_back(predicate);
    }
    for (const auto& axiom : m_instantiated_axioms)
    {
        builder.get_axioms().push_back(axiom);
    }

    auto requirements = problem->get_requirements()->get_requirements();
    if (!builder.get_axioms().empty())
        requirements.insert(loki::RequirementEnum::DERIVED_PREDICATES);
    builder.get_requirements() = repositories.get_or_create_requirements(std::move(requirements));

    return builder.get_result(problem->get_index());
}

}