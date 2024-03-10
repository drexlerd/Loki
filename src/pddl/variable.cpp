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

#include "loki/pddl/variable.hpp"

#include "loki/collections.hpp"
#include "loki/hash.hpp"

namespace loki::pddl
{
VariableImpl::VariableImpl(int identifier, std::string name) : Base(identifier), m_name(std::move(name)) {}

bool VariableImpl::is_structurally_equivalent_to_impl(const VariableImpl& other) const { return (m_name == other.m_name); }

size_t VariableImpl::hash_impl() const { return hash_combine(m_name); }

void VariableImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const { out << m_name; }

const std::string& VariableImpl::get_name() const { return m_name; }

}

namespace std
{
bool less<loki::pddl::Variable>::operator()(const loki::pddl::Variable& left_variable, const loki::pddl::Variable& right_variable) const
{
    return *left_variable < *right_variable;
}

std::size_t hash<loki::pddl::VariableImpl>::operator()(const loki::pddl::VariableImpl& variable) const { return variable.hash(); }
}
