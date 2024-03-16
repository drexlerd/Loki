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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FUNCTION_SKELETON_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FUNCTION_SKELETON_HPP_

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"

#include <string>

namespace loki
{
template<typename HolderType>
class PersistentFactory;
}

namespace loki::pddl
{
class FunctionSkeletonImpl : public Base<FunctionSkeletonImpl>
{
private:
    std::string m_name;
    ParameterList m_parameters;
    Type m_type;

    FunctionSkeletonImpl(int identifier, std::string name, ParameterList parameters, Type type);

    // Give access to the constructor.
    template<typename>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const FunctionSkeletonImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<FunctionSkeletonImpl>;

public:
    /// @brief Returns a parseable string representation in the context of a domain.
    void str(std::ostringstream& out, const FormattingOptions& options, bool typing_enabled) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const Type& get_type() const;
};

}

namespace std
{
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