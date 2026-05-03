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

#include "loki/pddl/action.hpp"
#include "loki/pddl/atom.hpp"
#include "loki/pddl/axiom.hpp"
#include "loki/pddl/conditions.hpp"
#include "loki/pddl/domain.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/function.hpp"
#include "loki/pddl/function_expressions.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/function_value.hpp"
#include "loki/pddl/literal.hpp"
#include "loki/pddl/metric.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/problem.hpp"
#include "loki/pddl/requirements.hpp"
#include "loki/pddl/term.hpp"
#include "loki/pddl/type.hpp"
#include "loki/pddl/variable.hpp"

#include <concepts>
#include <cstddef>
#include <ostream>

namespace loki
{

struct StringFormatter
{
    size_t indent = 0;
    size_t add_indent = 4;
};
struct AddressFormatter
{
    size_t indent = 0;
    size_t add_indent = 4;
};

template<typename T>
concept Formatter = requires(T f) {
    { f.indent } -> std::convertible_to<std::size_t>;
    { f.add_indent } -> std::convertible_to<std::size_t>;
};

template<Formatter T>
void write(const ActionImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const AtomImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const AxiomImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionLiteralImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionAndImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionOrImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionNotImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionImplyImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionExistsImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionForallImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionNumericConstraintImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ConditionImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const DomainImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectLiteralImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectAndImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectNumericImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectCompositeForallImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectCompositeWhenImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectCompositeOneofImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectCompositeProbabilisticImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const EffectImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionNumberImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionBinaryOperatorImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionMultiOperatorImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionMinusImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionFunctionImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionExpressionImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionSkeletonImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const LiteralImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const OptimizationMetricImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const FunctionValueImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ParameterImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const PredicateImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ProblemImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const RequirementsImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const VariableImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const TypeImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const ObjectImpl& element, T formatter, std::ostream& out);
template<Formatter T>
void write(const TermImpl& element, T formatter, std::ostream& out);

}

#endif