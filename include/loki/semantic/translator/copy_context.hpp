/*
 * Copyright (C) 2026 Dominik Drexler
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
    std::vector<std::unordered_map<ygg::uint_t, ygg::Index<formalism::Variable>>> variable_bindings;
    ygg::IndexList<formalism::Predicate> generated_predicates;
    ygg::IndexList<formalism::Axiom> generated_axioms;
    std::unordered_map<ygg::uint_t, ygg::Index<formalism::Condition>> generated_axiom_conditions;
    std::unordered_map<ygg::uint_t, ygg::Index<formalism::Condition>> generated_positive_conditions;
    size_t num_generated_axioms = 0;
    bool append_generated_axioms_to_domain = true;
    cista::optional<ygg::Index<formalism::Predicate>> equality_predicate;
    std::unordered_map<ygg::uint_t, ygg::Index<formalism::Predicate>> type_predicates;
    bool remove_typing = true;
    bool renaming_enabled = true;
};

class CopyContextOwner
{
public:
    explicit CopyContextOwner(std::shared_ptr<TranslationStorage> storage, bool remove_typing)
    {
        m_context.storage = std::move(storage);
        m_context.remove_typing = remove_typing;
    }

    CopyContext& context() noexcept { return m_context; }
    const CopyContext& context() const noexcept { return m_context; }

private:
    CopyContext m_context;
};

} // namespace loki::semantic::detail

#endif
