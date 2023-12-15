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

#include "../../../include/loki/problem/pddl/numeric_fluent.hpp"

#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/domain/pddl/function.hpp"


namespace loki::pddl {
NumericFluentImpl::NumericFluentImpl(int identifier, const Function& function, double number)
    : Base(identifier), m_function(function), m_number(number) { }

bool NumericFluentImpl::are_equal_impl(const NumericFluentImpl& other) const {
    return (m_function == other.m_function)
        && (m_number == other.m_number);
}

size_t NumericFluentImpl::hash_impl() const {
    return hash_combine(m_function, m_number);
}

void NumericFluentImpl::str_impl(std::ostringstream& out, const FormattingOptions& /*options*/) const {
    out << "(= " << *m_function << " " << m_number << ")";
}

const Function& NumericFluentImpl::get_function() const {
    return m_function;
}

double NumericFluentImpl::get_number() const {
    return m_number;
}

}



namespace std {
    bool less<loki::pddl::NumericFluent>::operator()(
        const loki::pddl::NumericFluent& left_numeric_fluent,
        const loki::pddl::NumericFluent& right_numeric_fluent) const {
        return *left_numeric_fluent < *right_numeric_fluent;
    }

    std::size_t hash<loki::pddl::NumericFluentImpl>::operator()(const loki::pddl::NumericFluentImpl& numeric_fluent) const {
        return numeric_fluent.hash_impl();
    }
}