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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_SKELETON_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_FUNCTION_SKELETON_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
class FunctionSkeletonImpl : public Base<FunctionSkeletonImpl> {
private:
    std::string m_name;
    ParameterList m_parameters;
    Type m_type;

    FunctionSkeletonImpl(int identifier, std::string name, ParameterList parameters, Type type);

    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const FunctionSkeletonImpl& other) const;
    size_t hash_impl() const;
    /// @brief Returns a string representation where typing is assumed to be true.
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;
    /// @brief Returns a parseable string representation in the context of a domain.
    void str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const Type& get_type() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::FunctionSkeleton>
    {
        bool operator()(const loki::pddl::FunctionSkeleton& left_function, const loki::pddl::FunctionSkeleton& right_function) const;
    };

    template<>
    struct hash<loki::pddl::FunctionSkeletonImpl>
    {
        std::size_t operator()(const loki::pddl::FunctionSkeletonImpl& function) const;
    };
}

#endif
