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

#include "loki/details/pddl/formatter.hpp"

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
#include "loki/details/utils/equal_to.hpp"
#include "loki/details/utils/hash.hpp"

namespace loki
{

void DefaultFormatter::write(const ActionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };

    out << std::string(options.indent, ' ') << "(:action " << element.get_name() << "\n" << std::string(nested_options.indent, ' ') << ":parameters (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ")";
    out << "\n";
    out << std::string(nested_options.indent, ' ') << ":conditions ";
    if (element.get_condition().has_value())
        write(*element.get_condition().value(), options, out);
    else
        out << "()";

    out << "\n";
    out << std::string(nested_options.indent, ' ') << ":effects ";
    if (element.get_effect().has_value())
        write(*element.get_effect().value(), options, out);
    else
        out << "()";
    out << ")\n";
}

void DefaultFormatter::write(const AtomImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << element.get_predicate()->get_name();
    for (size_t i = 0; i < element.get_terms().size(); ++i)
    {
        out << " ";
        write(*element.get_terms()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const AxiomImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(options.indent, ' ') << "(:derived " << element.get_derived_predicate_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << "\n";
    out << std::string(nested_options.indent, ' ');
    write(*element.get_condition(), options, out);
    out << ")\n";
}

void DefaultFormatter::write(const ConditionLiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    write(*element.get_literal(), options, out);
}

void DefaultFormatter::write(const ConditionAndImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(and ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_conditions()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const ConditionOrImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(or ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_conditions()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const ConditionNotImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(not ";
    write(*element.get_condition(), options, out);
    out << ")";
}

void DefaultFormatter::write(const ConditionImplyImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(imply ";
    write(*element.get_condition_left(), options, out);
    out << " ";
    write(*element.get_condition_right(), options, out);
    out << ")";
}

void DefaultFormatter::write(const ConditionExistsImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(exists (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ") ";
    write(*element.get_condition(), options, out);
    out << ")";
}

void DefaultFormatter::write(const ConditionForallImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ") ";
    write(*element.get_condition(), options, out);
    out << ")";
}

void DefaultFormatter::write(const ConditionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    std::visit([this, &options, &out](const auto& arg) { this->write(arg, options, out); }, element);
}

void DefaultFormatter::write(const DomainImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << std::string(options.indent, ' ') << "(define (domain " << element.get_name() << ")\n";
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(nested_options.indent, ' ');
        write(*element.get_requirements(), nested_options, out);
        out << "\n";
    }
    if (!element.get_types().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:types ";
        std::unordered_map<TypeList, TypeList, Hasher<TypeList>> subtypes_by_parent_types;
        for (const auto& type : element.get_types())
        {
            // We do not want to print root type "object"
            if (!type->get_bases().empty())
            {
                subtypes_by_parent_types[type->get_bases()].push_back(type);
            }
        }
        size_t i = 0;
        for (const auto& [types, sub_types] : subtypes_by_parent_types)
        {
            if (i != 0)
                out << "\n" << std::string(nested_options.indent, ' ');
            for (size_t i = 0; i < sub_types.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                out << sub_types[i]->get_name();
            }
            out << " - ";
            if (types.size() > 1)
            {
                out << "(either ";
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    types[i]->get_name();
                }
                out << ")";
            }
            else if (types.size() == 1)
            {
                out << types.front()->get_name();
            }
            ++i;
        }
        out << ")\n";
    }
    if (!element.get_constants().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:constants ";
        std::unordered_map<TypeList, ObjectList, Hasher<TypeList>> constants_by_types;
        for (const auto& constant : element.get_constants())
        {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t i = 0;
        for (const auto& pair : constants_by_types)
        {
            if (i != 0)
                out << "\n" << std::string(nested_options.indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                write(*constants[i], nested_options, out);
            }
            if (element.get_requirements()->test(RequirementEnum::TYPING))
            {
                out << " - ";
                const auto& types = pair.first;
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    write(*types[i], nested_options, out);
                }
            }
            ++i;
        }
        out << ")\n";
    }
    if (!element.get_predicates().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_predicates()[i], nested_options, out);
        }
        out << ")\n";
    }
    if (!element.get_functions().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:functions ";
        for (size_t i = 0; i < element.get_functions().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_functions()[i], nested_options, out);
        }
    }

    for (const auto& action : element.get_actions())
    {
        write(*action, nested_options, out);
    }

    for (const auto& axiom : element.get_axioms())
    {
        write(*axiom, nested_options, out);
    }

    out << std::string(options.indent, ' ') << ")";
}

void DefaultFormatter::write(const EffectLiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    write(*element.get_literal(), options, out);
}

void DefaultFormatter::write(const EffectAndImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(and ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_effects()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const EffectNumericImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << to_string(element.get_assign_operator()) << " ";
    write(*element.get_function(), options, out);
    out << " ";
    write(*element.get_function_expression(), options, out);
    out << ")";
}

void DefaultFormatter::write(const EffectConditionalForallImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ") ";
    write(*element.get_effect(), options, out);
    out << ")";
}

void DefaultFormatter::write(const EffectConditionalWhenImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(when ";
    write(*element.get_condition(), options, out);
    out << " ";
    write(*element.get_effect(), options, out);
    out << ")";
}

void DefaultFormatter::write(const EffectImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    std::visit([this, &options, &out](const auto& arg) { this->write(arg, options, out); }, element);
}

void DefaultFormatter::write(const FunctionExpressionNumberImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << element.get_number();
}

void DefaultFormatter::write(const FunctionExpressionBinaryOperatorImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << to_string(element.get_binary_operator()) << " ";
    write(*element.get_left_function_expression(), options, out);
    out << " ";
    write(*element.get_right_function_expression(), options, out);
    out << ")";
}

void DefaultFormatter::write(const FunctionExpressionMultiOperatorImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << to_string(element.get_multi_operator());
    assert(!element.get_function_expressions().empty());
    for (const auto& function_expression : element.get_function_expressions())
    {
        out << " ";
        write(*function_expression, options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const FunctionExpressionMinusImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(- ";
    write(*element.get_function_expression(), options, out);
    out << ")";
}

void DefaultFormatter::write(const FunctionExpressionFunctionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    write(*element.get_function(), options, out);
}

void DefaultFormatter::write(const FunctionExpressionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    std::visit([this, &options, &out](const auto& arg) { this->write(arg, options, out); }, element);
}

void DefaultFormatter::write(const FunctionSkeletonImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const FunctionImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    if (element.get_terms().empty())
    {
        out << "(" << element.get_function_skeleton()->get_name() << ")";
    }
    else
    {
        out << "(" << element.get_function_skeleton()->get_name() << "(";
        for (size_t i = 0; i < element.get_terms().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_terms()[i], options, out);
        }
        out << "))";
    }
}

void DefaultFormatter::write(const LiteralImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    if (element.is_negated())
    {
        out << "(not ";
        write(*element.get_atom(), options, out);
        out << ")";
    }
    else
    {
        write(*element.get_atom(), options, out);
    }
}

void DefaultFormatter::write(const OptimizationMetricImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << to_string(element.get_optimization_metric()) << " ";
    write(*element.get_function_expression(), options, out);
    out << ")";
}

void DefaultFormatter::write(const NumericFluentImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(= ";
    write(*element.get_function(), options, out);
    out << " " << element.get_number() << ")";
}

void DefaultFormatter::write(const ObjectImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << element.get_name();
    if (!element.get_bases().empty())
    {
        out << " - ";
        if (element.get_bases().size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < element.get_bases().size(); ++i)
            {
                if (i != 0)
                    out << " ";
                write(*element.get_bases()[i], options, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write(*element.get_bases().front(), options, out);
        }
    }
}

void DefaultFormatter::write(const ParameterImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << element.get_variable()->get_name();
    if (!element.get_bases().empty())
    {
        out << " - ";
        if (element.get_bases().size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < element.get_bases().size(); ++i)
            {
                if (i != 0)
                    out << " ";
                out << element.get_bases()[i]->get_name();
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            out << element.get_bases().front()->get_name();
        }
    }
}

void DefaultFormatter::write(const PredicateImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], options, out);
    }
    out << ")";
}

void DefaultFormatter::write(const ProblemImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    out << std::string(options.indent, ' ') << "(define (problem " << element.get_name() << ")\n";
    auto nested_options = DefaultFormatterOptions { options.indent + options.add_indent, options.add_indent };
    out << std::string(nested_options.indent, ' ') << "(:domain " << element.get_domain()->get_name() << ")\n";
    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(nested_options.indent, ' ');
        write(*element.get_requirements(), options, out);
        out << "\n";
    }

    if (!element.get_objects().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:objects ";
        std::unordered_map<TypeList, ObjectList, Hasher<TypeList>> objects_by_types;
        for (const auto& object : element.get_objects())
        {
            objects_by_types[object->get_bases()].push_back(object);
        }
        size_t i = 0;
        for (const auto& [types, objects] : objects_by_types)
        {
            if (i != 0)
                out << "\n" << std::string(nested_options.indent, ' ');
            for (size_t i = 0; i < objects.size(); ++i)
            {
                if (i != 0)
                {
                    out << " ";
                }
                out << objects[i]->get_name();
            }
            if (element.get_requirements()->test(RequirementEnum::TYPING))
            {
                out << " - ";
                if (types.size() > 1)
                {
                    out << "(either ";
                    for (size_t i = 0; i < types.size(); ++i)
                    {
                        if (i != 0)
                            out << " ";
                        types[i]->get_name();
                    }
                    out << ")";
                }
                else if (types.size() == 1)
                {
                    out << types.front()->get_name();
                }
            }
            ++i;
        }
        out << ")\n";
    }

    if (!element.get_derived_predicates().empty())
    {
        out << std::string(nested_options.indent, ' ') << "(:derived-predicates ";
        for (size_t i = 0; i < element.get_derived_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_derived_predicates()[i], options, out);
        }
        out << ")\n";
    }

    if (!(element.get_initial_literals().empty() && element.get_numeric_fluents().empty()))
    {
        out << std::string(nested_options.indent, ' ') << "(:init ";
        for (size_t i = 0; i < element.get_initial_literals().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_initial_literals()[i], options, out);
        }
        for (size_t i = 0; i < element.get_numeric_fluents().size(); ++i)
        {
            out << " ";
            write(*element.get_numeric_fluents()[i], options, out);
        }
    }
    out << ")\n";

    if (element.get_goal_condition().has_value())
    {
        out << std::string(nested_options.indent, ' ') << "(:goal ";
        write(*element.get_goal_condition().value(), options, out);
        out << ")\n";
    }

    if (element.get_optimization_metric().has_value())
    {
        out << std::string(nested_options.indent, ' ') << "(:metric ";
        write(*element.get_optimization_metric().value(), options, out);
        out << ")\n";
    }

    for (const auto& axiom : element.get_axioms())
    {
        write(*axiom, options, out);
    }

    out << std::string(options.indent, ' ') << ")";
}

void DefaultFormatter::write(const RequirementsImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << "(:requirements ";
    int i = 0;
    for (const auto& requirement : element.get_requirements())
    {
        if (i != 0)
            out << " ";
        out << to_string(requirement);
        ++i;
    }
    out << ")";
}

void DefaultFormatter::write(const TermObjectImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << element.get_object()->get_name();
}

void DefaultFormatter::write(const TermVariableImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << element.get_variable()->get_name();
}

void DefaultFormatter::write(const TermImpl& element, const DefaultFormatterOptions& options, std::ostream& out) const
{
    std::visit([this, &options, &out](const auto& arg) { this->write(arg, options, out); }, element);
}

void DefaultFormatter::write(const TypeImpl& element, const DefaultFormatterOptions&, std::ostream& out) const
{
    out << element.get_name();
    if (!element.get_bases().empty())
    {
        out << " - ";
        if (element.get_bases().size() > 1)
        {
            out << "(either ";
            for (size_t i = 0; i < element.get_bases().size(); ++i)
            {
                if (i != 0)
                    out << " ";
                out << element.get_bases()[i]->get_name();
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            out << element.get_bases().front()->get_name();
        }
    }
}

void DefaultFormatter::write(const VariableImpl& element, const DefaultFormatterOptions&, std::ostream& out) const { out << element.get_name(); }

/**
 * StreamWriter
 */

template<typename T, typename Formatter, typename FormatterOptions>
StreamWriter<T, Formatter, FormatterOptions>::StreamWriter(const T& element, const Formatter& formatter, const FormatterOptions& options) :
    m_element(element),
    m_formatter(formatter),
    m_options(options)
{
}

template<typename T, typename Formatter, typename FormatterOptions>
std::string StreamWriter<T, Formatter, FormatterOptions>::str() const
{
    std::stringstream ss;
    m_formatter.write(m_element, m_options, ss);
    return ss.str();
}

template<typename T, typename Formatter, typename FormatterOptions>
const T& StreamWriter<T, Formatter, FormatterOptions>::get_element() const
{
    return m_element;
}

template<typename T, typename Formatter, typename FormatterOptions>
const Formatter& StreamWriter<T, Formatter, FormatterOptions>::get_formatter() const
{
    return m_formatter;
}

template<typename T, typename Formatter, typename FormatterOptions>
const FormatterOptions& StreamWriter<T, Formatter, FormatterOptions>::get_options() const
{
    return m_options;
}

// Explicit instantiation of StreamWriter for each specific type
template class StreamWriter<ActionImpl>;
template class StreamWriter<AtomImpl>;
template class StreamWriter<AxiomImpl>;
template class StreamWriter<ConditionLiteralImpl>;
template class StreamWriter<ConditionAndImpl>;
template class StreamWriter<ConditionOrImpl>;
template class StreamWriter<ConditionNotImpl>;
template class StreamWriter<ConditionImplyImpl>;
template class StreamWriter<ConditionExistsImpl>;
template class StreamWriter<ConditionForallImpl>;
template class StreamWriter<ConditionImpl>;
template class StreamWriter<DomainImpl>;
template class StreamWriter<EffectLiteralImpl>;
template class StreamWriter<EffectAndImpl>;
template class StreamWriter<EffectNumericImpl>;
template class StreamWriter<EffectConditionalForallImpl>;
template class StreamWriter<EffectConditionalWhenImpl>;
template class StreamWriter<EffectImpl>;
template class StreamWriter<FunctionExpressionNumberImpl>;
template class StreamWriter<FunctionExpressionBinaryOperatorImpl>;
template class StreamWriter<FunctionExpressionMultiOperatorImpl>;
template class StreamWriter<FunctionExpressionMinusImpl>;
template class StreamWriter<FunctionExpressionFunctionImpl>;
template class StreamWriter<FunctionExpressionImpl>;
template class StreamWriter<FunctionSkeletonImpl>;
template class StreamWriter<FunctionImpl>;
template class StreamWriter<LiteralImpl>;
template class StreamWriter<OptimizationMetricImpl>;
template class StreamWriter<NumericFluentImpl>;
template class StreamWriter<ObjectImpl>;
template class StreamWriter<ParameterImpl>;
template class StreamWriter<PredicateImpl>;
template class StreamWriter<ProblemImpl>;
template class StreamWriter<RequirementsImpl>;
template class StreamWriter<TermObjectImpl>;
template class StreamWriter<TermVariableImpl>;
template class StreamWriter<TermImpl>;
template class StreamWriter<TypeImpl>;
template class StreamWriter<VariableImpl>;

}