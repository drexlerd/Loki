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

#include "loki/pddl/base.hpp"
#include "loki/pddl/declarations.hpp"

#include <string>

namespace loki
{
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}

namespace loki::pddl
{
enum class OptimizationMetricEnum
{
    MINIMIZE,
    MAXIMIZE
};

extern std::unordered_map<OptimizationMetricEnum, std::string> optimization_metric_enum_to_string;
extern const std::string& to_string(pddl::OptimizationMetricEnum optimization_metric);

class OptimizationMetricImpl : public Base<OptimizationMetricImpl>
{
private:
    OptimizationMetricEnum m_optimization_metric;
    FunctionExpression m_function_expression;

    OptimizationMetricImpl(int identifier, OptimizationMetricEnum optimization_metric, FunctionExpression function_expression);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    /// @brief Test for semantic equivalence
    bool is_structurally_equivalent_to_impl(const OptimizationMetricImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<OptimizationMetricImpl>;

public:
    OptimizationMetricEnum get_optimization_metric() const;
    const FunctionExpression& get_function_expression() const;
};

}

namespace std
{
// Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
template<>
struct less<loki::pddl::OptimizationMetric>
{
    bool operator()(const loki::pddl::OptimizationMetric& left_metric, const loki::pddl::OptimizationMetric& right_metric) const;
};

template<>
struct hash<loki::pddl::OptimizationMetricImpl>
{
    std::size_t operator()(const loki::pddl::OptimizationMetricImpl& metric) const;
};
}

#endif
