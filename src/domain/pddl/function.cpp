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

#include "../../../include/loki/domain/pddl/function.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
FunctionImpl::FunctionImpl(int identifier, const FunctionSkeleton& function_skeleton, const TermList& terms)
    : Base(identifier)
    , m_function_skeleton(function_skeleton)
    , m_terms(terms)
{
}

bool FunctionImpl::are_equal_impl(const FunctionImpl& other) const {
    return (m_function_skeleton == other.m_function_skeleton) && (m_terms == other.m_terms);
}

size_t FunctionImpl::hash_impl() const {
    return hash_combine(m_function_skeleton, hash_vector(m_terms));
}

const FunctionSkeleton& FunctionImpl::get_function_skeleton() const {
    return m_function_skeleton;
}

const TermList& FunctionImpl::get_terms() const {
    return m_terms;
}

}

namespace std {
    bool less<loki::pddl::Function>::operator()(
        const loki::pddl::Function& left_function,
        const loki::pddl::Function& right_function) const {
        return *left_function < *right_function;
    }

    std::size_t hash<loki::pddl::FunctionImpl>::operator()(const loki::pddl::FunctionImpl& function) const {
        return function.hash_impl();
    }
}
