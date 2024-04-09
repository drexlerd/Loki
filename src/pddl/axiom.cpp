/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg and Simon Stahlberg
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

#include "loki/pddl/axiom.hpp"

#include "loki/pddl/atom.hpp"
#include "loki/pddl/conditions.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
AxiomImpl::AxiomImpl(int identifier, ParameterList parameters, Atom atom, Condition condition) :
    Base(identifier),
    m_parameters(std::move(parameters)),
    m_atom(std::move(atom)),
    m_condition(std::move(condition))
{
    // TODO (Dominik): must ensure that all parameters are used. Add assertion maybe?
}

bool AxiomImpl::is_structurally_equivalent_to_impl(const AxiomImpl& other) const
{
    return (m_parameters == other.m_parameters) && (m_atom == other.m_atom) && (m_condition == other.m_condition);
}

size_t AxiomImpl::hash_impl() const { return hash_combine(hash_container(m_parameters), m_atom, m_condition); }

void AxiomImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    // Not PDDL language specific. Our own definition of how we want to print it.
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(options.indent, ' ') << "(:axiom"
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

    out << std::string(nested_options.indent, ' ') << ":implies (" << *m_atom << ")\n";

    out << ")\n";
}

const ParameterList& AxiomImpl::get_parameters() const { return m_parameters; }

const Atom& AxiomImpl::get_atom() const { return m_atom; }

const Condition& AxiomImpl::get_condition() const { return m_condition; }

}
