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

#ifndef LOKI_INCLUDE_LOKI_PDDL_METRIC_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_METRIC_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
enum class OptimizationMetricEnum
{
    MINIMIZE,
    MAXIMIZE
};

extern std::unordered_map<OptimizationMetricEnum, std::string> optimization_metric_enum_to_string;
extern const std::string& to_string(OptimizationMetricEnum optimization_metric);

class OptimizationMetricImpl
{
private:
    size_t m_index;
    OptimizationMetricEnum m_optimization_metric;
    FunctionExpression m_function_expression;

    OptimizationMetricImpl(size_t index, OptimizationMetricEnum optimization_metric, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    OptimizationMetricImpl(const OptimizationMetricImpl& other) = delete;
    OptimizationMetricImpl& operator=(const OptimizationMetricImpl& other) = delete;
    OptimizationMetricImpl(OptimizationMetricImpl&& other) = default;
    OptimizationMetricImpl& operator=(OptimizationMetricImpl&& other) = default;

    size_t get_index() const;
    OptimizationMetricEnum get_optimization_metric() const;
    FunctionExpression get_function_expression() const;

    auto identifying_members() const { return std::tuple(get_optimization_metric(), get_function_expression()); }
};

extern std::ostream& operator<<(std::ostream& out, const OptimizationMetricImpl& element);

extern std::ostream& operator<<(std::ostream& out, OptimizationMetric element);
}

#endif
