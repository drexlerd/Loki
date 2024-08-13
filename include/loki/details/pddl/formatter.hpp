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

#ifndef LOKI_INCLUDE_LOKI_PDDL_FORMATTER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_FORMATTER_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <cstddef>
#include <ostream>

namespace loki
{

class DefaultFormatter
{
private:
    // The indentation in the current level.
    size_t m_indent;
    // The amount of indentation added per nesting
    size_t m_add_indent;

public:
    DefaultFormatter(size_t indent, size_t add_indent);

    void write(const ActionImpl& element, std::ostream& out);
    void write(const AtomImpl& element, std::ostream& out);
    void write(const AxiomImpl& element, std::ostream& out);
    void write(const ConditionLiteralImpl& element, std::ostream& out);
    void write(const ConditionAndImpl& element, std::ostream& out);
    void write(const ConditionOrImpl& element, std::ostream& out);
    void write(const ConditionNotImpl& element, std::ostream& out);
    void write(const ConditionImplyImpl& element, std::ostream& out);
    void write(const ConditionExistsImpl& element, std::ostream& out);
    void write(const ConditionForallImpl& element, std::ostream& out);
    void write(const ConditionImpl& element, std::ostream& out);
    void write(const DomainImpl& element, std::ostream& out);
    void write(const EffectLiteralImpl& element, std::ostream& out);
    void write(const EffectAndImpl& element, std::ostream& out);
    void write(const EffectNumericImpl& element, std::ostream& out);
    void write(const EffectConditionalForallImpl& element, std::ostream& out);
    void write(const EffectConditionalWhenImpl& element, std::ostream& out);
    void write(const EffectImpl& element, std::ostream& out);
    void write(const FunctionExpressionNumberImpl& element, std::ostream& out);
    void write(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out);
    void write(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out);
    void write(const FunctionExpressionMinusImpl& element, std::ostream& out);
    void write(const FunctionExpressionFunctionImpl& element, std::ostream& out);
    void write(const FunctionExpressionImpl& element, std::ostream& out);
    void write(const FunctionSkeletonImpl& element, std::ostream& out);
    void write(const FunctionImpl& element, std::ostream& out);
    void write(const LiteralImpl& element, std::ostream& out);
    void write(const OptimizationMetricImpl& element, std::ostream& out);
    void write(const NumericFluentImpl& element, std::ostream& out);
    void write(const ObjectImpl& element, std::ostream& out);
    void write(const ParameterImpl& element, std::ostream& out);
    void write(const PredicateImpl& element, std::ostream& out);
    void write(const ProblemImpl& element, std::ostream& out);
    void write(const RequirementsImpl& element, std::ostream& out);
    void write(const TermObjectImpl& element, std::ostream& out);
    void write(const TermVariableImpl& element, std::ostream& out);
    void write(const TermImpl& element, std::ostream& out);
    void write(const TypeImpl& element, std::ostream& out);
    void write(const VariableImpl& element, std::ostream& out);
};

/// @brief `Writer` is a utility class to write to a stream using `operator<<`
template<typename T, typename Formatter = DefaultFormatter>
class Writer
{
private:
    const Formatter& element;
    const Writer& writer;

public:
    Writer(const T& element, const Formatter& formatter);

    friend std::ostream& operator<<(std::ostream& out, const Formatter& formatter);
};

}

#endif