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

#ifndef LOKI_INCLUDE_LOKI_PDDL_POSITION_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_POSITION_CACHE_HPP_

#include "loki/ast/config.hpp"
#include "loki/pddl/action.hpp"
#include "loki/pddl/atom.hpp"
#include "loki/pddl/axiom.hpp"
#include "loki/pddl/conditions.hpp"
#include "loki/pddl/declarations.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/error_reporting.hpp"
#include "loki/pddl/function.hpp"
#include "loki/pddl/function_expressions.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/function_value.hpp"
#include "loki/pddl/literal.hpp"
#include "loki/pddl/metric.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/requirements.hpp"
#include "loki/pddl/term.hpp"
#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"
#include "loki/utils/filesystem.hpp"

#include <unordered_map>

namespace loki
{
template<typename T>
using PositionMapType = std::unordered_map<const T*, PositionList>;

/// @brief Stores all occurrences of a PDDL object in the input file for each PDDL type T.
template<typename... Ts>
class PositionCache
{
private:
    std::tuple<PositionMapType<Ts>...> m_positions;

public:
    template<typename T>
    void push_back(const T* element, const Position& position)
    {
        auto& t_positions = std::get<PositionMapType<T>>(m_positions);
        t_positions[element].push_back(position);
    }

    template<typename T>
    PositionList get(const T* element) const
    {
        auto& t_positions = std::get<PositionMapType<T>>(m_positions);
        auto it = t_positions.find(element);
        if (it != t_positions.end())
        {
            return it->second;
        }
        return {};
    }
};

using PositionCaches = PositionCache<RequirementsImpl,
                                     TypeImpl,
                                     VariableImpl,
                                     TermImpl,
                                     ObjectImpl,
                                     AtomImpl,
                                     LiteralImpl,
                                     ParameterImpl,
                                     PredicateImpl,
                                     FunctionExpressionImpl,
                                     FunctionImpl,
                                     FunctionSkeletonImpl,
                                     ConditionImpl,
                                     EffectImpl,
                                     ActionImpl,
                                     AxiomImpl,
                                     OptimizationMetricImpl,
                                     FunctionValueImpl,
                                     DomainImpl,
                                     ProblemImpl>;

}

#endif