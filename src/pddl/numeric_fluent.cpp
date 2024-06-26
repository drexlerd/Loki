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

#include "loki/details/pddl/numeric_fluent.hpp"

#include "loki/details/pddl/function.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
NumericFluentImpl::NumericFluentImpl(size_t identifier, Function function, double number) : Base(identifier), m_function(std::move(function)), m_number(number)
{
}

bool NumericFluentImpl::is_structurally_equivalent_to_impl(const NumericFluentImpl& other) const
{
    if (this != &other)
    {
        return (m_function == other.m_function) && (m_number == other.m_number);
    }
    return true;
}

size_t NumericFluentImpl::hash_impl() const { return hash_combine(m_function, m_number); }

void NumericFluentImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(= ";
    m_function->str(out, options);
    out << " " << m_number << ")";
}

const Function& NumericFluentImpl::get_function() const { return m_function; }

double NumericFluentImpl::get_number() const { return m_number; }

}
