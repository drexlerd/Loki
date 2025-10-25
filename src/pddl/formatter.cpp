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
#include "loki/details/utils/ostream.hpp"

#include <cassert>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <sstream>

namespace loki
{

/**
 * Helpers to materialize strings
 */

template<Formatter F, typename T>
std::string string(const T& element, F formatter)
{
    std::stringstream ss;
    if constexpr (std::is_pointer_v<std::decay_t<decltype(element)>>)
        write(*element, formatter, ss);
    else
        write(element, formatter, ss);
    return ss.str();
}

template<Formatter F, std::ranges::input_range Range>
std::vector<std::string> strings(const Range& range, F formatter)
{
    auto result = std::vector<std::string> {};
    if constexpr (std::ranges::sized_range<Range>)
        result.reserve(std::ranges::size(range));
    for (const auto& element : range)
        result.push_back(string(element, formatter));
    return result;
}

/**
 * Generic templates
 */

template<Formatter T>
void write(const ActionImpl& element, T formatter, std::ostream& out)
{
    // Header
    fmt::print(out, "(:action {}\n", element.get_name());

    formatter.indent += formatter.add_indent;
    auto indent = std::string(formatter.indent, ' ');

    // Parameters
    if (element.get_parameters().empty())
        fmt::print(out, "{}:parameters ()\n", indent);
    else
        fmt::print(out, "{}:parameters ({})\n", indent, fmt::join(strings(element.get_parameters(), formatter), " "));

    // Conditions
    if (element.get_condition().has_value())
        fmt::print(out, "{}:precondition {}\n", indent, string(element.get_condition().value(), formatter));
    else
        fmt::print(out, "{}:precondition ()\n", indent);

    // Effects
    if (element.get_effect().has_value())
        fmt::print(out, "{}:effect {}\n", indent, string(element.get_effect().value(), formatter));
    else
        fmt::print(out, "{}:effect ()\n", indent);

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End action
    fmt::print(out, "{})", indent);
}

template void write<StringFormatter>(const ActionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ActionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const AtomImpl& element, T formatter, std::ostream& out)
{
    if (element.get_terms().empty())
        fmt::print(out, "({})", element.get_predicate()->get_name());
    else
        fmt::print(out, "({} {})", element.get_predicate()->get_name(), fmt::join(strings(element.get_terms(), formatter), " "));
}

template void write<StringFormatter>(const AtomImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const AtomImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const AxiomImpl& element, T formatter, std::ostream& out)
{
    // Header line: "(:derived <pred> <params...\n"
    const auto& predicate_name = element.get_literal()->get_atom()->get_predicate()->get_name();
    if (element.get_parameters().empty())
        fmt::print(out, "(:derived ({})\n", predicate_name);
    else
        fmt::print(out, "(:derived ({} {})\n", predicate_name, fmt::join(strings(element.get_parameters(), formatter), " "));

    formatter.indent += formatter.add_indent;
    auto indent = std::string(formatter.indent, ' ');

    // Conditions
    fmt::print(out, "{}{}\n", indent, string(element.get_condition(), formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End axiom
    fmt::print(out, "{})", indent);
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
    fmt::print(out, "(and {})", fmt::join(strings(element.get_conditions(), formatter), " "));
}

template void write<StringFormatter>(const ConditionAndImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionAndImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionOrImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(or {})", fmt::join(strings(element.get_conditions(), formatter), " "));
}

template void write<StringFormatter>(const ConditionOrImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionOrImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionNotImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(not {})", string(element.get_condition(), formatter));
}

template void write<StringFormatter>(const ConditionNotImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionNotImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionImplyImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(imply {} {})", string(element.get_left_condition(), formatter), string(element.get_right_condition(), formatter));
}

template void write<StringFormatter>(const ConditionImplyImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionImplyImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionExistsImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(exists ({}) {})", fmt::join(strings(element.get_parameters(), formatter), " "), string(element.get_condition(), formatter));
}

template void write<StringFormatter>(const ConditionExistsImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionExistsImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionForallImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", fmt::join(strings(element.get_parameters(), formatter), " "), string(element.get_condition(), formatter));
}

template void write<StringFormatter>(const ConditionForallImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ConditionForallImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ConditionNumericConstraintImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               to_string(element.get_binary_comparator()),
               string(element.get_left_function_expression(), formatter),
               string(element.get_right_function_expression(), formatter));
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
    auto indent = std::string(formatter.indent, ' ');

    // Header
    fmt::print(out, "{}(define (domain {})\n", indent, element.get_name());

    formatter.indent += formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // Requirements
    if (!element.get_requirements()->get_requirements().empty())
        fmt::print(out, "{}{}\n", indent, string(element.get_requirements(), formatter));

    // Types
    if (!element.get_types().empty())
    {
        std::unordered_map<TypeList, TypeList, Hash<TypeList>> subtypes_by_parent_types;
        for (const auto& type : element.get_types())
            if (!type->get_bases().empty())
                subtypes_by_parent_types[type->get_bases()].push_back(type);

        fmt::print(out, "{}(:types\n", indent);

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& [types, sub_types] : subtypes_by_parent_types)
            if (types.size() > 1)
                fmt::print(out, "{}{} - (either {})\n", indent, fmt::join(strings(sub_types, formatter), " "), fmt::join(strings(types, formatter), " "));
            else
                fmt::print(out, "{}{} - {}\n", indent, fmt::join(strings(sub_types, formatter), " "), fmt::join(strings(types, formatter), " "));

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // Constants
    if (!element.get_constants().empty())
    {
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> constants_by_types;
        for (const auto& constant : element.get_constants())
            constants_by_types[constant->get_bases()].push_back(constant);

        fmt::print(out, "{}(:constants\n", indent);

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& [types, constants] : constants_by_types)
        {
            if (!element.get_requirements()->test(RequirementEnum::TYPING))
                fmt::print(out, "{}{}\n", indent, fmt::join(strings(constants, formatter), " "));
            else if (types.size() > 1)
                fmt::print(out, "{}{} - (either {})\n", indent, fmt::join(strings(constants, formatter), " "), fmt::join(strings(types, formatter), " "));
            else
                fmt::print(out, "{}{} - {}\n", indent, fmt::join(strings(constants, formatter), " "), fmt::join(strings(types, formatter), " "));
        }

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // Predicates
    if (!element.get_predicates().empty())
    {
        fmt::print(out, "{}(:predicates\n", std::string(formatter.indent, ' '));

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& predicate : element.get_predicates())
            if (predicate->get_name() != "=")
                fmt::print(out, "{}{}\n", indent, string(predicate, formatter));

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // FunctionSkeletons
    if (!element.get_function_skeletons().empty())
    {
        fmt::print(out, "{}(:functions\n", std::string(formatter.indent, ' '));

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& function_skeleton : element.get_function_skeletons())
            fmt::print(out, "{}{}\n", indent, string(function_skeleton, formatter));

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    for (const auto& action : element.get_actions())
        fmt::print(out, "{}{}\n", indent, string(action, formatter));

    for (const auto& axiom : element.get_axioms())
        fmt::print(out, "{}{}\n", indent, string(axiom, formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End domain
    fmt::print(out, "{})", indent);
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
    fmt::print(out, "(and {})", fmt::join(strings(element.get_effects(), formatter), " "));
}

template void write<StringFormatter>(const EffectAndImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectAndImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectNumericImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out,
               "({} {} {})",
               to_string(element.get_assign_operator()),
               string(element.get_function(), formatter),
               string(element.get_function_expression(), formatter));
}

template void write<StringFormatter>(const EffectNumericImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectNumericImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeForallImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(forall ({}) {})", fmt::join(strings(element.get_parameters(), formatter), " "), string(element.get_effect(), formatter));
}

template void write<StringFormatter>(const EffectCompositeForallImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeForallImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeWhenImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(when {} {})", string(element.get_condition(), formatter), string(element.get_effect(), formatter));
}

template void write<StringFormatter>(const EffectCompositeWhenImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeWhenImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeOneofImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(oneof {})", fmt::join(strings(element.get_effects(), formatter), " "));
}

template void write<StringFormatter>(const EffectCompositeOneofImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const EffectCompositeOneofImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const EffectCompositeProbabilisticImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out,
               "(probabilistic {})",
               fmt::join(element.get_effect_distribution()
                             | std::views::transform([&](const auto& pair) { return fmt::format("{} {}", pair.first, string(pair.second, formatter)); }),
                         " "));
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
    fmt::print(out,
               "({} {} {})",
               to_string(element.get_binary_operator()),
               string(element.get_left_function_expression(), formatter),
               string(element.get_right_function_expression(), formatter));
}

template void write<StringFormatter>(const FunctionExpressionBinaryOperatorImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionBinaryOperatorImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionMultiOperatorImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "({} {})", to_string(element.get_multi_operator()), fmt::join(strings(element.get_function_expressions(), formatter), " "));
}

template void write<StringFormatter>(const FunctionExpressionMultiOperatorImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionExpressionMultiOperatorImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionExpressionMinusImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(- {})", string(element.get_function_expression(), formatter));
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
    if (element.get_parameters().empty())
        fmt::print(out, "({})", element.get_name());
    else
        fmt::print(out, "({} {})", element.get_name(), fmt::join(strings(element.get_parameters(), formatter), " "));
}

template void write<StringFormatter>(const FunctionSkeletonImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionSkeletonImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionImpl& element, T formatter, std::ostream& out)
{
    if (element.get_terms().empty())
        fmt::print(out, "({})", element.get_function_skeleton()->get_name());
    else
        fmt::print(out, "({} {})", element.get_function_skeleton()->get_name(), fmt::join(strings(element.get_terms(), formatter), " "));
}

template void write<StringFormatter>(const FunctionImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const LiteralImpl& element, T formatter, std::ostream& out)
{
    if (!element.get_polarity())
        fmt::print(out, "(not {})", string(element.get_atom(), formatter));
    else
        write<T>(*element.get_atom(), formatter, out);
}

template void write<StringFormatter>(const LiteralImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const LiteralImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const OptimizationMetricImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "{} {}", to_string(element.get_optimization_metric()), string(element.get_function_expression(), formatter));
}

template void write<StringFormatter>(const OptimizationMetricImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const OptimizationMetricImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const FunctionValueImpl& element, T formatter, std::ostream& out)
{
    fmt::print(out, "(= {} {})", string(element.get_function(), formatter), element.get_number());
}

template void write<StringFormatter>(const FunctionValueImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const FunctionValueImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ParameterImpl& element, T formatter, std::ostream& out)
{
    write(*element.get_variable(), formatter, out);

    if (!element.get_bases().empty())
    {
        fmt::print(out, "{}", " - ");

        if (element.get_bases().size() > 1)
            fmt::print(out, "(either {})", fmt::join(strings(element.get_bases(), formatter), " "));
        else if (element.get_bases().size() == 1)
            fmt::print(out, "{}", string(element.get_bases().front(), formatter));
    }
}

template void write<StringFormatter>(const ParameterImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ParameterImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const PredicateImpl& element, T formatter, std::ostream& out)
{
    if (element.get_parameters().empty())
        fmt::print(out, "({})", element.get_name());
    else
        fmt::print(out, "({} {})", element.get_name(), fmt::join(strings(element.get_parameters(), formatter), " "));
}

template void write<StringFormatter>(const PredicateImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const PredicateImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ProblemImpl& element, T formatter, std::ostream& out)
{
    auto indent = std::string(formatter.indent, ' ');

    // Header
    fmt::print(out, "{}(define (problem {})\n", indent, element.get_name());

    formatter.indent += formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // Domain
    fmt::print(out, "{}(:domain {})\n", indent, element.get_domain()->get_name());

    // Requirements
    if (!element.get_requirements()->get_requirements().empty())
        fmt::print(out, "{}{}\n", indent, string(element.get_requirements(), formatter));

    // Constants
    if (!element.get_objects().empty())
    {
        std::unordered_map<TypeList, ObjectList, Hash<TypeList>> objects_by_types;
        for (const auto& constant : element.get_objects())
            objects_by_types[constant->get_bases()].push_back(constant);

        fmt::print(out, "{}(:objects\n", indent);

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& [types, objects] : objects_by_types)
        {
            if (!element.get_requirements()->test(RequirementEnum::TYPING))
                fmt::print(out, "{}{}\n", indent, fmt::join(strings(objects, formatter), " "));
            else if (types.size() > 1)
                fmt::print(out, "{}{} - (either {})\n", indent, fmt::join(strings(objects, formatter), " "), fmt::join(strings(types, formatter), " "));
            else
                fmt::print(out, "{}{} - {}\n", indent, fmt::join(strings(objects, formatter), " "), fmt::join(strings(types, formatter), " "));
        }

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // Predicates
    if (!element.get_predicates().empty())
    {
        fmt::print(out, "{}(:predicates\n", std::string(formatter.indent, ' '));

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& predicate : element.get_predicates())
            fmt::print(out, "{}{}\n", indent, string(predicate, formatter));

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // Init
    if (!(element.get_initial_literals().empty() && element.get_initial_function_values().empty()))
    {
        fmt::print(out, "{}(:init\n", indent);

        formatter.indent += formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        for (const auto& literal : element.get_initial_literals())
            fmt::print(out, "{}{}\n", indent, string(literal, formatter));

        for (const auto& function_value : element.get_initial_function_values())
            fmt::print(out, "{}{}\n", indent, string(function_value, formatter));

        formatter.indent -= formatter.add_indent;
        indent = std::string(formatter.indent, ' ');

        fmt::print(out, "{})\n", indent);
    }

    // Goal
    if (element.get_goal_condition().has_value())
        fmt::print(out, "{}(:goal {})\n", indent, string(element.get_goal_condition().value(), formatter));

    // Metric
    if (element.get_optimization_metric().has_value())
        fmt::print(out, "{}(:metric {})\n", indent, string(element.get_optimization_metric().value(), formatter));

    // Axioms
    for (const auto& axiom : element.get_axioms())
        fmt::print(out, "{}{}\n", indent, string(axiom, formatter));

    formatter.indent -= formatter.add_indent;
    indent = std::string(formatter.indent, ' ');

    // End problem
    fmt::print(out, "{})", indent);
}

template void write<StringFormatter>(const ProblemImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ProblemImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const RequirementsImpl& element, T, std::ostream& out)
{
    fmt::print(out,
               "(:requirements {})",
               fmt::join(element.get_requirements() | std::views::transform([](RequirementEnum r) { return loki::to_string(r); }), " "));
}

template void write<StringFormatter>(const RequirementsImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const RequirementsImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const VariableImpl& element, T, std::ostream& out)
{
    fmt::print(out, "{}", element.get_name());
}

template void write<StringFormatter>(const VariableImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const VariableImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const TypeImpl& element, T, std::ostream& out)
{
    fmt::print(out, "{}", element.get_name());
}

template void write<StringFormatter>(const TypeImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const TypeImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const ObjectImpl& element, T, std::ostream& out)
{
    fmt::print(out, "{}", element.get_name());
}

template void write<StringFormatter>(const ObjectImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const ObjectImpl& element, AddressFormatter formatter, std::ostream& out);

template<Formatter T>
void write(const TermImpl& element, T formatter, std::ostream& out)
{
    std::visit([&](const auto& arg) { write(*arg, formatter, out); }, element.get_object_or_variable());
}

template void write<StringFormatter>(const TermImpl& element, StringFormatter formatter, std::ostream& out);
template void write<AddressFormatter>(const TermImpl& element, AddressFormatter formatter, std::ostream& out);

}