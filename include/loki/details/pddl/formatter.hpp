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

// StreamWriter needs complete definition
#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/atom.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/effects.hpp"
#include "loki/details/pddl/function.hpp"
#include "loki/details/pddl/function_expressions.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/numeric_fluent.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/position.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/term.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/pddl/variable.hpp"

#include <concepts>
#include <cstddef>
#include <ostream>
#include <sstream>
#include <type_traits>

namespace loki
{

struct DefaultFormatterOptions
{
    // The indentation in the current level.
    int indent = 0;
    // The amount of indentation added per nesting
    int add_indent = 0;
};

class DefaultFormatter
{
public:
    void write(const ActionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const AtomImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const AxiomImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionLiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionAndImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionOrImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionNotImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionImplyImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionExistsImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionForallImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ConditionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const DomainImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectLiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectAndImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectNumericImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectConditionalForallImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectConditionalWhenImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const EffectImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionNumberImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionBinaryOperatorImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionMultiOperatorImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionMinusImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionFunctionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionExpressionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionSkeletonImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const FunctionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const LiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const OptimizationMetricImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const NumericFluentImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ObjectImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ParameterImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const PredicateImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const ProblemImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const RequirementsImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const TermObjectImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const TermVariableImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const TermImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const TypeImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
    void write(const VariableImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const;
};

/// @brief `Writer` is a utility class to write to a stream using `operator<<`
template<typename T, typename Formatter = DefaultFormatter, typename FormatterOptions = DefaultFormatterOptions>
class StreamWriter
{
private:
    const T& m_element;
    const Formatter& m_formatter;
    FormatterOptions m_options;

public:
    StreamWriter(const T& element, const Formatter& formatter = Formatter(), const FormatterOptions& options = FormatterOptions()) :
        m_element(element),
        m_formatter(formatter),
        m_options(options)
    {
    }

    std::string str() const
    {
        std::stringstream ss;
        m_formatter.write(m_element, m_options, ss);
        return ss.str();
    }

    const T& get_element() const { return m_element; }
    const Formatter& get_formatter() const { return m_formatter; }
    const FormatterOptions& get_options() const { return m_options; }
};

// Define the operator<< function outside of the class template
template<typename T, typename Formatter, typename FormatterOptions>
std::ostream& operator<<(std::ostream& out, const StreamWriter<T, Formatter, FormatterOptions>& writer)
{
    writer.get_formatter().write(writer.get_element(), writer.get_options(), out);
    return out;
}

}

#endif