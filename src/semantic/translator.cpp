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

DomainTranslationResult::DomainTranslationResult(formalism::DomainView original_domain_, std::shared_ptr<detail::TranslationStorage> storage_) :
    m_original_domain(original_domain_),
    m_storage(std::move(storage_))
{
}

formalism::DomainView DomainTranslationResult::get_original_domain() const noexcept { return m_original_domain; }

formalism::DomainView DomainTranslationResult::get_translated_domain() const noexcept { return ygg::make_view(m_storage->translated_domain, m_storage->repository); }

const formalism::Repository& DomainTranslationResult::get_repository() const noexcept { return m_storage->repository; }

formalism::Repository& DomainTranslationResult::get_repository() noexcept { return m_storage->repository; }

ProblemTranslationResult::ProblemTranslationResult(formalism::TaskView original_task_, std::shared_ptr<detail::TranslationStorage> storage_, ygg::Index<formalism::Task> translated_task_) :
    m_original_task(original_task_),
    m_storage(std::move(storage_)),
    m_translated_task(translated_task_)
{
}

formalism::TaskView ProblemTranslationResult::get_original_task() const noexcept { return m_original_task; }

formalism::TaskView ProblemTranslationResult::get_translated_task() const noexcept { return ygg::make_view(m_translated_task, m_storage->repository); }

const formalism::Repository& ProblemTranslationResult::get_repository() const noexcept { return m_storage->repository; }

formalism::Repository& ProblemTranslationResult::get_repository() noexcept { return m_storage->repository; }

DomainTranslationResult translate(formalism::DomainView domain, const TranslatorOptions& options)
{
    auto middle = std::make_shared<detail::TranslationStorage>(1);
    auto semantic_copier = detail::CopyTranslator(middle, options.remove_typing);
    for ([[maybe_unused]] auto step : domain_translation_steps())
    {
    }
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
    for ([[maybe_unused]] auto step : task_translation_steps())
    {
    }
    const auto middle_task = semantic_copier.copy_task(task);
    const auto canonical = detail::canonicalize_problem_storage(middle_task, middle, *result.m_storage);
    return ProblemTranslationResult(task, canonical, canonical->tasks.at(middle_task.get_index().get_value()));
}

} // namespace loki::semantic
