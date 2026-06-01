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
    std::vector<ygg::IndexList<pddl::Parameter>> active_parameters;
    ygg::IndexList<pddl::Predicate> generated_predicates;
    ygg::IndexList<pddl::Axiom> generated_axioms;
    size_t num_generated_axioms = 0;
    bool append_generated_axioms_to_domain = true;
    cista::optional<ygg::Index<pddl::Predicate>> equality_predicate;
    std::unordered_map<ygg::uint_t, ygg::Index<pddl::Predicate>> type_predicates;
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
