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

#include "index.hpp"

#include <loki/semantic.hpp>
#include <nanobind/nanobind.h>
#include <yggdrasil/python/bindings.hpp>

namespace nb = nanobind;
using namespace nb::literals;

namespace loki::formalism
{
void bind_indices(nb::module_& m)
{
    ygg::bind_index<ygg::Index<formalism::Requirement>>(m, "RequirementIndex");
    ygg::bind_index<ygg::Index<formalism::Type>>(m, "TypeIndex");
    ygg::bind_index<ygg::Index<formalism::Object>>(m, "ObjectIndex");
    ygg::bind_index<ygg::Index<formalism::Variable>>(m, "VariableIndex");
    ygg::bind_index<ygg::Index<formalism::Parameter>>(m, "ParameterIndex");
    ygg::bind_index<ygg::Index<formalism::Predicate>>(m, "PredicateIndex");
    ygg::bind_index<ygg::Index<formalism::FunctionSkeleton>>(m, "FunctionSkeletonIndex");
    ygg::bind_index<ygg::Index<formalism::Term>>(m, "TermIndex");
    ygg::bind_index<ygg::Index<formalism::Atom>>(m, "AtomIndex");
    ygg::bind_index<ygg::Index<formalism::Literal>>(m, "LiteralIndex");
    ygg::bind_index<ygg::Index<formalism::FunctionExpressionNumber>>(m, "FunctionExpressionNumberIndex");
    ygg::bind_index<ygg::Index<formalism::FunctionTerm>>(m, "FunctionTermIndex");
    ygg::bind_index<ygg::Index<formalism::UnaryFunctionExpression>>(m, "UnaryFunctionExpressionIndex");
    ygg::bind_index<ygg::Index<formalism::BinaryFunctionExpression>>(m, "BinaryFunctionExpressionIndex");
    ygg::bind_index<ygg::Index<formalism::MultiFunctionExpression>>(m, "MultiFunctionExpressionIndex");
    ygg::bind_index<ygg::Index<formalism::FunctionExpression>>(m, "FunctionExpressionIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionLiteral>>(m, "ConditionLiteralIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionAnd>>(m, "ConditionAndIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionOr>>(m, "ConditionOrIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionNot>>(m, "ConditionNotIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionImply>>(m, "ConditionImplyIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionExists>>(m, "ConditionExistsIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionForall>>(m, "ConditionForallIndex");
    ygg::bind_index<ygg::Index<formalism::ConditionNumericConstraint>>(m, "ConditionNumericConstraintIndex");
    ygg::bind_index<ygg::Index<formalism::Condition>>(m, "ConditionIndex");
    ygg::bind_index<ygg::Index<formalism::EffectLiteral>>(m, "EffectLiteralIndex");
    ygg::bind_index<ygg::Index<formalism::EffectAnd>>(m, "EffectAndIndex");
    ygg::bind_index<ygg::Index<formalism::EffectNumeric>>(m, "EffectNumericIndex");
    ygg::bind_index<ygg::Index<formalism::EffectForall>>(m, "EffectForallIndex");
    ygg::bind_index<ygg::Index<formalism::EffectWhen>>(m, "EffectWhenIndex");
    ygg::bind_index<ygg::Index<formalism::EffectOneOf>>(m, "EffectOneOfIndex");
    ygg::bind_index<ygg::Index<formalism::EffectProbabilisticAlternative>>(m, "EffectProbabilisticAlternativeIndex");
    ygg::bind_index<ygg::Index<formalism::EffectProbabilistic>>(m, "EffectProbabilisticIndex");
    ygg::bind_index<ygg::Index<formalism::Effect>>(m, "EffectIndex");
    ygg::bind_index<ygg::Index<formalism::Action>>(m, "ActionIndex");
    ygg::bind_index<ygg::Index<formalism::Axiom>>(m, "AxiomIndex");
    ygg::bind_index<ygg::Index<formalism::Metric>>(m, "MetricIndex");
    ygg::bind_index<ygg::Index<formalism::InitialFunctionValue>>(m, "InitialFunctionValueIndex");
    ygg::bind_index<ygg::Index<formalism::Domain>>(m, "DomainIndex");
    ygg::bind_index<ygg::Index<formalism::Task>>(m, "TaskIndex");
}

}  // namespace loki::formalism
