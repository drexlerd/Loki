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

#include "mappings.hpp"

#include "loki/semantic/errors.hpp"

#include <utility>

namespace loki::semantic
{

std::string key(std::string text)
{
    if (!text.empty() && text.front() == ':')
        text.erase(text.begin());
    for (auto& c : text)
        if (c >= 'A' && c <= 'Z')
            c = static_cast<char>(c - 'A' + 'a');
    return text;
}

cista::offset::string to_cista(const std::string& text) { return cista::offset::string(text); }

formalism::RequirementKind requirement_kind(const ast::Requirement& node, const DiagnosticContext& diagnostics)
{
    auto name = key(node.name.text);
    if (name == "strips")
        return formalism::RequirementKind::Strips;
    if (name == "typing")
        return formalism::RequirementKind::Typing;
    if (name == "negative-preconditions")
        return formalism::RequirementKind::NegativePreconditions;
    if (name == "disjunctive-preconditions")
        return formalism::RequirementKind::DisjunctivePreconditions;
    if (name == "equality")
        return formalism::RequirementKind::Equality;
    if (name == "existential-preconditions")
        return formalism::RequirementKind::ExistentialPreconditions;
    if (name == "universal-preconditions")
        return formalism::RequirementKind::UniversalPreconditions;
    if (name == "quantified-preconditions")
        return formalism::RequirementKind::QuantifiedPreconditions;
    if (name == "conditional-effects")
        return formalism::RequirementKind::ConditionalEffects;
    if (name == "fluents")
        return formalism::RequirementKind::Fluents;
    if (name == "numeric-fluents")
        return formalism::RequirementKind::NumericFluents;
    if (name == "action-costs")
        return formalism::RequirementKind::ActionCosts;
    if (name == "adl")
        return formalism::RequirementKind::QuantifiedPreconditions;
    if (name == "durative-actions")
        return formalism::RequirementKind::DurativeActions;
    if (name == "derived-predicates")
        return formalism::RequirementKind::DerivedPredicates;
    if (name == "non-deterministic")
        return formalism::RequirementKind::NonDeterministic;
    if (name == "probabilistic-effects")
        return formalism::RequirementKind::ProbabilisticEffects;
    diagnostics.throw_at(node.name, UnsupportedRequirementError(name));
}

std::string requirement_name(formalism::RequirementKind kind)
{
    switch (kind)
    {
        case formalism::RequirementKind::Typing:
            return "typing";
        case formalism::RequirementKind::NegativePreconditions:
            return "negative-preconditions";
        case formalism::RequirementKind::DisjunctivePreconditions:
            return "disjunctive-preconditions";
        case formalism::RequirementKind::Equality:
            return "equality";
        case formalism::RequirementKind::ExistentialPreconditions:
            return "existential-preconditions";
        case formalism::RequirementKind::UniversalPreconditions:
            return "universal-preconditions";
        case formalism::RequirementKind::QuantifiedPreconditions:
            return "quantified-preconditions";
        case formalism::RequirementKind::ConditionalEffects:
            return "conditional-effects";
        case formalism::RequirementKind::Fluents:
            return "fluents";
        case formalism::RequirementKind::NumericFluents:
            return "numeric-fluents";
        case formalism::RequirementKind::ActionCosts:
            return "action-costs";
        case formalism::RequirementKind::DurativeActions:
            return "durative-actions";
        case formalism::RequirementKind::DerivedPredicates:
            return "derived-predicates";
        case formalism::RequirementKind::NonDeterministic:
            return "non-deterministic";
        case formalism::RequirementKind::ProbabilisticEffects:
            return "probabilistic-effects";
        case formalism::RequirementKind::Strips:
            return "strips";
    }
    return "unknown";
}

formalism::BinaryComparator comparator(const ast::ConditionNumericConstraint& node, const DiagnosticContext& diagnostics)
{
    auto op = key(node.comparator);
    if (op == "=")
        return formalism::BinaryComparator::Equal;
    if (op == "!=")
        return formalism::BinaryComparator::NotEqual;
    if (op == "<")
        return formalism::BinaryComparator::Less;
    if (op == "<=")
        return formalism::BinaryComparator::LessEqual;
    if (op == ">")
        return formalism::BinaryComparator::Greater;
    if (op == ">=")
        return formalism::BinaryComparator::GreaterEqual;
    diagnostics.throw_at(node, InvalidNumericConstraintError(op));
}

formalism::BinaryArithmeticOperator binary_operator(std::string op)
{
    op = key(std::move(op));
    if (op == "/")
        return formalism::BinaryArithmeticOperator::Divide;
    return formalism::BinaryArithmeticOperator::Subtract;
}

formalism::MultiArithmeticOperator multi_operator(std::string op)
{
    return key(std::move(op)) == "*" ? formalism::MultiArithmeticOperator::Multiply : formalism::MultiArithmeticOperator::Add;
}

formalism::NumericEffectOperator numeric_effect_operator(const ast::EffectNumeric& node, const DiagnosticContext& diagnostics)
{
    auto op = key(node.op);
    if (op == "assign")
        return formalism::NumericEffectOperator::Assign;
    if (op == "increase")
        return formalism::NumericEffectOperator::Increase;
    if (op == "decrease")
        return formalism::NumericEffectOperator::Decrease;
    if (op == "scale-up")
        return formalism::NumericEffectOperator::ScaleUp;
    if (op == "scale-down")
        return formalism::NumericEffectOperator::ScaleDown;
    diagnostics.throw_at(node, InvalidNumericEffectError(op));
}

}  // namespace loki::semantic
