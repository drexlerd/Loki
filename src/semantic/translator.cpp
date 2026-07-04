/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#include "loki/semantic/translator.hpp"

#include "loki/semantic/errors.hpp"

#include <utility>

namespace loki::semantic
{

namespace detail
{

template<typename T>
void compose_map(ViewMap<T>& out, const ViewMap<T>& source_to_middle, const ViewMap<T>& middle_to_target)
{
    out.clear();
    for (const auto& [source, middle] : source_to_middle)
    {
        if (auto it = middle_to_target.find(middle.get_index().get_value()); it != middle_to_target.end())
            out.emplace(source, it->second);
    }
}

formalism::TypeView copy_type_view_for_metadata(TranslationStorage& target, formalism::TypeView source)
{
    if (auto mapped = find_mapped(target.types, source.get_index()))
        return *mapped;

    auto bases = ygg::IndexList<formalism::Type> {};
    for (auto base : source.get_bases())
        bases.push_back(copy_type_view_for_metadata(target, base).get_index());

    auto out = formalism::get_or_create<formalism::Type>(target.repository, source.get_name(), std::move(bases));
    remember(target.types, source.get_index(), out);
    return out;
}

void remap_object_type_metadata(TranslationStorage& target,
                                const TranslationStorage& source,
                                const ViewMap<formalism::Object>& object_map,
                                const ViewMap<formalism::Type>& type_map)
{
    target.object_types.clear();
    target.object_type_views.clear();
    for (const auto& [source_object, types] : source.object_types)
    {
        auto object_it = object_map.find(source_object);
        if (object_it == object_map.end())
            continue;

        auto remapped = ygg::IndexList<formalism::Type> {};
        auto complete = true;
        for (auto type : types)
        {
            if (auto type_it = type_map.find(type.get_value()); type_it != type_map.end())
                remapped.push_back(type_it->second.get_index());
            else
            {
                complete = false;
                break;
            }
        }
        if (complete)
            target.object_types.emplace(object_it->second.get_index().get_value(), std::move(remapped));
    }

    for (const auto& [source_object, type_views] : source.object_type_views)
    {
        auto object_it = object_map.find(source_object);
        if (object_it == object_map.end())
            continue;

        auto remapped = std::vector<formalism::TypeView> {};
        auto remapped_indices = ygg::IndexList<formalism::Type> {};
        for (auto type : type_views)
        {
            if (auto type_it = type_map.find(type.get_index().get_value()); type_it != type_map.end())
                remapped.push_back(type_it->second);
            else
                remapped.push_back(copy_type_view_for_metadata(target, type));
            remapped_indices.push_back(remapped.back().get_index());
        }
        const auto object = object_it->second.get_index().get_value();
        target.object_types[object] = std::move(remapped_indices);
        target.object_type_views[object] = std::move(remapped);
    }
}

std::shared_ptr<TranslationStorage> canonicalize_domain_storage(formalism::DomainView original_domain, const std::shared_ptr<TranslationStorage>& middle)
{
    auto canonical = std::make_shared<TranslationStorage>(middle->repository.get_index());
    auto copier = CanonicalCopyTranslator(canonical);
    const auto middle_domain = middle->domains.at(original_domain.get_index().get_value());
    const auto canonical_domain = copier.copy_domain(middle_domain);

    const auto middle_domains = canonical->domains;
    const auto middle_requirements = canonical->requirements;
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

    auto middle_types = canonical->types;
    remap_object_type_metadata(*canonical, *middle, middle_objects, middle_types);
    middle_types = canonical->types;

    canonical->original_domain = original_domain.get_index();
    canonical->translated_domain = canonical_domain;
    canonical->domains.clear();
    remember(canonical->domains, original_domain.get_index(), canonical_domain);
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
    return canonical;
}

template<typename T>
void copy_map(ViewMap<T>& target, const ViewMap<T>& source)
{
    target = source;
}

struct PhaseStep
{
    TranslationPhase phase;
    std::string_view name;
};

const std::vector<PhaseStep>& domain_phase_steps()
{
    static const auto steps = std::vector<PhaseStep> {
        { TranslationPhase::ToNegationNormalForm, "to-negation-normal-form" },
        { TranslationPhase::RenameQuantifiedVariables, "rename-quantified-variables" },
        { TranslationPhase::RemoveUniversalQuantifiers, "remove-universal-quantifiers" },
        { TranslationPhase::ToDisjunctiveNormalForm, "to-disjunctive-normal-form" },
        { TranslationPhase::SplitDisjunctiveConditions, "split-disjunctive-conditions" },
        { TranslationPhase::MoveExistentialQuantifiers, "move-existential-quantifiers" },
        { TranslationPhase::AddTypePredicates, "add-type-predicates" },
        { TranslationPhase::ToEffectNormalForm, "to-effect-normal-form" },
        { TranslationPhase::MultiplyConditionalEffects, "multiply-conditional-effects" },
        { TranslationPhase::InitializeEquality, "initialize-equality" },
    };
    return steps;
}

const std::vector<PhaseStep>& task_phase_steps()
{
    static const auto steps = std::vector<PhaseStep> {
        { TranslationPhase::ToNegationNormalForm, "to-negation-normal-form" },
        { TranslationPhase::RenameQuantifiedVariables, "rename-quantified-variables" },
        { TranslationPhase::RemoveUniversalQuantifiers, "remove-universal-quantifiers" },
        { TranslationPhase::SimplifyGoal, "simplify-goal" },
        { TranslationPhase::ToDisjunctiveNormalForm, "to-disjunctive-normal-form" },
        { TranslationPhase::SplitDisjunctiveConditions, "split-disjunctive-conditions" },
        { TranslationPhase::MoveExistentialQuantifiers, "move-existential-quantifiers" },
        { TranslationPhase::ToEffectNormalForm, "to-effect-normal-form" },
        { TranslationPhase::InitializeEquality, "initialize-equality" },
        { TranslationPhase::AddTypePredicates, "add-type-predicates" },
    };
    return steps;
}

void compose_storage_maps_from_previous(TranslationStorage& target, const TranslationStorage& previous)
{
    const auto precompose_objects = target.objects;
    auto precompose_types = target.types;
    remap_object_type_metadata(target, previous, precompose_objects, precompose_types);

    const auto requirements = target.requirements;
    const auto types = target.types;
    const auto objects = target.objects;
    const auto variables = target.variables;
    const auto parameters = target.parameters;
    const auto predicates = target.predicates;
    const auto functions = target.functions;
    const auto terms = target.terms;
    const auto atoms = target.atoms;
    const auto literals = target.literals;
    const auto numbers = target.numbers;
    const auto function_terms = target.function_terms;
    const auto unary_expressions = target.unary_expressions;
    const auto binary_expressions = target.binary_expressions;
    const auto multi_expressions = target.multi_expressions;
    const auto function_expressions = target.function_expressions;
    const auto condition_literals = target.condition_literals;
    const auto condition_ands = target.condition_ands;
    const auto condition_ors = target.condition_ors;
    const auto condition_nots = target.condition_nots;
    const auto condition_implies = target.condition_implies;
    const auto condition_exists = target.condition_exists;
    const auto condition_foralls = target.condition_foralls;
    const auto condition_numeric_constraints = target.condition_numeric_constraints;
    const auto conditions = target.conditions;
    const auto effect_literals = target.effect_literals;
    const auto effect_ands = target.effect_ands;
    const auto effect_numerics = target.effect_numerics;
    const auto effect_foralls = target.effect_foralls;
    const auto effect_whens = target.effect_whens;
    const auto effect_one_ofs = target.effect_one_ofs;
    const auto effect_probabilistic_alternatives = target.effect_probabilistic_alternatives;
    const auto effect_probabilistics = target.effect_probabilistics;
    const auto effects = target.effects;
    const auto actions = target.actions;
    const auto axioms = target.axioms;
    const auto metrics = target.metrics;
    const auto initial_function_values = target.initial_function_values;
    const auto domains = target.domains;
    const auto tasks = target.tasks;

    target.original_domain = previous.original_domain;

    compose_map(target.requirements, previous.requirements, requirements);
    compose_map(target.types, previous.types, types);
    compose_map(target.objects, previous.objects, objects);
    compose_map(target.variables, previous.variables, variables);
    compose_map(target.parameters, previous.parameters, parameters);
    compose_map(target.predicates, previous.predicates, predicates);
    compose_map(target.functions, previous.functions, functions);
    compose_map(target.terms, previous.terms, terms);
    compose_map(target.atoms, previous.atoms, atoms);
    compose_map(target.literals, previous.literals, literals);
    compose_map(target.numbers, previous.numbers, numbers);
    compose_map(target.function_terms, previous.function_terms, function_terms);
    compose_map(target.unary_expressions, previous.unary_expressions, unary_expressions);
    compose_map(target.binary_expressions, previous.binary_expressions, binary_expressions);
    compose_map(target.multi_expressions, previous.multi_expressions, multi_expressions);
    compose_map(target.function_expressions, previous.function_expressions, function_expressions);
    compose_map(target.condition_literals, previous.condition_literals, condition_literals);
    compose_map(target.condition_ands, previous.condition_ands, condition_ands);
    compose_map(target.condition_ors, previous.condition_ors, condition_ors);
    compose_map(target.condition_nots, previous.condition_nots, condition_nots);
    compose_map(target.condition_implies, previous.condition_implies, condition_implies);
    compose_map(target.condition_exists, previous.condition_exists, condition_exists);
    compose_map(target.condition_foralls, previous.condition_foralls, condition_foralls);
    compose_map(target.condition_numeric_constraints, previous.condition_numeric_constraints, condition_numeric_constraints);
    compose_map(target.conditions, previous.conditions, conditions);
    compose_map(target.effect_literals, previous.effect_literals, effect_literals);
    compose_map(target.effect_ands, previous.effect_ands, effect_ands);
    compose_map(target.effect_numerics, previous.effect_numerics, effect_numerics);
    compose_map(target.effect_foralls, previous.effect_foralls, effect_foralls);
    compose_map(target.effect_whens, previous.effect_whens, effect_whens);
    compose_map(target.effect_one_ofs, previous.effect_one_ofs, effect_one_ofs);
    compose_map(target.effect_probabilistic_alternatives, previous.effect_probabilistic_alternatives, effect_probabilistic_alternatives);
    compose_map(target.effect_probabilistics, previous.effect_probabilistics, effect_probabilistics);
    compose_map(target.effects, previous.effects, effects);
    compose_map(target.actions, previous.actions, actions);
    compose_map(target.axioms, previous.axioms, axioms);
    compose_map(target.metrics, previous.metrics, metrics);
    compose_map(target.initial_function_values, previous.initial_function_values, initial_function_values);
    compose_map(target.domains, previous.domains, domains);
    compose_map(target.tasks, previous.tasks, tasks);
}

void inherit_domain_mappings(TranslationStorage& problem, const TranslationStorage& domain)
{
    copy_map(problem.domains, domain.domains);
    copy_map(problem.requirements, domain.requirements);
    copy_map(problem.types, domain.types);
    copy_map(problem.objects, domain.objects);
    copy_map(problem.predicates, domain.predicates);
    copy_map(problem.functions, domain.functions);
    problem.object_types = domain.object_types;
    problem.object_type_views = domain.object_type_views;
    problem.original_domain = domain.original_domain;
    problem.translated_domain = domain.translated_domain;
}

template<typename T>
void copy_identity_map(ViewMap<T>& target, const ViewMap<T>& source)
{
    for (const auto& [_, view] : source)
        target.emplace(view.get_index().get_value(), view);
}

template<typename T>
formalism::EntityView<T> target_view(const ViewMap<T>& map, ygg::Index<T> target)
{
    for (const auto& [_, view] : map)
        if (view.get_index() == target)
            return view;
    throw SemanticError("Missing translated view in translation storage.");
}

void inherit_domain_identity_mappings(TranslationStorage& problem, const TranslationStorage& domain)
{
    problem.domains.emplace(domain.translated_domain->get_index().get_value(), *domain.translated_domain);
    copy_identity_map(problem.requirements, domain.requirements);
    copy_identity_map(problem.types, domain.types);
    copy_identity_map(problem.objects, domain.objects);
    copy_identity_map(problem.predicates, domain.predicates);
    copy_identity_map(problem.functions, domain.functions);
    problem.object_types = domain.object_types;
    problem.object_type_views = domain.object_type_views;
    problem.original_domain = domain.original_domain;
    problem.translated_domain = domain.translated_domain;
}

std::shared_ptr<TranslationStorage>
canonicalize_problem_storage(formalism::TaskView middle_task, const std::shared_ptr<TranslationStorage>& middle, const TranslationStorage& domain)
{
    auto canonical = std::make_shared<TranslationStorage>(middle->repository.get_index(), &domain.repository);
    inherit_domain_identity_mappings(*canonical, domain);
    if (middle->translated_domain->get_index() == domain.translated_domain->get_index())
        remember(canonical->domains, middle->translated_domain->get_index(), *domain.translated_domain);
    auto copier = CanonicalCopyTranslator(canonical);
    copier.copy_task(middle_task);
    compose_storage_maps_from_previous(*canonical, *middle);
    return canonical;
}

}  // namespace detail

DomainTranslationResult::DomainTranslationResult(formalism::DomainView original_domain_, std::shared_ptr<detail::TranslationStorage> storage_) :
    m_original_domain(original_domain_),
    m_storage(std::move(storage_))
{
}

formalism::DomainView DomainTranslationResult::get_original_domain() const noexcept { return m_original_domain; }

formalism::DomainView DomainTranslationResult::get_translated_domain() const noexcept { return *m_storage->translated_domain; }

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
    auto current_domain = domain;
    auto current_storage = std::shared_ptr<detail::TranslationStorage> {};
    auto phase_index = size_t { 1 };

    for (const auto& step : detail::domain_phase_steps())
    {
        if (step.phase == TranslationPhase::MultiplyConditionalEffects && !options.multiply_conditional_effects)
            continue;

        auto phase_storage = std::make_shared<detail::TranslationStorage>(phase_index++);
        auto semantic_copier = detail::CopyTranslator(phase_storage, options.remove_typing, step.phase);
        current_domain = semantic_copier.copy_domain(current_domain);
        if (current_storage)
            detail::compose_storage_maps_from_previous(*phase_storage, *current_storage);
        current_storage = std::move(phase_storage);
    }

    return DomainTranslationResult(domain, detail::canonicalize_domain_storage(domain, current_storage));
}

ProblemTranslationResult translate(formalism::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options)
{
    const auto expected_domain = result.get_original_domain();
    const auto task_domain = task.get_domain();
    if (task_domain.get_index() != expected_domain.get_index() || &task_domain.get_context().get_root() != &expected_domain.get_context().get_root())
        throw MismatchedDomainError(std::string(expected_domain.get_name()), std::string(task_domain.get_name()));

    auto current_task = task;
    auto current_storage = std::shared_ptr<detail::TranslationStorage> {};
    auto phase_index = size_t { 1 };

    for (const auto& step : detail::task_phase_steps())
    {
        auto phase_storage = std::make_shared<detail::TranslationStorage>(phase_index++, &result.m_storage->repository);
        if (current_storage)
            detail::inherit_domain_identity_mappings(*phase_storage, *result.m_storage);
        else
            detail::inherit_domain_mappings(*phase_storage, *result.m_storage);

        auto semantic_copier = detail::CopyTranslator(phase_storage, options.remove_typing, step.phase);
        current_task = semantic_copier.copy_task(current_task);
        if (current_storage)
            detail::compose_storage_maps_from_previous(*phase_storage, *current_storage);
        current_storage = std::move(phase_storage);
    }

    const auto canonical = detail::canonicalize_problem_storage(current_task, current_storage, *result.m_storage);
    auto translated_task = canonical->tasks.at(task.get_index().get_value());
    return ProblemTranslationResult(task, canonical, translated_task);
}

}  // namespace loki::semantic
