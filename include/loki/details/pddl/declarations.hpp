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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DECLARATIONS_HPP_

#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

namespace loki
{

/**
 * Common
 */
template<typename T>
using PDDLElement = const T*;

/**
 * Utils
 */

template<typename HolderType, typename Hash, typename EqualTo>
class UniqueFactory;

/**
 * Domain
 */

class RequirementsImpl;
using Requirements = const RequirementsImpl*;

class TypeImpl;
using Type = const TypeImpl*;
using TypeList = std::vector<Type>;
using TypeSet = std::unordered_set<Type>;

class ObjectImpl;
using Object = const ObjectImpl*;
using ObjectList = std::vector<Object>;

class VariableImpl;
using Variable = const VariableImpl*;
using VariableList = std::vector<Variable>;
using VariableSet = std::unordered_set<Variable>;
using VariableAssignment = std::unordered_map<Variable, Object>;

class TermObjectImpl;
using TermObject = const TermObjectImpl*;
class TermVariableImpl;
using TermVariable = const TermVariableImpl*;
using TermImpl = std::variant<TermObjectImpl, TermVariableImpl>;
using Term = const TermImpl*;
using TermList = std::vector<Term>;

class AtomImpl;
using Atom = const AtomImpl*;
using AtomList = std::vector<Atom>;

class ParameterImpl;
using Parameter = const ParameterImpl*;
using ParameterList = std::vector<Parameter>;
using ParameterAssignment = std::unordered_map<Parameter, Object>;

class PredicateImpl;
using Predicate = const PredicateImpl*;
using PredicateList = std::vector<Predicate>;

class LiteralImpl;
using Literal = const LiteralImpl*;
using LiteralList = std::vector<Literal>;

class ConditionLiteralImpl;
using ConditionLiteral = const ConditionLiteralImpl*;
class ConditionAndImpl;
using ConditionAnd = const ConditionAndImpl*;
class ConditionOrImpl;
using ConditionOr = const ConditionOrImpl*;
class ConditionNotImpl;
using ConditionNot = const ConditionNotImpl*;
class ConditionImplyImpl;
using ConditionImply = const ConditionImplyImpl*;
class ConditionExistsImpl;
using ConditionExists = const ConditionExistsImpl*;
class ConditionForallImpl;
using ConditionImpl =
    std::variant<ConditionLiteralImpl, ConditionAndImpl, ConditionOrImpl, ConditionNotImpl, ConditionImplyImpl, ConditionExistsImpl, ConditionForallImpl>;
using Condition = const ConditionImpl*;
using ConditionList = std::vector<Condition>;

class EffectLiteralImpl;
using EffectLiteral = const EffectLiteralImpl*;
class EffectAndImpl;
using EffectAnd = const EffectAndImpl*;
class EffectNumericImpl;
using EffectNumeric = const EffectNumericImpl*;
class EffectCompositeForallImpl;
using EffectCompositeForall = const EffectCompositeForallImpl*;
class EffectCompositeWhenImpl;
using EffectCompositeWhen = const EffectCompositeWhenImpl*;
class EffectCompositeOneofImpl;
using EffectCompositeOneof = const EffectCompositeOneofImpl*;
using EffectImpl = std::variant<EffectLiteralImpl, EffectAndImpl, EffectNumericImpl, EffectCompositeForallImpl, EffectCompositeWhenImpl, EffectCompositeOneofImpl>;
using Effect = const EffectImpl*;
using EffectList = std::vector<Effect>;

class FunctionExpressionNumberImpl;
using FunctionExpressionNumber = const FunctionExpressionNumberImpl*;
class FunctionExpressionBinaryOperatorImpl;
using FunctionExpressionBinaryOperator = const FunctionExpressionBinaryOperatorImpl*;
class FunctionExpressionMultiOperatorImpl;
using FunctionExpressionMultiOperator = const FunctionExpressionMultiOperatorImpl*;
class FunctionExpressionMinusImpl;
using FunctionExpressionMinus = const FunctionExpressionMinusImpl*;
class FunctionExpressionFunctionImpl;
using FunctionExpressionFunction = const FunctionExpressionFunctionImpl*;
using FunctionExpressionImpl = std::variant<FunctionExpressionNumberImpl,
                                            FunctionExpressionBinaryOperatorImpl,
                                            FunctionExpressionMultiOperatorImpl,
                                            FunctionExpressionMinusImpl,
                                            FunctionExpressionFunctionImpl>;
using FunctionExpression = const FunctionExpressionImpl*;
using FunctionExpressionList = std::vector<FunctionExpression>;

class FunctionSkeletonImpl;
using FunctionSkeleton = const FunctionSkeletonImpl*;
using FunctionSkeletonList = std::vector<FunctionSkeleton>;

class FunctionImpl;
using Function = const FunctionImpl*;
using FunctionList = std::vector<Function>;

class ConstraintImpl;
using Constraint = const ConstraintImpl*;
using ConstraintList = std::vector<Constraint>;

class ActionImpl;
using Action = const ActionImpl*;
using ActionList = std::vector<Action>;

class AxiomImpl;
using Axiom = const AxiomImpl*;
using AxiomList = std::vector<Axiom>;

class DomainImpl;
using Domain = const DomainImpl*;
using DomainList = std::vector<Domain>;

/**
 * Problem
 */

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
