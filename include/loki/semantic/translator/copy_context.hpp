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


#ifndef LOKI_SEMANTIC_TRANSLATOR_COPY_CONTEXT_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COPY_CONTEXT_HPP_

#include "loki/semantic/translator/common.hpp"

namespace loki::semantic::detail
{

struct CopyContext
{
    std::shared_ptr<TranslationStorage> storage;
    std::unordered_map<ygg::uint_t, size_t> num_quantifications;
    std::vector<ygg::IndexList<formalism::Parameter>> active_parameters;
    std::vector<std::unordered_map<ygg::uint_t, formalism::VariableView>> variable_bindings;
    ygg::IndexList<formalism::Predicate> generated_predicates;
    ygg::IndexList<formalism::Axiom> generated_axioms;
    std::unordered_map<std::string, formalism::ConditionView> generated_universal_conditions;
    size_t num_generated_axioms = 0;
    bool append_generated_axioms_to_domain = true;
    std::unordered_map<ygg::uint_t, formalism::PredicateView> type_predicates;
    bool remove_typing = true;
    bool renaming_enabled = true;
    TranslationPhase phase = TranslationPhase::ToNegationNormalForm;
};

class CopyContextOwner
{
public:
    explicit CopyContextOwner(std::shared_ptr<TranslationStorage> storage, bool remove_typing, TranslationPhase phase)
    {
        m_context.storage = std::move(storage);
        m_context.remove_typing = remove_typing;
        m_context.phase = phase;
    }

    CopyContext& context() noexcept { return m_context; }
    const CopyContext& context() const noexcept { return m_context; }

private:
    CopyContext m_context;
};

} // namespace loki::semantic::detail

#endif
