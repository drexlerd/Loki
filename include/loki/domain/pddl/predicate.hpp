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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PREDICATE_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType>
class PersistentFactory;
}


namespace loki::pddl {

// An alternative name is AtomSkeleton
class PredicateImpl : public Base<PredicateImpl> {
private:
    std::string m_name;
    ParameterList m_parameters;

    PredicateImpl(int identifier, std::string name, ParameterList parameters);

    template<typename HolderType>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const PredicateImpl& other) const;
    size_t hash_impl() const;
    /// @brief Returns a string representation where typing is assumed to be true.
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;
    /// @brief Returns a parseable string representation in the context of a domain.
    void str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Predicate>
    {
        bool operator()(const loki::pddl::Predicate& left_predicate, const loki::pddl::Predicate& right_predicate) const;
    };

    template<>
    struct hash<loki::pddl::PredicateImpl>
    {
        std::size_t operator()(const loki::pddl::PredicateImpl& predicate) const;
    };
}

#endif
