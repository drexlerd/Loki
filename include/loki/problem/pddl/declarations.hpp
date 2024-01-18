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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_DECLARATIONS_HPP_

#include "../../domain/pddl/declarations.hpp"

#include <vector>


namespace loki::pddl {
    class GroundAtomImpl;
    using GroundAtom = const GroundAtomImpl*;
    using GroundAtomList = std::vector<GroundAtom>;

    class GroundLiteralImpl;
    using GroundLiteral = const GroundLiteralImpl*;
    using GroundLiteralList = std::vector<GroundLiteral>;

    class GroundConditionLiteralImpl;
    class GroundConditionAndImpl;
    class GroundConditionOrImpl;
    class GroundConditionNotImpl;
    class GroundConditionImplyImpl;
    class GroundConditionExistsImpl;
    class GroundConditionForallImpl;
    using GroundConditionImpl = std::variant<GroundConditionLiteralImpl
        , GroundConditionAndImpl
        , GroundConditionOrImpl
        , GroundConditionNotImpl
        , GroundConditionImplyImpl
        , GroundConditionExistsImpl
        , GroundConditionForallImpl>;
    using GroundCondition = const GroundConditionImpl*;
    using GroundConditionList = std::vector<GroundCondition>;

    class GroundFunctionExpressionNumberImpl;
    class GroundFunctionExpressionBinaryOperatorImpl;
    class GroundFunctionExpressionMultiOperatorImpl;
    class GroundFunctionExpressionMinusImpl;
    class GroundFunctionExpressionFunctionImpl;
    using GroundFunctionExpressionImpl = std::variant<GroundFunctionExpressionNumberImpl
        , GroundFunctionExpressionBinaryOperatorImpl
        , GroundFunctionExpressionMultiOperatorImpl
        , GroundFunctionExpressionMinusImpl
        , GroundFunctionExpressionFunctionImpl>;
    using GroundFunctionExpression = const GroundFunctionExpressionImpl*;
    using GroundFunctionExpressionList = std::vector<GroundFunctionExpression>;;

    class GroundFunctionImpl;
    using GroundFunction = const GroundFunctionImpl*;
    using GroundFunctionList = std::vector<GroundFunction>;

    class NumericFluentImpl;
    using NumericFluent = const NumericFluentImpl*;
    using NumericFluentList = std::vector<NumericFluent>;

    class OptimizationMetricImpl;
    using OptimizationMetric = const OptimizationMetricImpl*;

    class ProblemImpl;
    using Problem = const ProblemImpl*;
    using ProblemList = std::vector<Problem>;
}

#endif
