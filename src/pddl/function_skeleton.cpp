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

#include "loki/details/pddl/function_skeleton.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/parameter.hpp"

namespace loki
{
FunctionSkeletonImpl::FunctionSkeletonImpl(size_t index, std::string name, ParameterList parameters, Type type) :
    m_index(index),
    m_name(std::move(name)),
    m_parameters(parameters),
    m_type(std::move(type))
{
}

size_t FunctionSkeletonImpl::get_index() const { return m_index; }

const std::string& FunctionSkeletonImpl::get_name() const { return m_name; }

const ParameterList& FunctionSkeletonImpl::get_parameters() const { return m_parameters; }

Type FunctionSkeletonImpl::get_type() const { return m_type; }

std::ostream& operator<<(std::ostream& out, const FunctionSkeletonImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, FunctionSkeleton element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

}
