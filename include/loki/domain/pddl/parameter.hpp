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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARAMETER_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class PersistentFactory;
}


namespace loki::pddl {

class ParameterImpl : public Base<ParameterImpl> {
private:
    Variable m_variable;
    TypeList m_types;

    ParameterImpl(int identifier, const Variable& variable, const TypeList& types);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for semantic equivalence
    bool are_equal_impl(const ParameterImpl& other) const;
    size_t hash_impl() const;
    /// @brief Returns a string representation where typing is assumed to be true.
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;
    /// @brief Returns a parseable string representation in the context of a domain.
    void str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const;

    const Variable& get_variable() const;
    const TypeList& get_bases() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Parameter>
    {
        bool operator()(const loki::pddl::Parameter& left_parameter, const loki::pddl::Parameter& right_parameter) const;
    };

    template<>
    struct hash<loki::pddl::ParameterImpl>
    {
        std::size_t operator()(const loki::pddl::ParameterImpl& parameter) const;
    };
}

#endif
