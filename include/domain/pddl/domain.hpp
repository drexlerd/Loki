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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DOMAIN_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {

class DomainImpl : public Base<DomainImpl> {
private:
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    PredicateList m_predicates;
    FunctionSkeletonList m_functions;
    ActionList m_actions;

    DomainImpl(int identifier,
        std::string name,
        Requirements requirements,
        TypeList types,
        ObjectList constants,
        PredicateList predicates,
        FunctionSkeletonList functions,
        ActionList actions);

    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for structural equivalence
    bool is_structurally_equivalent_to_impl(const DomainImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const TypeList& get_types() const;
    const ObjectList& get_constants() const;
    const PredicateList& get_predicates() const;
    const FunctionSkeletonList& get_functions() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Domain>
    {
        bool operator()(const loki::pddl::Domain& left_domain, const loki::pddl::Domain& right_domain) const;
    };

    template<>
    struct hash<loki::pddl::DomainImpl>
    {
        std::size_t operator()(const loki::pddl::DomainImpl& domain) const;
    };
}

#endif
