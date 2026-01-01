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

/**
 * Forward declarations
 */

static void collect_free_variables_recursively(const loki::FunctionImpl& function, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::FunctionExpressionNumberImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::FunctionExpressionBinaryOperatorImpl& fexpr,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::FunctionExpressionMultiOperatorImpl& fexpr,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::FunctionExpressionMinusImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::FunctionExpressionFunctionImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::FunctionExpressionImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::ConditionLiteralImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::ConditionImplyImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::ConditionNotImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::ConditionAndImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::ConditionOrImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::ConditionExistsImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void
collect_free_variables_recursively(const loki::ConditionForallImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::ConditionNumericConstraintImpl& condition,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables);

static void collect_free_variables_recursively(const loki::ConditionImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables);

/**
 * Definitions
 */

static void collect_free_variables_recursively(const loki::FunctionImpl& function, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& term : function.get_terms())
    {
        std::visit(
            [&](auto&& arg)
            {
                using Variant = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Variant, Variable>)
                {
                    if (!ref_quantified_variables.count(arg))
                    {
                        ref_free_variables.insert(arg);
                    }
                }
            },
            term->get_object_or_variable());
    }
}

static void collect_free_variables_recursively(const loki::FunctionExpressionNumberImpl&, VariableSet&, VariableSet&) {}

static void collect_free_variables_recursively(const loki::FunctionExpressionBinaryOperatorImpl& fexpr,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*fexpr.get_left_function_expression(), ref_quantified_variables, ref_free_variables);
    collect_free_variables_recursively(*fexpr.get_right_function_expression(), ref_quantified_variables, ref_free_variables);
}

static void collect_free_variables_recursively(const loki::FunctionExpressionMultiOperatorImpl& fexpr,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables)
{
    for (const auto& part : fexpr.get_function_expressions())
    {
        collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
    }
}

static void
collect_free_variables_recursively(const loki::FunctionExpressionMinusImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*fexpr.get_function_expression(), ref_quantified_variables, ref_free_variables);
}

static void
collect_free_variables_recursively(const loki::FunctionExpressionFunctionImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*fexpr.get_function(), ref_quantified_variables, ref_free_variables);
}

static void
collect_free_variables_recursively(const loki::FunctionExpressionImpl& fexpr, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    std::visit([&](auto&& arg) { collect_free_variables_recursively(*arg, ref_quantified_variables, ref_free_variables); }, fexpr.get_function_expression());
}

static void
collect_free_variables_recursively(const loki::ConditionLiteralImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& term : condition.get_literal()->get_atom()->get_terms())
    {
        std::visit(
            [&](auto&& arg)
            {
                using Variant = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Variant, Variable>)
                {
                    if (!ref_quantified_variables.count(arg))
                    {
                        ref_free_variables.insert(arg);
                    }
                }
            },
            term->get_object_or_variable());
    }
}

static void
collect_free_variables_recursively(const loki::ConditionImplyImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*condition.get_left_condition(), ref_quantified_variables, ref_free_variables);
    collect_free_variables_recursively(*condition.get_right_condition(), ref_quantified_variables, ref_free_variables);
}

static void collect_free_variables_recursively(const loki::ConditionNotImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*condition.get_condition(), ref_quantified_variables, ref_free_variables);
}

static void collect_free_variables_recursively(const loki::ConditionAndImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& part : condition.get_conditions())
    {
        collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
    }
}

static void collect_free_variables_recursively(const loki::ConditionOrImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& part : condition.get_conditions())
    {
        collect_free_variables_recursively(*part, ref_quantified_variables, ref_free_variables);
    }
}

static void
collect_free_variables_recursively(const loki::ConditionExistsImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& parameter : condition.get_parameters())
    {
        ref_quantified_variables.insert(parameter->get_variable());
    }
    collect_free_variables_recursively(*condition.get_condition(), ref_quantified_variables, ref_free_variables);
}

static void
collect_free_variables_recursively(const loki::ConditionForallImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    for (const auto& parameter : condition.get_parameters())
    {
        ref_quantified_variables.insert(parameter->get_variable());
    }
    collect_free_variables_recursively(*condition.get_condition(), ref_quantified_variables, ref_free_variables);
}

static void collect_free_variables_recursively(const loki::ConditionNumericConstraintImpl& condition,
                                               VariableSet& ref_quantified_variables,
                                               VariableSet& ref_free_variables)
{
    collect_free_variables_recursively(*condition.get_left_function_expression(), ref_quantified_variables, ref_free_variables);
    collect_free_variables_recursively(*condition.get_right_function_expression(), ref_quantified_variables, ref_free_variables);
}

static void collect_free_variables_recursively(const loki::ConditionImpl& condition, VariableSet& ref_quantified_variables, VariableSet& ref_free_variables)
{
    std::visit([&](auto&& arg) { collect_free_variables_recursively(*arg, ref_quantified_variables, ref_free_variables); }, condition.get_condition());
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
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Variable element)
{
    write(*element, AddressFormatter(), out);
    return out;
}
}
