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

#ifndef LOKI_FORMALISM_ENUMS_HPP_
#define LOKI_FORMALISM_ENUMS_HPP_

#include <cstdint>
#include <stdexcept>

namespace loki::formalism
{

enum class BinaryComparator : std::uint8_t
{
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual
};
enum class UnaryArithmeticOperator : std::uint8_t
{
    Minus
};
enum class BinaryArithmeticOperator : std::uint8_t
{
    Add,
    Subtract,
    Multiply,
    Divide
};
enum class MultiArithmeticOperator : std::uint8_t
{
    Add,
    Multiply
};
enum class NumericEffectOperator : std::uint8_t
{
    Assign,
    Increase,
    Decrease,
    ScaleUp,
    ScaleDown
};

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
    Adl,
    ConditionalEffects,
    Fluents,
    NumericFluents,
    ActionCosts,
    DurativeActions,
    DerivedPredicates,
    NonDeterministic,
    ProbabilisticEffects,
};

inline const char* to_string(BinaryComparator op)
{
    switch (op)
    {
        case BinaryComparator::Equal:
            return "=";
        case BinaryComparator::NotEqual:
            return "!=";
        case BinaryComparator::Less:
            return "<";
        case BinaryComparator::LessEqual:
            return "<=";
        case BinaryComparator::Greater:
            return ">";
        case BinaryComparator::GreaterEqual:
            return ">=";
    }
    throw std::invalid_argument("invalid BinaryComparator");
}

inline const char* to_string(BinaryArithmeticOperator op)
{
    switch (op)
    {
        case BinaryArithmeticOperator::Add:
            return "+";
        case BinaryArithmeticOperator::Subtract:
            return "-";
        case BinaryArithmeticOperator::Multiply:
            return "*";
        case BinaryArithmeticOperator::Divide:
            return "/";
    }
    throw std::invalid_argument("invalid BinaryArithmeticOperator");
}

inline const char* to_string(MultiArithmeticOperator op)
{
    switch (op)
    {
        case MultiArithmeticOperator::Add:
            return "+";
        case MultiArithmeticOperator::Multiply:
            return "*";
    }
    throw std::invalid_argument("invalid MultiArithmeticOperator");
}

inline const char* to_string(NumericEffectOperator op)
{
    switch (op)
    {
        case NumericEffectOperator::Assign:
            return "assign";
        case NumericEffectOperator::Increase:
            return "increase";
        case NumericEffectOperator::Decrease:
            return "decrease";
        case NumericEffectOperator::ScaleUp:
            return "scale-up";
        case NumericEffectOperator::ScaleDown:
            return "scale-down";
    }
    throw std::invalid_argument("invalid NumericEffectOperator");
}

inline const char* to_string(RequirementKind kind)
{
    switch (kind)
    {
        case RequirementKind::Strips:
            return ":strips";
        case RequirementKind::Typing:
            return ":typing";
        case RequirementKind::NegativePreconditions:
            return ":negative-preconditions";
        case RequirementKind::DisjunctivePreconditions:
            return ":disjunctive-preconditions";
        case RequirementKind::Equality:
            return ":equality";
        case RequirementKind::ExistentialPreconditions:
            return ":existential-preconditions";
        case RequirementKind::UniversalPreconditions:
            return ":universal-preconditions";
        case RequirementKind::QuantifiedPreconditions:
            return ":quantified-preconditions";
        case RequirementKind::Adl:
            return ":adl";
        case RequirementKind::ConditionalEffects:
            return ":conditional-effects";
        case RequirementKind::Fluents:
            return ":fluents";
        case RequirementKind::NumericFluents:
            return ":numeric-fluents";
        case RequirementKind::ActionCosts:
            return ":action-costs";
        case RequirementKind::DurativeActions:
            return ":durative-actions";
        case RequirementKind::DerivedPredicates:
            return ":derived-predicates";
        case RequirementKind::NonDeterministic:
            return ":non-deterministic";
        case RequirementKind::ProbabilisticEffects:
            return ":probabilistic-effects";
    }
    throw std::invalid_argument("invalid RequirementKind");
}

}  // namespace loki::formalism

#endif
