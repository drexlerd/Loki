/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_SRC_PDDL_TRANSLATOR_REMOVE_UNIVERSAL_QUANTIFIERS_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_REMOVE_UNIVERSAL_QUANTIFIERS_HPP_

#include "recursive_cached_base.hpp"

#include <deque>

namespace loki
{

/**
 * Compile away universal quantifiers by introducing axioms using the equivalence
 * forall(vars, phi) == not exists(vars, not phi)
 */
class RemoveUniversalQuantifiersTranslator : public RecursiveCachedBaseTranslator<RemoveUniversalQuantifiersTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<RemoveUniversalQuantifiersTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    class Scope
    {
    private:
        std::unordered_map<Variable, Parameter> m_variable_to_parameter;

        const Scope* m_parent_scope;

    public:
        Scope(std::unordered_map<Variable, Parameter> variable_to_parameter, const Scope* parent_scope = nullptr);

        std::optional<Parameter> get_parameter(Variable variable) const;
    };

    class ScopeStack
    {
    private:
        std::deque<std::unique_ptr<Scope>> m_stack;

    public:
        const Scope& open_scope(const ParameterList& parameters);

        void close_scope();

        const Scope& get() const;
    };

    ScopeStack m_scopes;

    // Track simple and derived predicate names to give unique names
    std::unordered_set<std::string> m_existing_predicate_names;
    size_t m_next_axiom_index;

    // Translation might introduce additional derived predicates and axioms.
    std::unordered_set<Predicate> m_instantiated_predicates;
    std::unordered_set<Axiom> m_instantiated_axioms;

    /// @brief Translate the condition while keeping track of scopes
    Condition translate_level_2(ConditionExists condition, Repositories& repositories);
    Condition translate_level_2(ConditionForall condition, Repositories& repositories);
    Action translate_level_2(Action action, Repositories& repositories);
    Axiom translate_level_2(Axiom axiom, Repositories& repositories);

    Domain translate_level_2(const Domain& domain, DomainBuilder& builder);
    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};
}

#endif