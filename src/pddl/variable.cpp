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

#include "loki/details/pddl/variable.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/term.hpp"

namespace loki
{
VariableImpl::VariableImpl(size_t index, std::string name) : m_index(index), m_name(std::move(name)) {}

size_t VariableImpl::get_index() const { return m_index; }

const std::string& VariableImpl::get_name() const { return m_name; }

static void collect_free_variables_recursively(const loki::ConditionImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    if (const auto condition_literal = std::get_if<loki::ConditionLiteral>(&condition.get_condition()))
    {
        for (const auto& term : (*condition_literal)->get_literal()->get_atom()->get_terms())
        {
            if (const auto variable = std::get_if<Variable>(&term->get_object_or_variable()))
            {
                if (!ref_quantified_variables.count(*variable))
                {
                    ref_free_variables.insert(*variable);
                }
            }
        }
    }
    else if (const auto condition_imply = std::get_if<loki::ConditionImply>(&condition.get_condition()))
    {
        collect_free_variables_recursively(*(*condition_imply)->get_condition_left(), ref_quantified_variables, ref_free_variables);
        collect_free_variables_recursively(*(*condition_imply)->get_condition_right(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_not = std::get_if<loki::ConditionNot>(&condition.get_condition()))
    {
        collect_free_variables_recursively(*(*condition_not)->get_condition(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_and = std::get_if<loki::ConditionAnd>(&condition.get_condition()))
    {
        for (const auto& part : (*condition_and)->get_conditions())
        {
            collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
        }
    }
    else if (const auto condition_or = std::get_if<loki::ConditionOr>(&condition.get_condition()))
    {
        for (const auto& part : (*condition_or)->get_conditions())
        {
            collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
        }
    }
    else if (const auto condition_exists = std::get_if<loki::ConditionExists>(&condition.get_condition()))
    {
        for (const auto& parameter : (*condition_exists)->get_parameters())
        {
            ref_quantified_variables.insert(parameter->get_variable());
        }
        collect_free_variables_recursively(*(*condition_exists)->get_condition(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_forall = std::get_if<loki::ConditionForall>(&condition.get_condition()))
    {
        for (const auto& parameter : (*condition_forall)->get_parameters())
        {
            ref_quantified_variables.insert(parameter->get_variable());
        }
        collect_free_variables_recursively(*(*condition_forall)->get_condition(), ref_quantified_variables, ref_free_variables);
    }
}

VariableSet collect_free_variables(const ConditionImpl& condition)
{
    auto quantified_variables = VariableSet {};
    auto free_variables = VariableSet {};

    collect_free_variables_recursively(condition, quantified_variables, free_variables);

    return free_variables;
}

std::ostream& operator<<(std::ostream& out, const VariableImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
