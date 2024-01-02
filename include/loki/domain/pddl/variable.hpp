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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_VARIABLE_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_VARIABLE_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
class VariableImpl : public Base<VariableImpl> {
private:
    std::string m_name;

    VariableImpl(int identifier, std::string name);

    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const VariableImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Variable>
    {
        bool operator()(const loki::pddl::Variable& left_variable, const loki::pddl::Variable& right_variable) const;
    };

    template<>
    struct hash<loki::pddl::VariableImpl>
    {
        std::size_t operator()(const loki::pddl::VariableImpl& variable) const;
    };
}


#endif
