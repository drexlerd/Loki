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

#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{
VariableImpl::VariableImpl(size_t index, std::string name) : Base(index), m_name(std::move(name)) {}

void VariableImpl::str_impl(std::ostream& out, const FormattingOptions& /*options*/) const { out << m_name; }

const std::string& VariableImpl::get_name() const { return m_name; }

size_t ShallowHash<const VariableImpl*>::operator()(const VariableImpl* e) const { return ShallowHashCombiner()(e->get_name()); }

bool ShallowEqualTo<const VariableImpl*>::operator()(const VariableImpl* l, const VariableImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name());
    }
    return true;
}

static void collect_free_variables_recursively(const loki::ConditionImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    if (const auto condition_literal = std::get_if<loki::ConditionLiteralImpl>(&condition))
    {
        for (const auto& term : condition_literal->get_literal()->get_atom()->get_terms())
        {
            if (const auto term_variable = std::get_if<loki::TermVariableImpl>(term))
            {
                if (!ref_quantified_variables.count(term_variable->get_variable()))
                {
                    ref_free_variables.insert(term_variable->get_variable());
                }
            }
        }
    }
    else if (const auto condition_imply = std::get_if<loki::ConditionImplyImpl>(&condition))
    {
        collect_free_variables_recursively(*condition_imply->get_condition_left(), ref_quantified_variables, ref_free_variables);
        collect_free_variables_recursively(*condition_imply->get_condition_right(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_not = std::get_if<loki::ConditionNotImpl>(&condition))
    {
        collect_free_variables_recursively(*condition_not->get_condition(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_and = std::get_if<loki::ConditionAndImpl>(&condition))
    {
        for (const auto& part : condition_and->get_conditions())
        {
            collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
        }
    }
    else if (const auto condition_or = std::get_if<loki::ConditionOrImpl>(&condition))
    {
        for (const auto& part : condition_or->get_conditions())
        {
            collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
        }
    }
    else if (const auto condition_exists = std::get_if<loki::ConditionExistsImpl>(&condition))
    {
        for (const auto& parameter : condition_exists->get_parameters())
        {
            ref_quantified_variables.insert(parameter->get_variable());
        }
        collect_free_variables_recursively(*condition_exists->get_condition(), ref_quantified_variables, ref_free_variables);
    }
    else if (const auto condition_forall = std::get_if<loki::ConditionForallImpl>(&condition))
    {
        for (const auto& parameter : condition_forall->get_parameters())
        {
            ref_quantified_variables.insert(parameter->get_variable());
        }
        collect_free_variables_recursively(*condition_forall->get_condition(), ref_quantified_variables, ref_free_variables);
    }
}

VariableSet collect_free_variables(const ConditionImpl& condition)
{
    auto quantified_variables = VariableSet {};
    auto free_variables = VariableSet {};

    collect_free_variables_recursively(condition, quantified_variables, free_variables);

    return free_variables;
}

}
