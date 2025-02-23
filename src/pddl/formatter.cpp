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
void write_untyped<AddressTag>(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_untyped<AddressTag>(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_untyped<AddressTag>(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

template<>
void write_typed<AddressTag>(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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
                write_untyped<AddressTag>(*element.get_bases()[i], out, indent, add_indent);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<AddressTag>(*element.get_bases().front(), out, indent, add_indent);
        }
    }
}

template<>
void write_typed<AddressTag>(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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
                write_untyped<AddressTag>(*element.get_bases()[i], out, indent, add_indent);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<AddressTag>(*element.get_bases().front(), out, indent, add_indent);
        }
    }
}

template<>
void write_typed<AddressTag>(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << reinterpret_cast<uintptr_t>(&element);
}

/**
 * Generic templates
 */

template<StringOrAddress T>
void write(const ActionImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << std::string(indent, ' ') << "(:action " << element.get_name() << "\n";

    indent += add_indent;

    out << std::string(indent, ' ') << ":parameters (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ")";
    out << "\n";
    out << std::string(indent, ' ') << ":conditions ";
    if (element.get_condition().has_value())
        write<T>(*element.get_condition().value(), out, indent, add_indent);
    else
        out << "()";

    out << "\n";
    out << std::string(indent, ' ') << ":effects ";
    if (element.get_effect().has_value())
        write<T>(*element.get_effect().value(), out, indent, add_indent);
    else
        out << "()";
    out << ")\n";

    indent -= add_indent;
}

template void write<StringTag>(const ActionImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ActionImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const AtomImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << element.get_predicate()->get_name();
    for (size_t i = 0; i < element.get_terms().size(); ++i)
    {
        out << " ";
        write_untyped<T>(*element.get_terms()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const AtomImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const AtomImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const AxiomImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << std::string(indent, ' ') << "(:derived " << element.get_literal()->get_atom()->get_predicate()->get_name();
    for (size_t i = 0; i < element.get_literal()->get_atom()->get_terms().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << "\n";

    indent += add_indent;

    out << std::string(indent, ' ');
    write<T>(*element.get_condition(), out, indent, add_indent);
    out << ")\n";

    indent -= add_indent;
}

template void write<StringTag>(const AxiomImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const AxiomImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    write<T>(*element.get_literal(), out, indent, add_indent);
}

template void write<StringTag>(const ConditionLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionAndImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_conditions()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const ConditionAndImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionAndImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionOrImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(or ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_conditions()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const ConditionOrImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionOrImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionNotImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(not ";
    write<T>(*element.get_condition(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const ConditionNotImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionNotImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionImplyImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(imply ";
    write<T>(*element.get_condition_left(), out, indent, add_indent);
    out << " ";
    write<T>(*element.get_condition_right(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const ConditionImplyImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionImplyImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionExistsImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(exists (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ") ";
    write<T>(*element.get_condition(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const ConditionExistsImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionExistsImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionForallImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ") ";
    write<T>(*element.get_condition(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const ConditionForallImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionForallImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionNumericConstraintImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << to_string(element.get_binary_comparator()) << " ";
    write<T>(*element.get_function_expression_left(), out, indent, add_indent);
    out << " ";
    write<T>(*element.get_function_expression_right(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const ConditionNumericConstraintImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionNumericConstraintImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ConditionImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    std::visit([&](const auto& arg) { write<T>(*arg, out, indent, add_indent); }, element.get_condition());
}

template void write<StringTag>(const ConditionImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ConditionImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const DomainImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << std::string(indent, ' ') << "(define (domain " << element.get_name() << ")\n";

    indent += add_indent;

    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(indent, ' ');
        write<T>(*element.get_requirements(), out, indent, add_indent);
        out << "\n";
    }
    if (!element.get_types().empty())
    {
        out << std::string(indent, ' ') << "(:types ";
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
                out << "\n" << std::string(indent, ' ');
            for (size_t i = 0; i < sub_types.size(); ++i)
            {
                if (i != 0)
                    out << " ";

                write_untyped<T>(*sub_types[i], out, indent, add_indent);
            }
            out << " - ";
            if (types.size() > 1)
            {
                out << "(either ";
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    write_untyped<T>(*types[i], out, indent, add_indent);
                }
                out << ")";
            }
            else if (types.size() == 1)
            {
                write_untyped<T>(*types.front(), out, indent, add_indent);
            }
            ++i;
        }
        out << ")\n";
    }
    if (!element.get_constants().empty())
    {
        out << std::string(indent, ' ') << "(:constants ";
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> constants_by_types;
        for (const auto& constant : element.get_constants())
        {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t j = 0;
        for (const auto& pair : constants_by_types)
        {
            if (j != 0)
                out << "\n" << std::string(indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                if (i < constants.size() - 1 || !element.get_requirements()->test(RequirementEnum::TYPING))
                {
                    write_untyped<T>(*constants[i], out, indent, add_indent);
                }
                else
                {
                    write_typed<T>(*constants[i], out, indent, add_indent);
                }
            }
            ++j;
        }
        out << ")\n";
    }
    if (!element.get_predicates().empty())
    {
        out << std::string(indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_predicates()[i], out, indent, add_indent);
        }
        out << ")\n";
    }
    if (!element.get_functions().empty())
    {
        out << std::string(indent, ' ') << "(:functions ";
        for (size_t i = 0; i < element.get_functions().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_functions()[i], out, indent, add_indent);
        }
        out << ")\n";
    }

    for (const auto& action : element.get_actions())
    {
        write<T>(*action, out, indent, add_indent);
    }

    for (const auto& axiom : element.get_axioms())
    {
        write<T>(*axiom, out, indent, add_indent);
    }

    indent -= add_indent;

    out << std::string(indent, ' ') << ")";
}

template void write<StringTag>(const DomainImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const DomainImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    write<T>(*element.get_literal(), out, indent, add_indent);
}

template void write<StringTag>(const EffectLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectLiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectAndImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_effects()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const EffectAndImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectAndImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectNumericImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << to_string(element.get_assign_operator()) << " ";
    write<T>(*element.get_function(), out, indent, add_indent);
    out << " ";
    write<T>(*element.get_function_expression(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const EffectNumericImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectNumericImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectCompositeForallImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ") ";
    write<T>(*element.get_effect(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const EffectCompositeForallImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectCompositeForallImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectCompositeWhenImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(when ";
    write<T>(*element.get_condition(), out, indent, add_indent);
    out << " ";
    write<T>(*element.get_effect(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const EffectCompositeWhenImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectCompositeWhenImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectCompositeOneofImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(oneof ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write<T>(*element.get_effects()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const EffectCompositeOneofImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectCompositeOneofImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectCompositeProbabilisticImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(probabilistic ";
    for (size_t i = 0; i < element.get_effect_distribution().size(); ++i)
    {
        if (i != 0)
            out << " ";

        const auto& [probability, possibility] = element.get_effect_distribution()[i];

        out << probability << " ";
        write<T>(*possibility, out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const EffectCompositeProbabilisticImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectCompositeProbabilisticImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const EffectImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    std::visit([&](const auto& arg) { write<T>(*arg, out, indent, add_indent); }, element.get_effect());
}

template void write<StringTag>(const EffectImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const EffectImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionNumberImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << element.get_number();
}

template void write<StringTag>(const FunctionExpressionNumberImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionNumberImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << to_string(element.get_binary_operator()) << " ";
    write<T>(*element.get_left_function_expression(), out, indent, add_indent);
    out << " ";
    write<T>(*element.get_right_function_expression(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << to_string(element.get_multi_operator());
    assert(!element.get_function_expressions().empty());
    for (const auto& function_expression : element.get_function_expressions())
    {
        out << " ";
        write<T>(*function_expression, out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionMinusImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(- ";
    write<T>(*element.get_function_expression(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const FunctionExpressionMinusImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionMinusImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionFunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    write<T>(*element.get_function(), out, indent, add_indent);
}

template void write<StringTag>(const FunctionExpressionFunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionFunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionExpressionImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    std::visit([&](const auto& arg) { write<T>(*arg, out, indent, add_indent); }, element.get_function_expression());
}

template void write<StringTag>(const FunctionExpressionImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionExpressionImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionSkeletonImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const FunctionSkeletonImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionSkeletonImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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
            write_untyped<T>(*element.get_terms()[i], out, indent, add_indent);
        }
        out << "))";
    }
}

template void write<StringTag>(const FunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const LiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    if (element.is_negated())
    {
        out << "(not ";
        write<T>(*element.get_atom(), out, indent, add_indent);
        out << ")";
    }
    else
    {
        write<T>(*element.get_atom(), out, indent, add_indent);
    }
}

template void write<StringTag>(const LiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const LiteralImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const OptimizationMetricImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << to_string(element.get_optimization_metric()) << " ";
    write<T>(*element.get_function_expression(), out, indent, add_indent);
    out << ")";
}

template void write<StringTag>(const OptimizationMetricImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const OptimizationMetricImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const FunctionValueImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(= ";
    write<T>(*element.get_function(), out, indent, add_indent);
    out << " " << element.get_number() << ")";
}

template void write<StringTag>(const FunctionValueImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const FunctionValueImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ParameterImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    write_untyped<T>(*element.get_variable(), out, indent, add_indent);
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
                write_untyped<T>(*element.get_bases()[i], out, indent, add_indent);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), out, indent, add_indent);
        }
    }
}

template void write<StringTag>(const ParameterImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ParameterImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const PredicateImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write<T>(*element.get_parameters()[i], out, indent, add_indent);
    }
    out << ")";
}

template void write<StringTag>(const PredicateImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const PredicateImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const ProblemImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << std::string(indent, ' ') << "(define (problem " << element.get_name() << ")\n";

    indent += add_indent;

    out << std::string(indent, ' ') << "(:domain " << element.get_domain()->get_name() << ")\n";
    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(indent, ' ');
        write<T>(*element.get_requirements(), out, indent, add_indent);
        out << "\n";
    }

    if (!element.get_objects().empty())
    {
        out << std::string(indent, ' ') << "(:objects ";
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> objects_by_types;
        for (const auto& object : element.get_objects())
        {
            objects_by_types[object->get_bases()].push_back(object);
        }
        size_t j = 0;
        for (const auto& [types, objects] : objects_by_types)
        {
            if (j != 0)
                out << "\n" << std::string(indent, ' ');
            for (size_t i = 0; i < objects.size(); ++i)
            {
                if (i != 0)
                {
                    out << " ";
                }
                if (i < objects.size() - 1 || !element.get_domain()->get_requirements()->test(RequirementEnum::TYPING))
                {
                    write_untyped<T>(*objects[i], out, indent, add_indent);
                }
                else
                {
                    write_typed<T>(*objects[i], out, indent, add_indent);
                }
            }
            ++j;
        }
        out << ")\n";
    }

    if (!element.get_predicates().empty())
    {
        out << std::string(indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_predicates()[i], out, indent, add_indent);
        }
        out << ")\n";
    }

    if (!(element.get_initial_literals().empty() && element.get_function_values().empty()))
    {
        out << std::string(indent, ' ') << "(:init ";
        for (size_t i = 0; i < element.get_initial_literals().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write<T>(*element.get_initial_literals()[i], out, indent, add_indent);
        }
        for (size_t i = 0; i < element.get_function_values().size(); ++i)
        {
            out << " ";
            write<T>(*element.get_function_values()[i], out, indent, add_indent);
        }
    }
    out << ")\n";

    if (element.get_goal_condition().has_value())
    {
        out << std::string(indent, ' ') << "(:goal ";
        write<T>(*element.get_goal_condition().value(), out, indent, add_indent);
        out << ")\n";
    }

    if (element.get_optimization_metric().has_value())
    {
        out << std::string(indent, ' ') << "(:metric ";
        write<T>(*element.get_optimization_metric().value(), out, indent, add_indent);
        out << ")\n";
    }

    for (const auto& axiom : element.get_axioms())
    {
        write<T>(*axiom, out, indent, add_indent);
    }

    indent -= add_indent;

    out << std::string(indent, ' ') << ")";
}

template void write<StringTag>(const ProblemImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const ProblemImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write(const RequirementsImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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

template void write<StringTag>(const RequirementsImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write<AddressTag>(const RequirementsImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_untyped(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << element.get_name();
}

template void write_untyped<StringTag>(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_untyped(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    std::visit([&](const auto& arg) { write_untyped<T>(*arg, out, indent, add_indent); }, element.get_object_or_variable());
}

template void write_untyped<StringTag>(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write_untyped<AddressTag>(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_untyped(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << element.get_name();
}

template void write_untyped<StringTag>(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_untyped(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << element.get_name();
}

template void write_untyped<StringTag>(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_typed(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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
                write_untyped<T>(*element.get_bases()[i], out, indent, add_indent);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), out, indent, add_indent);
        }
    }
}

template void write_typed<StringTag>(const TypeImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_typed(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    std::visit([&](const auto& arg) { write_typed<T>(*arg, out, indent, add_indent); }, element.get_object_or_variable());
}

template void write_typed<StringTag>(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent);
template void write_typed<AddressTag>(const TermImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_typed(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent)
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
                write_untyped<T>(*element.get_bases()[i], out, indent, add_indent);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write_untyped<T>(*element.get_bases().front(), out, indent, add_indent);
        }
    }
}

template void write_typed<StringTag>(const ObjectImpl& element, std::ostream& out, size_t indent, size_t add_indent);

template<StringOrAddress T>
void write_typed(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent)
{
    out << element.get_name();
}

template void write_typed<StringTag>(const VariableImpl& element, std::ostream& out, size_t indent, size_t add_indent);

}