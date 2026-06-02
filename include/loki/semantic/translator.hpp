/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_HPP_

#include "loki/semantic/translator/canonical_copy_translator.hpp"
#include "loki/semantic/translator/copy_translator.hpp"

namespace loki::semantic
{

class ProblemTranslationResult;

namespace detail
{

std::shared_ptr<TranslationStorage> canonicalize_domain_storage(formalism::DomainView original_domain, const std::shared_ptr<TranslationStorage>& middle);
void inherit_domain_mappings(TranslationStorage& problem, const TranslationStorage& domain);
void inherit_domain_identity_mappings(TranslationStorage& problem, const TranslationStorage& domain);
std::shared_ptr<TranslationStorage>
canonicalize_problem_storage(formalism::TaskView middle_task, const std::shared_ptr<TranslationStorage>& middle, const TranslationStorage& domain);

}  // namespace detail

class DomainTranslationResult
{
public:
    DomainTranslationResult(formalism::DomainView original_domain_, std::shared_ptr<detail::TranslationStorage> storage_);

    formalism::DomainView get_original_domain() const noexcept;
    formalism::DomainView get_translated_domain() const noexcept;
    const formalism::Repository& get_repository() const noexcept;
    formalism::Repository& get_repository() noexcept;

private:
    friend class ProblemTranslationResult;
    friend ProblemTranslationResult translate(formalism::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options);

    formalism::DomainView m_original_domain;
    std::shared_ptr<detail::TranslationStorage> m_storage;
};

class ProblemTranslationResult
{
public:
    ProblemTranslationResult(formalism::TaskView original_task_, std::shared_ptr<detail::TranslationStorage> storage_, formalism::TaskView translated_task_);

    formalism::TaskView get_original_task() const noexcept;
    formalism::TaskView get_translated_task() const noexcept;
    const formalism::Repository& get_repository() const noexcept;
    formalism::Repository& get_repository() noexcept;

private:
    formalism::TaskView m_original_task;
    std::shared_ptr<detail::TranslationStorage> m_storage;
    formalism::TaskView m_translated_task;
};

DomainTranslationResult translate(formalism::DomainView domain, const TranslatorOptions& options = {});
ProblemTranslationResult translate(formalism::TaskView task, const DomainTranslationResult& result, const TranslatorOptions& options = {});

}  // namespace loki::semantic

#endif
