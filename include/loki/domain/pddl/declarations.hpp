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
    using Requirements = std::shared_ptr<const RequirementsImpl>;

    class TypeImpl;
    using Type = std::shared_ptr<const TypeImpl>;
    using TypeList = std::vector<Type>;
    using TypeSet = std::set<Type>;

    class ObjectImpl;
    using Object = std::shared_ptr<const ObjectImpl>;
    using ObjectList = std::vector<Object>;
    using ObjectSet = std::set<Object>;

    class VariableImpl;
    using Variable = std::shared_ptr<const VariableImpl>;
    using VariableList = std::vector<Variable>;
    using VariableAssignment = std::unordered_map<Variable, Object>;

    class TermImpl;
    using Term = std::shared_ptr<const TermImpl>;
    using TermList = std::vector<Term>;
    class TermObjectImpl;
    using TermObject = std::shared_ptr<const TermObjectImpl>;
    class TermVariableImpl;
    using TermVariable = std::shared_ptr<const TermVariableImpl>;

    class AtomImpl;
    using Atom = std::shared_ptr<const AtomImpl>;
    using AtomList = std::vector<Atom>;

    class ParameterImpl;
    using Parameter = std::shared_ptr<const ParameterImpl>;
    using ParameterList = std::vector<Parameter>;
    using ParameterAssignment = std::unordered_map<Parameter, Object>;

    class PredicateImpl;
    using Predicate = std::shared_ptr<const PredicateImpl>;
    using PredicateList = std::vector<Predicate>;
    using PredicateSet = std::set<Predicate>;

    class LiteralImpl;
    using Literal = std::shared_ptr<const LiteralImpl>;
    using LiteralList = std::vector<Literal>;
    using LiteralSet = std::set<Literal>;

    class ConditionImpl;
    using Condition = std::shared_ptr<const ConditionImpl>;
    using ConditionList = std::vector<Condition>;
    using ConditionSet = std::set<Condition>;
    class ConditionLiteralImpl;
    using ConditionLiteral = std::shared_ptr<const ConditionLiteralImpl>;
    class ConditionAndImpl;
    using ConditionAnd = std::shared_ptr<const ConditionAndImpl>;
    class ConditionOrImpl;
    using ConditionOr = std::shared_ptr<const ConditionOrImpl>;
    class ConditionNotImpl;
    using ConditionNot = std::shared_ptr<const ConditionNotImpl>;

    class EffectImpl;
    using Effect = std::shared_ptr<const EffectImpl>;
    using EffectList = std::vector<Effect>;
    using EffectSet = std::set<Effect>;
    class EffectLiteralImpl;
    using EffectLiteral = std::shared_ptr<const EffectLiteralImpl>;
    class EffectAndImpl;
    using EffectAnd = std::shared_ptr<const EffectAndImpl>;
    class EffectNumericImpl;
    using EffectNumeric = std::shared_ptr<const EffectNumericImpl>;
    class EffectConditionalForallImpl;
    using EffectConditionalForall = std::shared_ptr<const EffectConditionalForallImpl>;
    class EffectConditionalWhenImpl;
    using EffectConditionalWhen = std::shared_ptr<const EffectConditionalWhenImpl>;

    class FunctionExpressionImpl;
    using FunctionExpression = std::shared_ptr<const FunctionExpressionImpl>;
    class FunctionExpressionNumberImpl;
    using FunctionExpressionNumber = std::shared_ptr<const FunctionExpressionNumberImpl>;
    class FunctionExpressionBinaryOperatorImpl;
    using FunctionExpressionBinaryOperator = std::shared_ptr<const FunctionExpressionBinaryOperatorImpl>;
    class FunctionExpressionMinusImpl;
    using FunctionExpressionMinus = std::shared_ptr<const FunctionExpressionMinusImpl>;
    class FunctionExpressionFunctionImpl;
    using FunctionExpressionFunction = std::shared_ptr<const FunctionExpressionFunctionImpl>;

    class FunctionSkeletonImpl;
    using FunctionSkeleton = std::shared_ptr<const FunctionSkeletonImpl>;
    using FunctionSkeletonList = std::vector<FunctionSkeleton>;
    using FunctionSkeletonSet = std::set<FunctionSkeleton>;

    class FunctionImpl;
    using Function = std::shared_ptr<const FunctionImpl>;
    using FunctionList = std::vector<Function>;
    using FunctionSet = std::set<Function>;

    class ConstraintImpl;
    using Constraint = std::shared_ptr<const ConstraintImpl>;
    using ConstraintList = std::vector<Constraint>;
    using ConstraintSet = std::set<Constraint>;

    class ActionImpl;
    using Action = std::shared_ptr<const ActionImpl>;
    using ActionList = std::vector<Action>;
    using ActionSet = std::set<Action>;

    class DerivedPredicateImpl;
    using DerivedPredicate = std::shared_ptr<const DerivedPredicateImpl>;
    using DerivedPredicateList = std::vector<DerivedPredicate>;
    using DerivedPredicateSet = std::set<DerivedPredicate>;

    class DomainImpl;
    using Domain = std::shared_ptr<const DomainImpl>;
    using DomainList = std::vector<Domain>;
    using DomainSet = std::set<Domain>;
}

#endif
