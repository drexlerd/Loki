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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "../../../include/loki/domain/pddl/function_skeleton.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
FunctionSkeletonImpl::FunctionSkeletonImpl(int identifier, const std::string& name, const ParameterList& parameters, const Type& type)
    : Base(identifier)
    , m_name(name)
    , m_parameters(parameters)
    , m_type(type)
{
}

bool FunctionSkeletonImpl::are_equal_impl(const FunctionSkeletonImpl& other) const {
    return (m_name == other.m_name) && (m_parameters == other.m_parameters) && (m_type == other.m_type);
}

size_t FunctionSkeletonImpl::hash_impl() const {
    return hash_combine(m_name, hash_vector(m_parameters), m_type);
}

const std::string& FunctionSkeletonImpl::get_name() const {
    return m_name;
}

const ParameterList& FunctionSkeletonImpl::get_parameters() const {
    return m_parameters;
}

const Type& FunctionSkeletonImpl::get_type() const {
    return m_type;
}

}

namespace std {
    bool less<loki::pddl::FunctionSkeleton>::operator()(
        const loki::pddl::FunctionSkeleton& left_function,
        const loki::pddl::FunctionSkeleton& right_function) const {
        return *left_function < *right_function;
    }

    std::size_t hash<loki::pddl::FunctionSkeletonImpl>::operator()(const loki::pddl::FunctionSkeletonImpl& function) const {
        return function.hash_impl();
    }

}
