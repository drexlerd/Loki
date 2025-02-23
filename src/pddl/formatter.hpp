/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_SRC_LOKI_PDDL_FORMATTER_HPP_
#define LOKI_SRC_LOKI_PDDL_FORMATTER_HPP_

#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/function_value.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"

#include <concepts>
#include <cstddef>
#include <ostream>

namespace loki
{

struct StringTag
{
};
struct AddressTag
{
};

template<typename T>
concept StringOrAddress = std::same_as<T, StringTag> || std::same_as<T, AddressTag>;

template<StringOrAddress T>
void write(const ActionImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const AtomImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const AxiomImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionLiteralImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionAndImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionOrImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionNotImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionImplyImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionExistsImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionForallImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionNumericConstraintImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ConditionImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const DomainImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectLiteralImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectAndImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectNumericImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectCompositeForallImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectCompositeWhenImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectCompositeOneofImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectCompositeProbabilisticImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const EffectImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionNumberImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionMinusImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionFunctionImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionExpressionImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionSkeletonImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const LiteralImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const OptimizationMetricImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const FunctionValueImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ParameterImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const PredicateImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const ProblemImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write(const RequirementsImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);

template<StringOrAddress T>
void write_untyped(const TypeImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_untyped(const TermImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_untyped(const ObjectImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_untyped(const VariableImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);

template<StringOrAddress T>
void write_typed(const TypeImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_typed(const TermImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_typed(const ObjectImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);
template<StringOrAddress T>
void write_typed(const VariableImpl& element, std::ostream& out, size_t indent = 0, size_t add_indent = 4);

}

#endif