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

#include "loki/details/pddl/axiom.hpp"

#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/visitors.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
AxiomImpl::AxiomImpl(int identifier, Literal literal, Condition condition) : Base(identifier), m_literal(std::move(literal)), m_condition(std::move(condition))
{
}

bool AxiomImpl::is_structurally_equivalent_to_impl(const AxiomImpl& other) const
{
    return (m_literal == other.m_literal) && (m_condition == other.m_condition);
}

size_t AxiomImpl::hash_impl() const { return hash_combine(m_literal, m_condition); }

void AxiomImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(options.indent, ' ') << "(:derived " << *m_literal << "\n";
    out << std::string(nested_options.indent, ' ');
    std::visit(StringifyVisitor(out, nested_options), *m_condition);
    out << ")\n";
}

const Literal& AxiomImpl::get_literal() const { return m_literal; }

const Condition& AxiomImpl::get_condition() const { return m_condition; }

}
