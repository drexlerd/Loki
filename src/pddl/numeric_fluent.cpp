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

namespace loki
{
NumericFluentImpl::NumericFluentImpl(size_t index, Function function, double number) : Base(index), m_function(std::move(function)), m_number(number) {}

void NumericFluentImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(= ";
    m_function->str(out, options);
    out << " " << m_number << ")";
}

const Function& NumericFluentImpl::get_function() const { return m_function; }

double NumericFluentImpl::get_number() const { return m_number; }

}
