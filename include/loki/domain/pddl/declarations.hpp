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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_DECLARATIONS_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>


namespace loki::pddl {
    class RequirementsImpl;
    using Requirements = RequirementsImpl const *;

    class TypeImpl;
    using Type = TypeImpl const *;
    using TypeList = std::vector<Type>;
    using TypeSet = std::set<Type>;

    class ObjectImpl;
    using Object = ObjectImpl const *;
    using ObjectList = std::vector<Object>;
    using ObjectSet = std::set<Object>;

    class VariableImpl;
    using Variable = VariableImpl const *;
    using VariableList = std::vector<Variable>;
    using VariableAssignment = std::unordered_map<Variable, Object>;

    class TermImpl;
    using Term = TermImpl const *;
    using TermList = std::vector<Term>;
    class TermObjectImpl;
    using TermObject = TermObjectImpl const *;
    class TermVariableImpl;
    using TermVariable = TermVariableImpl const *;

    class AtomImpl;
    using Atom = AtomImpl const *;
    using AtomList = std::vector<Atom>;

    class ParameterImpl;
    using Parameter = ParameterImpl const *;
    using ParameterList = std::vector<Parameter>;
    using ParameterAssignment = std::unordered_map<Parameter, Object>;

    class PredicateImpl;
    using Predicate = PredicateImpl const *;
    using PredicateList = std::vector<Predicate>;
    using PredicateSet = std::set<Predicate>;

    class LiteralImpl;
    using Literal = LiteralImpl const *;
    using LiteralList = std::vector<Literal>;
    using LiteralSet = std::set<Literal>;

    class ConditionImpl;
    using Condition = ConditionImpl const *;
    using ConditionList = std::vector<Condition>;
    using ConditionSet = std::set<Condition>;
    class ConditionLiteralImpl;
    using ConditionLiteral = ConditionLiteralImpl const *;
    class ConditionAndImpl;
    using ConditionAnd = ConditionAndImpl const *;
    class ConditionOrImpl;
    using ConditionOr = ConditionOrImpl const *;
    class ConditionNotImpl;
    using ConditionNot = ConditionNotImpl const *;

    class EffectImpl;
    using Effect = EffectImpl const *;
    using EffectList = std::vector<Effect>;
    using EffectSet = std::set<Effect>;
    class EffectLiteralImpl;
    using EffectLiteral = EffectLiteralImpl const *;
    class EffectAndImpl;
    using EffectAnd = EffectAndImpl const *;
    class EffectNumericImpl;
    using EffectNumeric = EffectNumericImpl const *;
    class EffectConditionalForallImpl;
    using EffectConditionalForall = EffectConditionalForallImpl const *;
    class EffectConditionalWhenImpl;
    using EffectConditionalWhen = EffectConditionalWhenImpl const *;

    class FunctionExpressionImpl;
    using FunctionExpression = FunctionExpressionImpl const *;
    class FunctionExpressionNumberImpl;
    using FunctionExpressionNumber = FunctionExpressionNumberImpl const *;
    class FunctionExpressionBinaryOperatorImpl;
    using FunctionExpressionBinaryOperator = FunctionExpressionBinaryOperatorImpl const *;
    class FunctionExpressionMinusImpl;
    using FunctionExpressionMinus = FunctionExpressionMinusImpl const *;
    class FunctionExpressionFunctionImpl;
    using FunctionExpressionFunction = FunctionExpressionFunctionImpl const *;

    class FunctionSkeletonImpl;
    using FunctionSkeleton = FunctionSkeletonImpl const *;
    using FunctionSkeletonList = std::vector<FunctionSkeleton>;
    using FunctionSkeletonSet = std::set<FunctionSkeleton>;

    class FunctionImpl;
    using Function = FunctionImpl const *;
    using FunctionList = std::vector<Function>;
    using FunctionSet = std::set<Function>;

    class ConstraintImpl;
    using Constraint = ConstraintImpl const *;
    using ConstraintList = std::vector<Constraint>;
    using ConstraintSet = std::set<Constraint>;

    class ActionImpl;
    using Action = ActionImpl const *;
    using ActionList = std::vector<Action>;
    using ActionSet = std::set<Action>;

    class DerivedPredicateImpl;
    using DerivedPredicate = DerivedPredicateImpl const *;
    using DerivedPredicateList = std::vector<DerivedPredicate>;
    using DerivedPredicateSet = std::set<DerivedPredicate>;

    class DomainImpl;
    using Domain = DomainImpl const *;
    using DomainList = std::vector<Domain>;
    using DomainSet = std::set<Domain>;
}

#endif
