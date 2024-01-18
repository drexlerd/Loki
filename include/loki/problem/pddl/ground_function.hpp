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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_FUNCTION_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_FUNCTION_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {
class GroundFunctionImpl : public Base<GroundFunctionImpl> {
private:
    FunctionSkeleton m_function_skeleton;
    ObjectList m_objects;

    GroundFunctionImpl(int identifier, FunctionSkeleton function_skeleton, ObjectList objects);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const GroundFunctionImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundFunctionImpl>;

public:
    const FunctionSkeleton& get_function_skeleton() const;
    const ObjectList& get_objects() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundFunction>
    {
        bool operator()(const loki::pddl::GroundFunction& left_function, const loki::pddl::GroundFunction& right_function) const;
    };

    template<>
    struct hash<loki::pddl::GroundFunctionImpl>
    {
        std::size_t operator()(const loki::pddl::GroundFunctionImpl& function) const;
    };
}

#endif
