/*
 * Copyright (C) 2026 Dominik Drexler
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
        m_generated_predicates(context.generated_predicates),
        m_generated_axioms(context.generated_axioms),
        m_num_generated_axioms(context.num_generated_axioms),
        m_append_generated_axioms_to_domain(context.append_generated_axioms_to_domain),
        m_equality_predicate(context.equality_predicate),
        m_type_predicates(context.type_predicates),
        m_remove_typing(context.remove_typing),
        m_renaming_enabled(context.renaming_enabled)
    {
    }

    Derived& self() noexcept { return static_cast<Derived&>(static_cast<Component&>(*this)); }
    const Derived& self() const noexcept { return static_cast<const Derived&>(static_cast<const Component&>(*this)); }

    CopyContext& m_context;
    std::shared_ptr<TranslationStorage>& m_storage;
    std::unordered_map<ygg::uint_t, size_t>& m_num_quantifications;
    std::vector<ygg::IndexList<formalism::Parameter>>& m_active_parameters;
    ygg::IndexList<formalism::Predicate>& m_generated_predicates;
    ygg::IndexList<formalism::Axiom>& m_generated_axioms;
    size_t& m_num_generated_axioms;
    bool& m_append_generated_axioms_to_domain;
    cista::optional<ygg::Index<formalism::Predicate>>& m_equality_predicate;
    std::unordered_map<ygg::uint_t, ygg::Index<formalism::Predicate>>& m_type_predicates;
    bool& m_remove_typing;
    bool& m_renaming_enabled;
};

} // namespace loki::semantic::detail

#endif
