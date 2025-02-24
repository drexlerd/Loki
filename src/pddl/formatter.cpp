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

#include "formatter.hpp"

#include "loki/details/utils/hash.hpp"

#include <cassert>
#include <sstream>

namespace loki
{

/**
 * Explicit templates
 */

template<>
void write_untyped<AddressFormatter>(const TypeImpl& element, AddressFormatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_untyped<AddressFormatter>(const ObjectImpl& element, AddressFormatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_untyped<AddressFormatter>(const VariableImpl& element, AddressFormatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_typed<AddressFormatter>(const TypeImpl& element, AddressFormatter formatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
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
                write_untyped<AddressFormatter>(*element.get_bases()[i], formatter, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<AddressFormatter>(*element.get_bases().front(), formatter, out);
        }
    }
}

template<>
void write_typed<AddressFormatter>(const ObjectImpl& element, AddressFormatter formatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
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
                write_untyped<AddressFormatter>(*element.get_bases()[i], formatter, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<AddressFormatter>(*element.get_bases().front(), formatter, out);
        }
    }
}

template<>
void write_typed<AddressFormatter>(const VariableImpl& element, AddressFormatter, std::ostream& out)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

/**
 * Generic templates
 */

template<Formatter T>
void write(const ActionImpl& element, T formatter, std::ostream& out)
{
    out << std::string(formatter.indent, ' ') << "(:action " << element.get_name() << "\n";

    formatter.indent += formatter.add_indent;

    out << std::string(formatter.indent, ' ') << ":parameters (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ")";
    out << "\n";
    out << std::string(formatter.indent, ' ') << ":conditions ";
    if (element.get_condition().has_value())
        write<T>(*element.get_condition().value(), formatter, out);
    else
        out << "()";

    out << "\n";
    out << std::string(formatter.indent, ' ') << ":effects ";
    if (element.get_effect().has_value())
        write<T>(*element.get_effect().value(), formatter, out);
    else
        out << "()";
    out << ")\n";
}

template void write<StringFormatter>(const ActionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ActionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const AtomImpl& element, T formatter, std::ostream& out)
{
    out << "(" << element.get_predicate()->get_name();
    for (size_t i = 0; i < element.get_terms().size(); ++i)
    {
        out << " ";
        write_untyped<T>(*element.get_terms()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const AtomImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const AtomImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const AxiomImpl& element, T formatter, std::ostream& out)
{
    out << std::string(formatter.indent, ' ') << "(:derived " << element.get_literal()->get_atom()->get_predicate()->get_name();
    for (size_t i = 0; i < element.get_literal()->get_atom()->get_terms().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << "\n";

    formatter.indent += formatter.add_indent;

    out << std::string(formatter.indent, ' ');
    write<T>(*element.get_condition(), formatter, out);
    out << ")\n";
}

template void write<StringFormatter>(const AxiomImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const AxiomImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionLiteralImpl& element, T formatter, std::ostream& out)
{
    write<T>(*element.get_literal(), formatter, out);
}

template void write<StringFormatter>(const ConditionLiteralImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionLiteralImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionAndImpl& element, T formatter, std::ostream& out)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_conditions()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const ConditionAndImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionAndImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionOrImpl& element, T formatter, std::ostream& out)
{
    out << "(or ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_conditions()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const ConditionOrImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionOrImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionNotImpl& element, T formatter, std::ostream& out)
{
    out << "(not ";
    write<T>(*element.get_condition(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const ConditionNotImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionNotImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionImplyImpl& element, T formatter, std::ostream& out)
{
    out << "(imply ";
    write<T>(*element.get_condition_left(), formatter, out);
    out << " ";
    write<T>(*element.get_condition_right(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const ConditionImplyImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionImplyImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionExistsImpl& element, T formatter, std::ostream& out)
{
    out << "(exists (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ") ";
    write<T>(*element.get_condition(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const ConditionExistsImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionExistsImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionForallImpl& element, T formatter, std::ostream& out)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ") ";
    write<T>(*element.get_condition(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const ConditionForallImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionForallImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionNumericConstraintImpl& element, T formatter, std::ostream& out)
{
    out << "(" << to_string(element.get_binary_comparator()) << " ";
    write<T>(*element.get_function_expression_left(), formatter, out);
    out << " ";
    write<T>(*element.get_function_expression_right(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const ConditionNumericConstraintImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionNumericConstraintImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write<T>(*arg, formatter, out); }, element.get_condition());
}

template void write<StringFormatter>(const ConditionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const DomainImpl& element, T formatter, std::ostream& out)
{
    out << std::string(formatter.indent, ' ') << "(define (domain " << element.get_name() << ")\n";

    formatter.indent += formatter.add_indent;

    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(formatter.indent, ' ');
        write<T>(*element.get_requirements(), formatter, out);
        out << "\n";
    }
    if (!element.get_types().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:types ";
        std::unordered_map<TypeList, TypeList, Hash<TypeList>> subtypes_by_parent_types;
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
                out << "\n" << std::string(formatter.indent, ' ');
            for (size_t i = 0; i < sub_types.size(); ++i)
            {
                if (i != 0)
                    out << " ";

                write_untyped<T>(*sub_types[i], formatter, out);
            }
            out << " - ";
            if (types.size() > 1)
            {
                out << "(either ";
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    write_untyped<T>(*types[i], formatter, out);
                }
                out << ")";
            }
            else if (types.size() == 1)
            {
                write_untyped<T>(*types.front(), formatter, out);
            }
            ++i;
        }
        out << ")\n";
    }
    if (!element.get_constants().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:constants ";
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> constants_by_types;
        for (const auto& constant : element.get_constants())
        {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t j = 0;
        for (const auto& pair : constants_by_types)
        {
            if (j != 0)
                out << "\n" << std::string(formatter.indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                if (i < constants.size() - 1 || !element.get_requirements()->test(RequirementEnum::TYPING))
                {
                    write_untyped<T>(*constants[i], formatter, out);
                }
                else
                {
                    write_typed<T>(*constants[i], formatter, out);
                }
            }
            ++j;
        }
        out << ")\n";
    }
    if (!element.get_predicates().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_predicates()[i], formatter, out);
        }
        out << ")\n";
    }
    if (!element.get_function_skeletons().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:functions ";
        for (size_t i = 0; i < element.get_function_skeletons().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_function_skeletons()[i], formatter, out);
        }
        out << ")\n";
    }

    for (const auto& action : element.get_actions())
    {
        write<T>(*action, formatter, out);
    }

    for (const auto& axiom : element.get_axioms())
    {
        write<T>(*axiom, formatter, out);
    }

    formatter.indent -= formatter.add_indent;

    out << std::string(formatter.indent, ' ') << ")";
}

template void write<StringFormatter>(const DomainImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const DomainImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectLiteralImpl& element, T formatter, std::ostream& out)
{
    write<T>(*element.get_literal(), formatter, out);
}

template void write<StringFormatter>(const EffectLiteralImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectLiteralImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectAndImpl& element, T formatter, std::ostream& out)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_effects()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const EffectAndImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectAndImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectNumericImpl& element, T formatter, std::ostream& out)
{
    out << "(" << to_string(element.get_assign_operator()) << " ";
    write<T>(*element.get_function(), formatter, out);
    out << " ";
    write<T>(*element.get_function_expression(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const EffectNumericImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectNumericImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeForallImpl& element, T formatter, std::ostream& out)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ") ";
    write<T>(*element.get_effect(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const EffectCompositeForallImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeForallImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeWhenImpl& element, T formatter, std::ostream& out)
{
    out << "(when ";
    write<T>(*element.get_condition(), formatter, out);
    out << " ";
    write<T>(*element.get_effect(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const EffectCompositeWhenImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeWhenImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeOneofImpl& element, T formatter, std::ostream& out)
{
    out << "(oneof ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_effects()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const EffectCompositeOneofImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeOneofImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeProbabilisticImpl& element, T formatter, std::ostream& out)
{
    out << "(probabilistic ";
    for (size_t i = 0; i < element.get_effect_distribution().size(); ++i)
    {
        if (i != 0)
            out << " ";

        const auto& [probability, possibility] = element.get_effect_distribution()[i];

        out << probability << " ";
        write<T>(*possibility, formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const EffectCompositeProbabilisticImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeProbabilisticImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write<T>(*arg, formatter, out); }, element.get_effect());
}

template void write<StringFormatter>(const EffectImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionNumberImpl& element, T, std::ostream& out)
{
    out << element.get_number();
}

template void write<StringFormatter>(const FunctionExpressionNumberImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionNumberImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionBinaryOperatorImpl& element, T formatter, std::ostream& out)
{
    out << "(" << to_string(element.get_binary_operator()) << " ";
    write<T>(*element.get_left_function_expression(), formatter, out);
    out << " ";
    write<T>(*element.get_right_function_expression(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const FunctionExpressionBinaryOperatorImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionBinaryOperatorImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionMultiOperatorImpl& element, T formatter, std::ostream& out)
{
    out << "(" << to_string(element.get_multi_operator());
    assert(!element.get_function_expressions().empty());
    for (const auto& function_expression : element.get_function_expressions())
    {
        out << " ";
        write<T>(*function_expression, formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const FunctionExpressionMultiOperatorImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionMultiOperatorImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionMinusImpl& element, T formatter, std::ostream& out)
{
    out << "(- ";
    write<T>(*element.get_function_expression(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const FunctionExpressionMinusImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionMinusImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionFunctionImpl& element, T formatter, std::ostream& out)
{
    write<T>(*element.get_function(), formatter, out);
}

template void write<StringFormatter>(const FunctionExpressionFunctionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionFunctionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write<T>(*arg, formatter, out); }, element.get_function_expression());
}

template void write<StringFormatter>(const FunctionExpressionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionSkeletonImpl& element, T formatter, std::ostream& out)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const FunctionSkeletonImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionSkeletonImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionImpl& element, T formatter, std::ostream& out)
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
            write_untyped<T>(*element.get_terms()[i], formatter, out);
        }
        out << "))";
    }
}

template void write<StringFormatter>(const FunctionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const LiteralImpl& element, T formatter, std::ostream& out)
{
    if (element.is_negated())
    {
        out << "(not ";
        write<T>(*element.get_atom(), formatter, out);
        out << ")";
    }
    else
    {
        write<T>(*element.get_atom(), formatter, out);
    }
}

template void write<StringFormatter>(const LiteralImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const LiteralImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const OptimizationMetricImpl& element, T formatter, std::ostream& out)
{
    out << "(" << to_string(element.get_optimization_metric()) << " ";
    write<T>(*element.get_function_expression(), formatter, out);
    out << ")";
}

template void write<StringFormatter>(const OptimizationMetricImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const OptimizationMetricImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionValueImpl& element, T formatter, std::ostream& out)
{
    out << "(= ";
    write<T>(*element.get_function(), formatter, out);
    out << " " << element.get_number() << ")";
}

template void write<StringFormatter>(const FunctionValueImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionValueImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ParameterImpl& element, T formatter, std::ostream& out)
{
    write_untyped<T>(*element.get_variable(), formatter, out);
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
                write_untyped<T>(*element.get_bases()[i], formatter, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), formatter, out);
        }
    }
}

template void write<StringFormatter>(const ParameterImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ParameterImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const PredicateImpl& element, T formatter, std::ostream& out)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], formatter, out);
    }
    out << ")";
}

template void write<StringFormatter>(const PredicateImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const PredicateImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ProblemImpl& element, T formatter, std::ostream& out)
{
    out << std::string(formatter.indent, ' ') << "(define (problem " << element.get_name() << ")\n";

    formatter.indent += formatter.add_indent;

    out << std::string(formatter.indent, ' ') << "(:domain " << element.get_domain()->get_name() << ")\n";
    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(formatter.indent, ' ');
        write<T>(*element.get_requirements(), formatter, out);
        out << "\n";
    }

    if (!element.get_objects().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:objects ";
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> objects_by_types;
        for (const auto& object : element.get_objects())
        {
            objects_by_types[object->get_bases()].push_back(object);
        }
        size_t j = 0;
        for (const auto& [types, objects] : objects_by_types)
        {
            if (j != 0)
                out << "\n" << std::string(formatter.indent, ' ');
            for (size_t i = 0; i < objects.size(); ++i)
            {
                if (i != 0)
                {
                    out << " ";
                }
                if (i < objects.size() - 1 || !element.get_domain()->get_requirements()->test(RequirementEnum::TYPING))
                {
                    write_untyped<T>(*objects[i], formatter, out);
                }
                else
                {
                    write_typed<T>(*objects[i], formatter, out);
                }
            }
            ++j;
        }
        out << ")\n";
    }

    if (!element.get_predicates().empty())
    {
        out << std::string(formatter.indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_predicates()[i], formatter, out);
        }
        out << ")\n";
    }

    if (!(element.get_initial_literals().empty() && element.get_initial_function_values().empty()))
    {
        out << std::string(formatter.indent, ' ') << "(:init ";
        for (size_t i = 0; i < element.get_initial_literals().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_initial_literals()[i], formatter, out);
        }
        for (size_t i = 0; i < element.get_initial_function_values().size(); ++i)
        {
            out << " ";
            write<T>(*element.get_initial_function_values()[i], formatter, out);
        }
    }
    out << ")\n";

    if (element.get_goal_condition().has_value())
    {
        out << std::string(formatter.indent, ' ') << "(:goal ";
        write<T>(*element.get_goal_condition().value(), formatter, out);
        out << ")\n";
    }

    if (element.get_optimization_metric().has_value())
    {
        out << std::string(formatter.indent, ' ') << "(:metric ";
        write<T>(*element.get_optimization_metric().value(), formatter, out);
        out << ")\n";
    }

    for (const auto& axiom : element.get_axioms())
    {
        write<T>(*axiom, formatter, out);
    }

    formatter.indent -= formatter.add_indent;

    out << std::string(formatter.indent, ' ') << ")";
}

template void write<StringFormatter>(const ProblemImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ProblemImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const RequirementsImpl& element, T, std::ostream& out)
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

template void write<StringFormatter>(const RequirementsImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const RequirementsImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write_untyped(const TypeImpl& element, T, std::ostream& out)
{
    out << element.get_name();
}

template void write_untyped<StringFormatter>(const TypeImpl& element, StringFormatter formatter, std::ostream& out);

template<Formatter T>
void write_untyped(const TermImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write_untyped<T>(*arg, formatter, out); }, element.get_object_or_variable());
}

template void write_untyped<StringFormatter>(const TermImpl& element, StringFormatter formatter, std::ostream& out);
template void write_untyped<AddressFormatter>(const TermImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write_untyped(const ObjectImpl& element, T, std::ostream& out)
{
    out << element.get_name();
}

template void write_untyped<StringFormatter>(const ObjectImpl& element, StringFormatter formatter, std::ostream& out);

template<Formatter T>
void write_untyped(const VariableImpl& element, T, std::ostream& out)
{
    out << element.get_name();
}

template void write_untyped<StringFormatter>(const VariableImpl& element, StringFormatter formatter, std::ostream& out);

template<Formatter T>
void write_typed(const TypeImpl& element, T formatter, std::ostream& out)
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
                write_untyped<T>(*element.get_bases()[i], formatter, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), formatter, out);
        }
    }
}

template void write_typed<StringFormatter>(const TypeImpl& element, StringFormatter formatter, std::ostream& out);

template<Formatter T>
void write_typed(const TermImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write_typed<T>(*arg, formatter, out); }, element.get_object_or_variable());
}

template void write_typed<StringFormatter>(const TermImpl& element, StringFormatter formatter, std::ostream& out);
template void write_typed<AddressFormatter>(const TermImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write_typed(const ObjectImpl& element, T formatter, std::ostream& out)
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
                write_untyped<T>(*element.get_bases()[i], formatter, out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), formatter, out);
        }
    }
}

template void write_typed<StringFormatter>(const ObjectImpl& element, StringFormatter formatter, std::ostream& out);

template<Formatter T>
void write_typed(const VariableImpl& element, T, std::ostream& out)
{
    out << element.get_name();
}

template void write_typed<StringFormatter>(const VariableImpl& element, StringFormatter formatter, std::ostream& out);

}