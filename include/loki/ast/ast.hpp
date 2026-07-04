/*
 * Copyright (C) 2024-2026 Dominik Drexler
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


#ifndef LOKI_AST_AST_HPP_
#define LOKI_AST_AST_HPP_

#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <string>
#include <vector>

namespace loki::ast
{
namespace x3 = boost::spirit::x3;

struct Identifier : x3::position_tagged
{
    std::string text;
};

struct TypeExpression;
using TypeExpressionPtr = x3::forward_ast<TypeExpression>;

struct TypeReference : x3::position_tagged
{
    Identifier name;
};

struct EitherType : x3::position_tagged
{
    std::vector<TypeExpressionPtr> alternatives;
};

struct TypeExpression : x3::position_tagged, x3::variant<TypeReference, EitherType>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct TypedName : x3::position_tagged
{
    Identifier name;
    boost::optional<TypeExpression> type;
};

struct TypedVariable : x3::position_tagged
{
    Identifier variable;
    boost::optional<TypeExpression> type;
};

struct Requirement : x3::position_tagged
{
    Identifier name;
};

struct Term : x3::position_tagged
{
    Identifier name;
    bool variable = false;
};

struct Atom : x3::position_tagged
{
    Identifier predicate;
    std::vector<Term> terms;
};

struct Literal : x3::position_tagged
{
    Atom atom;
    bool positive = true;
};

struct FunctionTerm : x3::position_tagged
{
    Identifier function;
    std::vector<Term> terms;
};

struct FunctionExpression;
using FunctionExpressionPtr = x3::forward_ast<FunctionExpression>;

struct FunctionExpressionNumber : x3::position_tagged
{
    double value = 0.0;
};

struct FunctionExpressionFunction : x3::position_tagged
{
    FunctionTerm term;
};

struct FunctionExpressionUnary : x3::position_tagged
{
    std::string op;
    FunctionExpressionPtr expression;
};

struct FunctionExpressionBinary : x3::position_tagged
{
    std::string op;
    FunctionExpressionPtr left;
    FunctionExpressionPtr right;
};

struct FunctionExpressionMulti : x3::position_tagged
{
    std::string op;
    std::vector<FunctionExpressionPtr> expressions;
};

struct FunctionExpression : x3::position_tagged,
                            x3::variant<FunctionExpressionNumber,
                                        FunctionExpressionFunction,
                                        FunctionExpressionUnary,
                                        FunctionExpressionBinary,
                                        FunctionExpressionMulti>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Condition;
using ConditionPtr = x3::forward_ast<Condition>;

struct ConditionLiteral : x3::position_tagged
{
    Literal literal;
};

struct ConditionAnd : x3::position_tagged
{
    std::vector<ConditionPtr> conditions;
};

struct ConditionOr : x3::position_tagged
{
    std::vector<ConditionPtr> conditions;
};

struct ConditionNot : x3::position_tagged
{
    ConditionPtr condition;
};

struct ConditionImply : x3::position_tagged
{
    ConditionPtr left;
    ConditionPtr right;
};

struct ConditionExists : x3::position_tagged
{
    std::vector<TypedVariable> parameters;
    ConditionPtr condition;
};

struct ConditionForall : x3::position_tagged
{
    std::vector<TypedVariable> parameters;
    ConditionPtr condition;
};

struct ConditionNumericConstraint : x3::position_tagged
{
    std::string comparator;
    FunctionExpressionPtr left;
    FunctionExpressionPtr right;
};

struct Condition : x3::position_tagged,
                   x3::variant<ConditionLiteral,
                               ConditionAnd,
                               ConditionOr,
                               ConditionNot,
                               ConditionImply,
                               ConditionExists,
                               ConditionForall,
                               ConditionNumericConstraint>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Effect;
using EffectPtr = x3::forward_ast<Effect>;

struct EffectLiteral : x3::position_tagged
{
    Literal literal;
};

struct EffectAnd : x3::position_tagged
{
    std::vector<EffectPtr> effects;
};

struct EffectNumeric : x3::position_tagged
{
    std::string op;
    FunctionTerm function;
    FunctionExpressionPtr expression;
};

struct EffectForall : x3::position_tagged
{
    std::vector<TypedVariable> parameters;
    EffectPtr effect;
};

struct EffectWhen : x3::position_tagged
{
    ConditionPtr condition;
    EffectPtr effect;
};

struct EffectOneOf : x3::position_tagged
{
    std::vector<EffectPtr> effects;
};

struct ProbabilisticEffectAlternative : x3::position_tagged
{
    double probability = 0.0;
    EffectPtr effect;
};

struct EffectProbabilistic : x3::position_tagged
{
    std::vector<ProbabilisticEffectAlternative> alternatives;
};

struct Effect : x3::position_tagged,
                x3::variant<EffectLiteral,
                            EffectAnd,
                            EffectNumeric,
                            EffectForall,
                            EffectWhen,
                            EffectOneOf,
                            EffectProbabilistic>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct PredicateDeclaration : x3::position_tagged
{
    Identifier name;
    std::vector<TypedVariable> parameters;
};

struct FunctionDeclaration : x3::position_tagged
{
    Identifier name;
    std::vector<TypedVariable> parameters;
    boost::optional<TypeExpression> type;
};

struct Action : x3::position_tagged
{
    Identifier name;
    std::vector<TypedVariable> parameters;
    boost::optional<Condition> precondition;
    boost::optional<Effect> effect;
};

struct Axiom : x3::position_tagged
{
    PredicateDeclaration head;
    Condition condition;
};

struct Metric : x3::position_tagged
{
    Identifier optimization;
    FunctionExpression expression;
};

struct InitialFunctionValue : x3::position_tagged
{
    FunctionTerm function;
    FunctionExpression value;
};

struct InitialElement : x3::position_tagged, x3::variant<Literal, InitialFunctionValue>
{
    using base_type::base_type;
    using base_type::operator=;
};

struct Domain : x3::position_tagged
{
    Identifier name;
    std::vector<Requirement> requirements;
    std::vector<TypedName> types;
    std::vector<TypedName> constants;
    std::vector<PredicateDeclaration> predicates;
    std::vector<FunctionDeclaration> functions;
    std::vector<Axiom> axioms;
    std::vector<Action> actions;
};

struct Task : x3::position_tagged
{
    Identifier name;
    Identifier domain_name;
    std::vector<Requirement> requirements;
    std::vector<TypedName> objects;
    std::vector<InitialElement> initial;
    boost::optional<Condition> goal;
    boost::optional<Metric> metric;
    std::vector<Axiom> axioms;
};

using File = x3::variant<Domain, Task>;

}

#endif
