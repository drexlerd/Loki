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

#include "loki/details/pddl/metric.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/function_expressions.hpp"

#include <cassert>

using namespace std;

namespace loki
{
std::unordered_map<OptimizationMetricEnum, std::string> optimization_metric_enum_to_string = {
    { OptimizationMetricEnum::MINIMIZE, "minimize" },
    { OptimizationMetricEnum::MAXIMIZE, "maximize" },
};

std::ostream& operator<<(std::ostream& out, OptimizationMetricEnum element)
{
    assert(optimization_metric_enum_to_string.count(element));
    out << optimization_metric_enum_to_string.at(element);
    return out;
}

OptimizationMetricImpl::OptimizationMetricImpl(size_t index, OptimizationMetricEnum optimization_metric, FunctionExpression function_expression) :
    m_index(index),
    m_optimization_metric(optimization_metric),
    m_function_expression(std::move(function_expression))
{
}

size_t OptimizationMetricImpl::get_index() const { return m_index; }

OptimizationMetricEnum OptimizationMetricImpl::get_optimization_metric() const { return m_optimization_metric; }

FunctionExpression OptimizationMetricImpl::get_function_expression() const { return m_function_expression; }

std::ostream& operator<<(std::ostream& out, const OptimizationMetricImpl& element)
{
    write(element, StringFormatter(), out);
    return out;
}

std::ostream& operator<<(std::ostream& out, OptimizationMetric element)
{
    write(*element, AddressFormatter(), out);
    return out;
}

}
