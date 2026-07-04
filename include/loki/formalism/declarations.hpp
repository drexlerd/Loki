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


#ifndef LOKI_FORMALISM_DECLARATIONS_HPP_
#define LOKI_FORMALISM_DECLARATIONS_HPP_

#include <yggdrasil/core/type_list.hpp>
#include <yggdrasil/formalism/declarations.hpp>

#include <concepts>
#include <cstdint>
#include <tuple>

namespace loki::formalism
{

struct ObjectTag {};

using ObjectBinding = ::ygg::formalism::Object<ObjectTag>;
using Row = ::ygg::formalism::Row;

template<typename T>
using RelationBinding = ::ygg::formalism::RelationBinding<T, ObjectTag>;

struct Requirement
{
};
struct Type
{
};
struct Object
{
};
struct Variable
{
};
struct Parameter
{
};
struct Predicate
{
};
struct FunctionSkeleton
{
};
struct Term
{
};
struct Atom
{
};
struct Literal
{
};
struct FunctionExpressionNumber
{
};
struct FunctionTerm
{
};
struct UnaryFunctionExpression
{
};
struct BinaryFunctionExpression
{
};
struct MultiFunctionExpression
{
};
struct FunctionExpression
{
};
struct ConditionLiteral
{
};
struct ConditionAnd
{
};
struct ConditionOr
{
};
struct ConditionNot
{
};
struct ConditionImply
{
};
struct ConditionExists
{
};
struct ConditionForall
{
};
struct ConditionNumericConstraint
{
};
struct Condition
{
};
struct EffectLiteral
{
};
struct EffectAnd
{
};
struct EffectNumeric
{
};
struct EffectForall
{
};
struct EffectWhen
{
};
struct EffectOneOf
{
};
struct EffectProbabilisticAlternative
{
};
struct EffectProbabilistic
{
};
struct Effect
{
};
struct Action
{
};
struct Axiom
{
};
struct Metric
{
};
struct InitialFunctionValue
{
};
struct Domain
{
};
struct Task
{
};

struct Positive { static constexpr std::uint8_t kind = 0; auto identifying_members() const noexcept { return std::tie(kind); } };
struct Negative { static constexpr std::uint8_t kind = 1; auto identifying_members() const noexcept { return std::tie(kind); } };

template<typename T>
concept Polarity = std::same_as<T, Positive> || std::same_as<T, Negative>;

struct Minimize { static constexpr std::uint8_t kind = 0; auto identifying_members() const noexcept { return std::tie(kind); } };
struct Maximize { static constexpr std::uint8_t kind = 1; auto identifying_members() const noexcept { return std::tie(kind); } };

template<typename T>
concept Optimization = std::same_as<T, Minimize> || std::same_as<T, Maximize>;

enum class BinaryComparator : std::uint8_t { Equal, NotEqual, Less, LessEqual, Greater, GreaterEqual };
enum class UnaryArithmeticOperator : std::uint8_t { Minus };
enum class BinaryArithmeticOperator : std::uint8_t { Add, Subtract, Multiply, Divide };
enum class MultiArithmeticOperator : std::uint8_t { Add, Multiply };
enum class NumericEffectOperator : std::uint8_t { Assign, Increase, Decrease, ScaleUp, ScaleDown };

enum class RequirementKind : std::uint8_t
{
    Strips,
    Typing,
    NegativePreconditions,
    DisjunctivePreconditions,
    Equality,
    ExistentialPreconditions,
    UniversalPreconditions,
    QuantifiedPreconditions,
    ConditionalEffects,
    Fluents,
    NumericFluents,
    DurativeActions,
    DerivedPredicates,
    NonDeterministic,
    ProbabilisticEffects,
};

using SymbolRepositoryTypes = ygg::TypeList<Requirement,
                                            Type,
                                            Object,
                                            Variable,
                                            Parameter,
                                            Predicate,
                                            FunctionSkeleton,
                                            Term,
                                            Atom,
                                            Literal,
                                            FunctionExpressionNumber,
                                            FunctionTerm,
                                            UnaryFunctionExpression,
                                            BinaryFunctionExpression,
                                            MultiFunctionExpression,
                                            FunctionExpression,
                                            ConditionLiteral,
                                            ConditionAnd,
                                            ConditionOr,
                                            ConditionNot,
                                            ConditionImply,
                                            ConditionExists,
                                            ConditionForall,
                                            ConditionNumericConstraint,
                                            Condition,
                                            EffectLiteral,
                                            EffectAnd,
                                            EffectNumeric,
                                            EffectForall,
                                            EffectWhen,
                                            EffectOneOf,
                                            EffectProbabilisticAlternative,
                                            EffectProbabilistic,
                                            Effect,
                                            Action,
                                            Axiom,
                                            Metric,
                                            InitialFunctionValue,
                                            Domain,
                                            Task>;
using RelationRepositoryTypes = ygg::TypeList<Atom>;
template<typename T>
inline const T& get_repository(const T& context) noexcept
{
    return context;
}

}

#endif
