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

#include "loki/pddl/derived_predicate.hpp"

#include "loki/pddl/conditions.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/literal.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
DerivedPredicateImpl::DerivedPredicateImpl(int identifier, ParameterList parameters, Condition condition, Literal literal) :
    Base(identifier),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition)),
    m_literal(std::move(literal))
{
}

bool DerivedPredicateImpl::is_structurally_equivalent_to_impl(const DerivedPredicateImpl& other) const
{
    return (m_parameters == other.m_parameters) && (m_condition == other.m_condition) && (m_literal == other.m_literal);
}

size_t DerivedPredicateImpl::hash_impl() const { return hash_combine(hash_container(m_parameters), m_condition, m_literal); }

void DerivedPredicateImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(options.indent, ' ') << "(:derived"
        << "\n";

    out << std::string(nested_options.indent, ' ') << ":vars (";
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (i != 0)
            out << " ";
        m_parameters[i]->str(out, options);
    }
    out << ")\n";

    out << std::string(nested_options.indent, ' ') << ":context (";
    std::visit(StringifyVisitor(out, nested_options), *m_condition);
    out << ")\n";

    out << std::string(nested_options.indent, ' ') << ":implies (" << *m_literal << ")\n";

    out << ")\n";
}

const ParameterList& DerivedPredicateImpl::get_parameters() const { return m_parameters; }

const Condition& DerivedPredicateImpl::get_condition() const { return m_condition; }

const Literal& DerivedPredicateImpl::get_literal() const { return m_literal; }

}
