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

#include "boost/hana.hpp"
#include "loki/details/utils/concepts.hpp"

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

namespace loki
{

/**
 * Common
 */
template<typename T>
using PDDLElement = const T*;

template<typename T, typename Hash, typename EqualTo>
class SegmentedRepository;

/**
 * Domain
 */

class RequirementsImpl;
using Requirements = const RequirementsImpl*;

class TypeImpl;
using Type = const TypeImpl*;
using TypeList = std::vector<Type>;
using TypeSet = std::unordered_set<Type>;
using TypeMap = std::unordered_map<Type, Type>;

class ObjectImpl;
using Object = const ObjectImpl*;
using ObjectList = std::vector<Object>;
using ObjectSet = std::unordered_set<Object>;
using ObjectMap = std::unordered_map<Object, Object>;

class VariableImpl;
using Variable = const VariableImpl*;
using VariableList = std::vector<Variable>;
using VariableSet = std::unordered_set<Variable>;
using VariableAssignment = std::unordered_map<Variable, Object>;

class TermImpl;
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
using PredicateSet = std::unordered_set<Predicate>;
using PredicateMap = std::unordered_map<Predicate, Predicate>;

class LiteralImpl;
using Literal = const LiteralImpl*;
using LiteralList = std::vector<Literal>;
using LiteralSet = std::unordered_set<Literal>;

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
using ConditionForall = const ConditionForallImpl*;
class ConditionNumericConstraintImpl;
using ConditionNumericConstraint = const ConditionNumericConstraintImpl*;
class ConditionImpl;
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
class EffectCompositeProbabilisticImpl;
using EffectCompositeProbabilistic = const EffectCompositeProbabilisticImpl*;
class EffectImpl;
using Effect = const EffectImpl*;
using EffectList = std::vector<Effect>;
using EffectDistribution = std::vector<std::pair<double, Effect>>;

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
class FunctionExpressionImpl;
using FunctionExpression = const FunctionExpressionImpl*;
using FunctionExpressionList = std::vector<FunctionExpression>;

class FunctionSkeletonImpl;
using FunctionSkeleton = const FunctionSkeletonImpl*;
using FunctionSkeletonList = std::vector<FunctionSkeleton>;
using FunctionSkeletonSet = std::unordered_set<FunctionSkeleton>;
using FunctionSkeletonMap = std::unordered_map<FunctionSkeleton, FunctionSkeleton>;

class FunctionImpl;
using Function = const FunctionImpl*;
using FunctionList = std::vector<Function>;

class ConstraintImpl;
using Constraint = const ConstraintImpl*;
using ConstraintList = std::vector<Constraint>;

class ActionImpl;
using Action = const ActionImpl*;
using ActionList = std::vector<Action>;
using ActionSet = std::unordered_set<Action>;

class AxiomImpl;
using Axiom = const AxiomImpl*;
using AxiomList = std::vector<Axiom>;
using AxiomSet = std::unordered_set<Axiom>;

class DomainImpl;
using Domain = std::shared_ptr<const DomainImpl>;

/**
 * Problem
 */

class FunctionValueImpl;
using FunctionValue = const FunctionValueImpl*;
using FunctionValueList = std::vector<FunctionValue>;
using FunctionValueSet = std::unordered_set<FunctionValue>;

class OptimizationMetricImpl;
using OptimizationMetric = const OptimizationMetricImpl*;

class ProblemImpl;
using Problem = std::shared_ptr<const ProblemImpl>;
using ProblemList = std::vector<Problem>;

/**
 * Construction
 */

class DomainBuilder;
struct DomainParsingContext;

class ProblemBuilder;
struct ProblemParsingContext;

class Repositories;

}

#endif
