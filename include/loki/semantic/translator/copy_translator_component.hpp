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


#ifndef LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_COMPONENT_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_COMPONENT_HPP_

#include "loki/semantic/translator/copy_context.hpp"

namespace loki::semantic::detail
{

template<typename Derived, typename Component>
class CopyTranslatorComponent
{
protected:
    explicit CopyTranslatorComponent(CopyContext& context) :
        m_context(context),
        m_storage(context.storage),
        m_num_quantifications(context.num_quantifications),
        m_active_parameters(context.active_parameters),
        m_variable_bindings(context.variable_bindings),
        m_generated_predicates(context.generated_predicates),
        m_generated_axioms(context.generated_axioms),
        m_generated_universal_conditions(context.generated_universal_conditions),
        m_num_generated_axioms(context.num_generated_axioms),
        m_append_generated_axioms_to_domain(context.append_generated_axioms_to_domain),
        m_type_predicates(context.type_predicates),
        m_remove_typing(context.remove_typing),
        m_renaming_enabled(context.renaming_enabled),
        m_phase(context.phase)
    {
    }

    Derived& self() noexcept { return static_cast<Derived&>(static_cast<Component&>(*this)); }
    const Derived& self() const noexcept { return static_cast<const Derived&>(static_cast<const Component&>(*this)); }

    CopyContext& m_context;
    std::shared_ptr<TranslationStorage>& m_storage;
    std::unordered_map<ygg::uint_t, size_t>& m_num_quantifications;
    std::vector<ygg::IndexList<formalism::Parameter>>& m_active_parameters;
    std::vector<std::unordered_map<ygg::uint_t, formalism::VariableView>>& m_variable_bindings;
    ygg::IndexList<formalism::Predicate>& m_generated_predicates;
    ygg::IndexList<formalism::Axiom>& m_generated_axioms;
    std::unordered_map<std::string, formalism::ConditionView>& m_generated_universal_conditions;
    size_t& m_num_generated_axioms;
    bool& m_append_generated_axioms_to_domain;
    std::unordered_map<ygg::uint_t, formalism::PredicateView>& m_type_predicates;
    bool& m_remove_typing;
    bool& m_renaming_enabled;
    TranslationPhase& m_phase;
};

} // namespace loki::semantic::detail

#endif
