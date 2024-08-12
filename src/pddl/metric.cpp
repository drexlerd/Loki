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

#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/visitors.hpp"
#include "loki/details/utils/collections.hpp"
#include "loki/details/utils/hash.hpp"

#include <cassert>

using namespace std;

namespace loki
{
std::unordered_map<OptimizationMetricEnum, std::string> optimization_metric_enum_to_string = {
    { OptimizationMetricEnum::MINIMIZE, "minimize" },
    { OptimizationMetricEnum::MAXIMIZE, "maximize" },
};

const std::string& to_string(OptimizationMetricEnum optimization_metric)
{
    assert(optimization_metric_enum_to_string.count(optimization_metric));
    return optimization_metric_enum_to_string.at(optimization_metric);
}

OptimizationMetricImpl::OptimizationMetricImpl(size_t index, OptimizationMetricEnum optimization_metric, FunctionExpression function_expression) :
    Base(index),
    m_optimization_metric(optimization_metric),
    m_function_expression(std::move(function_expression))
{
}

void OptimizationMetricImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << "(" << to_string(m_optimization_metric) << " ";
    std::visit(StringifyVisitor(out, options), *m_function_expression);
    out << ")";
}

OptimizationMetricEnum OptimizationMetricImpl::get_optimization_metric() const { return m_optimization_metric; }

const FunctionExpression& OptimizationMetricImpl::get_function_expression() const { return m_function_expression; }

size_t ShallowHash<OptimizationMetricImpl>::operator()(const OptimizationMetricImpl& e) const
{
    return ShallowHashCombiner()(e.get_optimization_metric(), e.get_function_expression());
}

bool ShallowEqualTo<OptimizationMetricImpl>::operator()(const OptimizationMetricImpl& l, const OptimizationMetricImpl& r) const
{
    if (&l != &r)
    {
        return (l.get_optimization_metric() == r.get_optimization_metric()) && (l.get_function_expression() == r.get_function_expression());
    }
    return true;
}

}
