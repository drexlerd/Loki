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

#include "loki/pddl/action.hpp"

#include "loki/pddl/conditions.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
ActionImpl::ActionImpl(int identifier, std::string name, ParameterList parameters, std::optional<Condition> condition, std::optional<Effect> effect) :
    Base(identifier),
    m_name(std::move(name)),
    m_parameters(std::move(parameters)),
    m_condition(std::move(condition)),
    m_effect(std::move(effect))
{
}

bool ActionImpl::is_structurally_equivalent_to_impl(const ActionImpl& other) const
{
    return (m_name == other.m_name) && (get_sorted_vector(m_parameters) == get_sorted_vector(other.m_parameters)) && (*m_condition == *other.m_condition)
           && (*m_effect == *other.m_effect);
}

size_t ActionImpl::hash_impl() const { return hash_combine(m_name, hash_container(m_parameters), *m_condition, *m_effect); }

void ActionImpl::str(std::ostream& out, const FormattingOptions& options, bool typing_enabled) const
{
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(options.indent, ' ') << "(action " << m_name << "\n" << std::string(nested_options.indent, ' ') << ":parameters (";
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        if (i != 0)
            out << " ";
        m_parameters[i]->str(out, options, typing_enabled);
    }
    out << ")";
    out << "\n";
    out << std::string(nested_options.indent, ' ') << ":conditions ";
    if (m_condition.has_value())
        std::visit(StringifyVisitor(out, nested_options, typing_enabled), *m_condition.value());
    else
        out << "()";

    out << "\n";
    out << std::string(nested_options.indent, ' ') << ":effects ";
    if (m_effect.has_value())
        std::visit(StringifyVisitor(out, nested_options, typing_enabled), *m_effect.value());
    else
        out << "()";

    out << ")\n";
}

const std::string& ActionImpl::get_name() const { return m_name; }

const ParameterList& ActionImpl::get_parameters() const { return m_parameters; }

const std::optional<Condition>& ActionImpl::get_condition() const { return m_condition; }

const std::optional<Effect>& ActionImpl::get_effect() const { return m_effect; }

}
