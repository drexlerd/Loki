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
#include <variant>


namespace loki::pddl {
    class RequirementsImpl;
    using Requirements = const RequirementsImpl*;

    class TypeImpl;
    using Type = const TypeImpl*;
    using TypeList = std::vector<Type>;
    using TypeSet = std::set<Type>;

    class ObjectImpl;
    using Object = const ObjectImpl*;
    using ObjectList = std::vector<Object>;
    using ObjectSet = std::set<Object>;

    class VariableImpl;
    using Variable = const VariableImpl*;
    using VariableList = std::vector<Variable>;
    using VariableAssignment = std::unordered_map<Variable, Object>;

    class TermImpl;
    using Term = const TermImpl*;
    using TermList = std::vector<Term>;
    class TermObjectImpl;
    using TermObject = const TermObjectImpl*;
    class TermVariableImpl;
    using TermVariable = const TermVariableImpl*;

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
    using PredicateSet = std::set<Predicate>;

    class LiteralImpl;
    using Literal = const LiteralImpl*;
    using LiteralList = std::vector<Literal>;
    using LiteralSet = std::set<Literal>;

    class ConditionLiteralImpl;
    class ConditionAndImpl;
    class ConditionOrImpl;
    class ConditionNotImpl;
    using ConditionImpl = std::variant<ConditionLiteralImpl
        , ConditionAndImpl
        , ConditionOrImpl
        , ConditionNotImpl>;
    using Condition = const ConditionImpl*;
    using ConditionList = std::vector<Condition>;
    using ConditionSet = std::set<Condition>;

    class EffectImpl;
    using Effect = const EffectImpl*;
    using EffectList = std::vector<Effect>;
    using EffectSet = std::set<Effect>;
    class EffectLiteralImpl;
    using EffectLiteral = const EffectLiteralImpl*;
    class EffectAndImpl;
    using EffectAnd = const EffectAndImpl*;
    class EffectNumericImpl;
    using EffectNumeric = const EffectNumericImpl*;
    class EffectConditionalForallImpl;
    using EffectConditionalForall = const EffectConditionalForallImpl*;
    class EffectConditionalWhenImpl;
    using EffectConditionalWhen = const EffectConditionalWhenImpl*;

    class FunctionExpressionImpl;
    using FunctionExpression = const FunctionExpressionImpl*;
    using FunctionExpressionList = std::vector<FunctionExpression>;
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

    class FunctionSkeletonImpl;
    using FunctionSkeleton = const FunctionSkeletonImpl*;
    using FunctionSkeletonList = std::vector<FunctionSkeleton>;
    using FunctionSkeletonSet = std::set<FunctionSkeleton>;

    class FunctionImpl;
    using Function = const FunctionImpl*;
    using FunctionList = std::vector<Function>;
    using FunctionSet = std::set<Function>;

    class ConstraintImpl;
    using Constraint = const ConstraintImpl*;
    using ConstraintList = std::vector<Constraint>;
    using ConstraintSet = std::set<Constraint>;

    class ActionImpl;
    using Action = const ActionImpl*;
    using ActionList = std::vector<Action>;
    using ActionSet = std::set<Action>;

    class DerivedPredicateImpl;
    using DerivedPredicate = const DerivedPredicateImpl*;
    using DerivedPredicateList = std::vector<DerivedPredicate>;
    using DerivedPredicateSet = std::set<DerivedPredicate>;

    class DomainImpl;
    using Domain = const DomainImpl*;
    using DomainList = std::vector<Domain>;
    using DomainSet = std::set<Domain>;
}

#endif
