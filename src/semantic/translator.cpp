/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "loki/semantic/translator.hpp"

#include <stdexcept>
#include <utility>

namespace loki::semantic
{

namespace detail
{

template<typename T>
void compose_map(IndexMap<T>& out, const IndexMap<T>& source_to_middle, const IndexMap<T>& middle_to_target)
{
    out.clear();
    for (const auto& [source, middle] : source_to_middle)
    {
        if (auto it = middle_to_target.find(middle.get_value()); it != middle_to_target.end())
            out.emplace(source, it->second);
    }
}

ygg::Index<formalism::Type>
copy_type_for_metadata(TranslationStorage& target, const formalism::Repository& source_repository, ygg::Index<formalism::Type> source)
{
    ygg::Index<formalism::Type> out;
    if (find_mapped(target.types, source, out))
        return out;

    const auto& data = source_repository[source];
    auto bases = ygg::IndexList<formalism::Type> {};
    for (auto base : data.bases)
        bases.push_back(copy_type_for_metadata(target, source_repository, base));

    out = formalism::get_or_create<formalism::Type>(target.repository, data.name, std::move(bases)).get_index();
    remember(target.types, source, out);
    return out;
}

std::shared_ptr<TranslationStorage> canonicalize_domain_storage(formalism::DomainView original_domain, const std::shared_ptr<TranslationStorage>& middle)
{
    auto canonical = std::make_shared<TranslationStorage>(middle->repository.get_index());
    auto copier = CanonicalCopyTranslator(canonical);
    const auto middle_domain = ygg::make_view(middle->translated_domain, middle->repository);
    const auto canonical_domain = copier.copy_domain(middle_domain);

    const auto middle_domains = canonical->domains;
    const auto middle_requirements = canonical->requirements;
    const auto middle_types = canonical->types;
    const auto middle_objects = canonical->objects;
    const auto middle_variables = canonical->variables;
    const auto middle_parameters = canonical->parameters;
    const auto middle_predicates = canonical->predicates;
    const auto middle_functions = canonical->functions;
    const auto middle_terms = canonical->terms;
    const auto middle_atoms = canonical->atoms;
    const auto middle_literals = canonical->literals;
    const auto middle_numbers = canonical->numbers;
    const auto middle_function_terms = canonical->function_terms;
    const auto middle_unary_expressions = canonical->unary_expressions;
    const auto middle_binary_expressions = canonical->binary_expressions;
    const auto middle_multi_expressions = canonical->multi_expressions;
    const auto middle_function_expressions = canonical->function_expressions;
    const auto middle_condition_literals = canonical->condition_literals;
    const auto middle_condition_ands = canonical->condition_ands;
    const auto middle_condition_ors = canonical->condition_ors;
    const auto middle_condition_nots = canonical->condition_nots;
    const auto middle_condition_implies = canonical->condition_implies;
    const auto middle_condition_exists = canonical->condition_exists;
    const auto middle_condition_foralls = canonical->condition_foralls;
    const auto middle_condition_numeric_constraints = canonical->condition_numeric_constraints;
    const auto middle_conditions = canonical->conditions;
    const auto middle_effect_literals = canonical->effect_literals;
    const auto middle_effect_ands = canonical->effect_ands;
    const auto middle_effect_numerics = canonical->effect_numerics;
    const auto middle_effect_foralls = canonical->effect_foralls;
    const auto middle_effect_whens = canonical->effect_whens;
    const auto middle_effect_one_ofs = canonical->effect_one_ofs;
    const auto middle_effect_probabilistic_alternatives = canonical->effect_probabilistic_alternatives;
    const auto middle_effect_probabilistics = canonical->effect_probabilistics;
    const auto middle_effects = canonical->effects;
    const auto middle_actions = canonical->actions;
    const auto middle_axioms = canonical->axioms;
    const auto middle_metrics = canonical->metrics;
    const auto middle_initial_function_values = canonical->initial_function_values;
    const auto middle_tasks = canonical->tasks;

    canonical->original_domain = original_domain.get_index();
    canonical->translated_domain = canonical_domain.get_index();
    canonical->domains.clear();
    remember(canonical->domains, original_domain.get_index(), canonical_domain.get_index());
    compose_map(canonical->requirements, middle->requirements, middle_requirements);
    compose_map(canonical->types, middle->types, middle_types);
    compose_map(canonical->objects, middle->objects, middle_objects);
    compose_map(canonical->variables, middle->variables, middle_variables);
    compose_map(canonical->parameters, middle->parameters, middle_parameters);
    compose_map(canonical->predicates, middle->predicates, middle_predicates);
    compose_map(canonical->functions, middle->functions, middle_functions);
    compose_map(canonical->terms, middle->terms, middle_terms);
    compose_map(canonical->atoms, middle->atoms, middle_atoms);
    compose_map(canonical->literals, middle->literals, middle_literals);
    compose_map(canonical->numbers, middle->numbers, middle_numbers);
    compose_map(canonical->function_terms, middle->function_terms, middle_function_terms);
    compose_map(canonical->unary_expressions, middle->unary_expressions, middle_unary_expressions);
    compose_map(canonical->binary_expressions, middle->binary_expressions, middle_binary_expressions);
    compose_map(canonical->multi_expressions, middle->multi_expressions, middle_multi_expressions);
    compose_map(canonical->function_expressions, middle->function_expressions, middle_function_expressions);
    compose_map(canonical->condition_literals, middle->condition_literals, middle_condition_literals);
    compose_map(canonical->condition_ands, middle->condition_ands, middle_condition_ands);
    compose_map(canonical->condition_ors, middle->condition_ors, middle_condition_ors);
    compose_map(canonical->condition_nots, middle->condition_nots, middle_condition_nots);
    compose_map(canonical->condition_implies, middle->condition_implies, middle_condition_implies);
    compose_map(canonical->condition_exists, middle->condition_exists, middle_condition_exists);
    compose_map(canonical->condition_foralls, middle->condition_foralls, middle_condition_foralls);
    compose_map(canonical->condition_numeric_constraints, middle->condition_numeric_constraints, middle_condition_numeric_constraints);
    compose_map(canonical->conditions, middle->conditions, middle_conditions);
    compose_map(canonical->effect_literals, middle->effect_literals, middle_effect_literals);
    compose_map(canonical->effect_ands, middle->effect_ands, middle_effect_ands);
    compose_map(canonical->effect_numerics, middle->effect_numerics, middle_effect_numerics);
    compose_map(canonical->effect_foralls, middle->effect_foralls, middle_effect_foralls);
    compose_map(canonical->effect_whens, middle->effect_whens, middle_effect_whens);
    compose_map(canonical->effect_one_ofs, middle->effect_one_ofs, middle_effect_one_ofs);
    compose_map(canonical->effect_probabilistic_alternatives, middle->effect_probabilistic_alternatives, middle_effect_probabilistic_alternatives);
    compose_map(canonical->effect_probabilistics, middle->effect_probabilistics, middle_effect_probabilistics);
    compose_map(canonical->effects, middle->effects, middle_effects);
    compose_map(canonical->actions, middle->actions, middle_actions);
    compose_map(canonical->axioms, middle->axioms, middle_axioms);
    compose_map(canonical->metrics, middle->metrics, middle_metrics);
    compose_map(canonical->initial_function_values, middle->initial_function_values, middle_initial_function_values);
    compose_map(canonical->tasks, middle->tasks, middle_tasks);
    canonical->object_types_by_name.clear();
    for (const auto& [name, types] : middle->object_types_by_name)
    {
        auto remapped = ygg::IndexList<formalism::Type> {};
        for (auto type : types)
        {
            if (auto it = middle_types.find(type.get_value()); it != middle_types.end())
            {
                remapped.push_back(it->second);
            }
            else
            {
                remapped.push_back(copy_type_for_metadata(*canonical, middle->repository, type));
            }
        }
        canonical->object_types_by_name.emplace(name, std::move(remapped));
    }
    return canonical;
}

template<typename T>
void copy_map(IndexMap<T>& target, const IndexMap<T>& source)
{
    target = source;
}

void inherit_domain_mappings(TranslationStorage& problem, const TranslationStorage& domain)
{
    copy_map(problem.domains, domain.domains);
    copy_map(problem.requirements, domain.requirements);
    copy_map(problem.types, domain.types);
    copy_map(problem.objects, domain.objects);
    copy_map(problem.variables, domain.variables);
    copy_map(problem.parameters, domain.parameters);
    copy_map(problem.predicates, domain.predicates);
    copy_map(problem.functions, domain.functions);
    copy_map(problem.terms, domain.terms);
    copy_map(problem.atoms, domain.atoms);
    copy_map(problem.literals, domain.literals);
    copy_map(problem.numbers, domain.numbers);
    copy_map(problem.function_terms, domain.function_terms);
    copy_map(problem.unary_expressions, domain.unary_expressions);
    copy_map(problem.binary_expressions, domain.binary_expressions);
    copy_map(problem.multi_expressions, domain.multi_expressions);
    copy_map(problem.function_expressions, domain.function_expressions);
    copy_map(problem.condition_literals, domain.condition_literals);
    copy_map(problem.condition_ands, domain.condition_ands);
    copy_map(problem.condition_ors, domain.condition_ors);
    copy_map(problem.condition_nots, domain.condition_nots);
    copy_map(problem.condition_implies, domain.condition_implies);
    copy_map(problem.condition_exists, domain.condition_exists);
    copy_map(problem.condition_foralls, domain.condition_foralls);
    copy_map(problem.condition_numeric_constraints, domain.condition_numeric_constraints);
    copy_map(problem.conditions, domain.conditions);
    copy_map(problem.effect_literals, domain.effect_literals);
    copy_map(problem.effect_ands, domain.effect_ands);
    copy_map(problem.effect_numerics, domain.effect_numerics);
    copy_map(problem.effect_foralls, domain.effect_foralls);
    copy_map(problem.effect_whens, domain.effect_whens);
    copy_map(problem.effect_one_ofs, domain.effect_one_ofs);
    copy_map(problem.effect_probabilistic_alternatives, domain.effect_probabilistic_alternatives);
    copy_map(problem.effect_probabilistics, domain.effect_probabilistics);
    copy_map(problem.effects, domain.effects);
    copy_map(problem.actions, domain.actions);
    copy_map(problem.axioms, domain.axioms);
    copy_map(problem.metrics, domain.metrics);
    copy_map(problem.initial_function_values, domain.initial_function_values);
    problem.object_types_by_name = domain.object_types_by_name;
    problem.original_domain = domain.original_domain;
    problem.translated_domain = domain.translated_domain;
}

template<typename T>
void copy_identity_map(IndexMap<T>& target, const IndexMap<T>& source)
{
    for (const auto& [_, index] : source)
        target.emplace(index.get_value(), index);
}

void inherit_domain_identity_mappings(TranslationStorage& problem, const TranslationStorage& domain)
{
    problem.domains.emplace(domain.translated_domain.get_value(), domain.translated_domain);
    copy_identity_map(problem.requirements, domain.requirements);
    copy_identity_map(problem.types, domain.types);
    copy_identity_map(problem.objects, domain.objects);
    copy_identity_map(problem.variables, domain.variables);
    copy_identity_map(problem.parameters, domain.parameters);
    copy_identity_map(problem.predicates, domain.predicates);
    copy_identity_map(problem.functions, domain.functions);
    copy_identity_map(problem.terms, domain.terms);
    copy_identity_map(problem.atoms, domain.atoms);
    copy_identity_map(problem.literals, domain.literals);
    copy_identity_map(problem.numbers, domain.numbers);
    copy_identity_map(problem.function_terms, domain.function_terms);
    copy_identity_map(problem.unary_expressions, domain.unary_expressions);
    copy_identity_map(problem.binary_expressions, domain.binary_expressions);
    copy_identity_map(problem.multi_expressions, domain.multi_expressions);
    copy_identity_map(problem.function_expressions, domain.function_expressions);
    copy_identity_map(problem.condition_literals, domain.condition_literals);
    copy_identity_map(problem.condition_ands, domain.condition_ands);
    copy_identity_map(problem.condition_ors, domain.condition_ors);
    copy_identity_map(problem.condition_nots, domain.condition_nots);
    copy_identity_map(problem.condition_implies, domain.condition_implies);
    copy_identity_map(problem.condition_exists, domain.condition_exists);
    copy_identity_map(problem.condition_foralls, domain.condition_foralls);
    copy_identity_map(problem.condition_numeric_constraints, domain.condition_numeric_constraints);
    copy_identity_map(problem.conditions, domain.conditions);
    copy_identity_map(problem.effect_literals, domain.effect_literals);
    copy_identity_map(problem.effect_ands, domain.effect_ands);
    copy_identity_map(problem.effect_numerics, domain.effect_numerics);
    copy_identity_map(problem.effect_foralls, domain.effect_foralls);
    copy_identity_map(problem.effect_whens, domain.effect_whens);
    copy_identity_map(problem.effect_one_ofs, domain.effect_one_ofs);
    copy_identity_map(problem.effect_probabilistic_alternatives, domain.effect_probabilistic_alternatives);
    copy_identity_map(problem.effect_probabilistics, domain.effect_probabilistics);
    copy_identity_map(problem.effects, domain.effects);
    copy_identity_map(problem.actions, domain.actions);
    copy_identity_map(problem.axioms, domain.axioms);
    copy_identity_map(problem.metrics, domain.metrics);
    copy_identity_map(problem.initial_function_values, domain.initial_function_values);
    problem.object_types_by_name = domain.object_types_by_name;
    problem.original_domain = domain.original_domain;
    problem.translated_domain = domain.translated_domain;
}

std::shared_ptr<TranslationStorage>
canonicalize_problem_storage(formalism::TaskView middle_task, const std::shared_ptr<TranslationStorage>& middle, const TranslationStorage& domain)
{
    auto canonical = std::make_shared<TranslationStorage>(middle->repository.get_index(), &domain.repository);
    inherit_domain_identity_mappings(*canonical, domain);
    if (middle->translated_domain == domain.translated_domain)
        remember(canonical->domains, middle->translated_domain, domain.translated_domain);
    auto copier = CanonicalCopyTranslator(canonical);
    copier.copy_task(middle_task);
    return canonical;
}

}  // namespace detail

DomainTranslationResult::DomainTranslationResult(formalism::DomainView original_domain_, std::shared_ptr<detail::TranslationStorage> storage_) :
    m_original_domain(original_domain_),
    m_storage(std::move(storage_))
{
}

formalism::DomainView DomainTranslationResult::get_original_domain() const noexcept { return m_original_domain; }

formalism::DomainView DomainTranslationResult::get_translated_domain() const noexcept
{
    return ygg::make_view(m_storage->translated_domain, m_storage->repository);
}

const formalism::Repository& DomainTranslationResult::get_repository() const noexcept { return m_storage->repository; }

formalism::Repository& DomainTranslationResult::get_repository() noexcept { return m_storage->repository; }

ProblemTranslationResult::ProblemTranslationResult(formalism::TaskView original_task_,
                                                   std::shared_ptr<detail::TranslationStorage> storage_,
                                                   formalism::TaskView translated_task_) :
    m_original_task(original_task_),
    m_storage(std::move(storage_)),
    m_translated_task(translated_task_)
{
}

formalism::TaskView ProblemTranslationResult::get_original_task() const noexcept { return m_original_task; }

formalism::TaskView ProblemTranslationResult::get_translated_task() const noexcept { return m_translated_task; }

const formalism::Repository& ProblemTranslationResult::get_repository() const noexcept { return m_storage->repository; }

formalism::Repository& ProblemTranslationResult::get_repository() noexcept { return m_storage->repository; }

DomainTranslationResult translate(formalism::DomainView domain, const TranslatorOptions& options)
{
    auto middle = std::make_shared<detail::TranslationStorage>(1);
    auto semantic_copier = detail::CopyTranslator(middle, options.remove_typing);
    for ([[maybe_unused]] auto step : domain_translation_steps()) {}
    semantic_copier.copy_domain(domain);
    return DomainTranslationResult(domain, detail::canonicalize_domain_storage(domain, middle));
}

ProblemTranslationResult translate(formalism::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options)
{
    if (task.get_data().domain != result.get_original_domain().get_index())
        throw std::runtime_error("translate(task, result): task domain must match original domain in DomainTranslationResult.");

    auto middle = std::make_shared<detail::TranslationStorage>(2, &result.m_storage->repository);
    detail::inherit_domain_mappings(*middle, *result.m_storage);
    auto semantic_copier = detail::CopyTranslator(middle, options.remove_typing);
    for ([[maybe_unused]] auto step : task_translation_steps()) {}
    const auto middle_task = semantic_copier.copy_task(task);
    const auto canonical = detail::canonicalize_problem_storage(middle_task, middle, *result.m_storage);
    auto translated_task = ygg::make_view(canonical->tasks.at(middle_task.get_index().get_value()), canonical->repository);
    return ProblemTranslationResult(task, canonical, translated_task);
}

}  // namespace loki::semantic
