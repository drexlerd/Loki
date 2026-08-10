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

#include "loki/formalism/builder.hpp"
#include "loki/semantic/translator/common.hpp"

#include <yggdrasil/containers/unordered_set.hpp>

namespace loki::semantic::detail
{

using ScopedFreeParameter = std::pair<formalism::ParameterView, formalism::VariableView>;
using GeneratedUniversalConditionKey = std::pair<std::vector<ScopedFreeParameter>, formalism::ConditionView>;

struct CopyContext
{
    formalism::Builder builder;
    std::shared_ptr<TranslationStorage> storage;
    ygg::UnorderedMap<formalism::VariableView, size_t> num_quantifications;
    std::vector<std::vector<formalism::ParameterView>> active_parameters;
    std::vector<std::vector<formalism::VariableView>> active_parameter_variables;
    std::vector<ygg::UnorderedMap<formalism::VariableView, formalism::VariableView>> variable_bindings;
    ygg::UnorderedSet<std::string> renamed_variable_names;
    ygg::UnorderedSet<std::string> used_predicate_names;
    std::vector<formalism::PredicateView> generated_predicates;
    std::vector<formalism::AxiomView> generated_axioms;
    ygg::UnorderedMap<GeneratedUniversalConditionKey, formalism::ConditionView> generated_universal_conditions;
    size_t num_generated_axioms = 0;
    ygg::UnorderedMap<formalism::TypeView, formalism::PredicateView> type_predicates;
    bool compile_typing = true;
    bool renaming_enabled = true;
    TranslationPhase phase = TranslationPhase::ToNegationNormalForm;
};

class CopyContextOwner
{
public:
    explicit CopyContextOwner(std::shared_ptr<TranslationStorage> storage, bool compile_typing, TranslationPhase phase)
    {
        m_context.storage = std::move(storage);
        m_context.compile_typing = compile_typing;
        m_context.phase = phase;
    }

    CopyContext& context() noexcept { return m_context; }
    const CopyContext& context() const noexcept { return m_context; }

private:
    CopyContext m_context;
};

}  // namespace loki::semantic::detail

#endif
