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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DERIVED_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DERIVED_PREDICATE_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <functional>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
class DerivedPredicateImpl : public Base<DerivedPredicateImpl> {
private:
    ParameterList m_parameters;
    Condition m_condition;

    DerivedPredicateImpl(int identifier, ParameterList parameters, Condition condition);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    /// @brief Test for structural equivalence
    bool is_structurally_equivalent_to_impl(const DerivedPredicateImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<DerivedPredicateImpl>;

public:
    const ParameterList& get_parameters() const;
    const Condition& get_condition() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::DerivedPredicate>
    {
        bool operator()(const loki::pddl::DerivedPredicate& left_predicate, const loki::pddl::DerivedPredicate& right_predicate) const;
    };

    template<>
    struct hash<loki::pddl::DerivedPredicateImpl>
    {
        std::size_t operator()(const loki::pddl::DerivedPredicateImpl& action) const;
    };
}


#endif
