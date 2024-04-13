/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DOMAIN_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DOMAIN_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/factory.hpp"
#include "loki/pddl/requirements.hpp"

#include <string>

namespace loki
{

class DomainImpl : public Base<DomainImpl>
{
private:
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    PredicateList m_predicates;
    PredicateList m_derived_predicates;
    FunctionSkeletonList m_functions;
    ActionList m_actions;
    AxiomList m_axioms;

    DomainImpl(int identifier,
               std::string name,
               Requirements requirements,
               TypeList types,
               ObjectList constants,
               PredicateList predicates,
               PredicateList derived_predicates,
               FunctionSkeletonList functions,
               ActionList actions,
               AxiomList axioms);

    // Give access to the constructor.
    friend class PDDLFactory<DomainImpl, Hash<DomainImpl*>, EqualTo<DomainImpl*>>;

    /// @brief Test for structural equivalence
    bool is_structurally_equivalent_to_impl(const DomainImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<DomainImpl>;

public:
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const TypeList& get_types() const;
    const ObjectList& get_constants() const;
    const PredicateList& get_predicates() const;
    const PredicateList& get_derived_predicates() const;
    const FunctionSkeletonList& get_functions() const;
    const ActionList& get_actions() const;
    const AxiomList& get_axioms() const;
};

}

#endif
