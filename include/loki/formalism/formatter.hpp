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

#ifndef LOKI_FORMALISM_FORMATTER_HPP_
#define LOKI_FORMALISM_FORMATTER_HPP_

#include "loki/formalism/repository.hpp"
#include "loki/formalism/views.hpp"

#include <cmath>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/formatting/cista_formatters.hpp>
#include <yggdrasil/io/iostream.hpp>

#if !YGG_ENABLE_FMT_FORMATTERS
#error "loki requires yggdrasil's fmt formatters (YGG_ENABLE_FMT_FORMATTERS=1)."
#endif

// Serialization lives in the plain loki::formalism::format::to_string overloads; the
// fmt::formatter specializations below are thin delegates to them.
namespace loki::formalism::format
{

std::string to_string(RequirementView value);
std::string to_string(TypeView value);
std::string to_string(ObjectView value);
std::string to_string(VariableView value);
std::string to_string(ParameterView value);
std::string to_string(PredicateView value);
std::string to_string(FunctionSkeletonView value);
std::string to_string(TermView value);
std::string to_string(AtomView value);
std::string to_string(LiteralView value);
std::string to_string(FunctionExpressionNumberView value);
std::string to_string(FunctionTermView value);
std::string to_string(UnaryFunctionExpressionView value);
std::string to_string(BinaryFunctionExpressionView value);
std::string to_string(MultiFunctionExpressionView value);
std::string to_string(FunctionExpressionView value);
std::string to_string(ConditionLiteralView value);
std::string to_string(ConditionAndView value);
std::string to_string(ConditionOrView value);
std::string to_string(ConditionNotView value);
std::string to_string(ConditionImplyView value);
std::string to_string(ConditionExistsView value);
std::string to_string(ConditionForallView value);
std::string to_string(ConditionNumericConstraintView value);
std::string to_string(ConditionView value);
std::string to_string(EffectLiteralView value);
std::string to_string(EffectAndView value);
std::string to_string(EffectNumericView value);
std::string to_string(EffectForallView value);
std::string to_string(EffectWhenView value);
std::string to_string(EffectOneOfView value);
std::string to_string(EffectProbabilisticAlternativeView value);
std::string to_string(EffectProbabilisticView value);
std::string to_string(EffectView value);
std::string to_string(ActionView value);
std::string to_string(AxiomView value);
std::string to_string(MetricView value);
std::string to_string(InitialFunctionValueView value);
std::string to_string(DomainView value);
std::string to_string(TaskView value);

namespace detail
{

inline bool is_builtin_type(TypeView type)
{
    const auto name = std::string_view(type.get_name().data(), type.get_name().size());
    return name == "object" || name == "number";
}

// Serializes a range as " e1 e2 ..." with a leading space per element; an empty range yields "".
template<typename Range>
inline std::string spaced(const Range& range)
{
    auto result = std::string {};
    for (auto element : range)
        fmt::format_to(std::back_inserter(result), " {}", to_string(element));
    return result;
}

// Renders " - type" or " - (either t1 t2 ...)"; multiple types require an either wrapper to reparse.
template<typename Types>
inline std::string type_annotation(const Types& types)
{
    if (types.size() == 1)
        return fmt::format(" - {}", to_string(types[0]));
    return fmt::format(" - (either{})", spaced(types));
}

}  // namespace detail

inline std::string to_string(RequirementView value) { return std::string(loki::formalism::to_string(value.get_kind())); }

inline std::string to_string(TypeView value) { return fmt::format("{}", value.get_name()); }

inline std::string to_string(ObjectView value) { return fmt::format("{}", value.get_name()); }

inline std::string to_string(VariableView value) { return fmt::format("{}", value.get_name()); }

inline std::string to_string(ParameterView value)
{
    return fmt::format("{}{}", to_string(value.get_variable()), value.get_types().empty() ? "" : detail::type_annotation(value.get_types()));
}

inline std::string to_string(PredicateView value) { return fmt::format("({}{})", value.get_name(), detail::spaced(value.get_parameters())); }

inline std::string to_string(FunctionSkeletonView value)
{
    return fmt::format("({}{}) - {}", value.get_name(), detail::spaced(value.get_parameters()), to_string(value.get_type()));
}

inline std::string to_string(TermView value)
{
    auto result = std::string {};
    visit([&](const auto& node) { result = to_string(node); }, value.get_value());
    return result;
}

inline std::string to_string(AtomView value) { return fmt::format("({}{})", value.get_predicate().get_name(), detail::spaced(value.get_terms())); }

inline std::string to_string(LiteralView value)
{
    if (value.get_polarity())
        return to_string(value.get_atom());
    return fmt::format("(not {})", to_string(value.get_atom()));
}

inline std::string to_string(FunctionExpressionNumberView value) { return fmt::format("{}", value.get_value()); }

inline std::string to_string(FunctionTermView value) { return fmt::format("({}{})", value.get_function().get_name(), detail::spaced(value.get_terms())); }

inline std::string to_string(UnaryFunctionExpressionView value) { return fmt::format("(- {})", to_string(value.get_expression())); }

inline std::string to_string(BinaryFunctionExpressionView value)
{
    return fmt::format("({} {} {})", loki::formalism::to_string(value.get_data().op), to_string(value.get_left()), to_string(value.get_right()));
}

inline std::string to_string(MultiFunctionExpressionView value)
{
    return fmt::format("({}{})", loki::formalism::to_string(value.get_data().op), detail::spaced(value.get_expressions()));
}

inline std::string to_string(FunctionExpressionView value)
{
    auto result = std::string {};
    visit([&](const auto& node) { result = to_string(node); }, value.get_value());
    return result;
}

inline std::string to_string(ConditionLiteralView value) { return to_string(value.get_literal()); }

inline std::string to_string(ConditionAndView value) { return fmt::format("(and{})", detail::spaced(value.get_conditions())); }

inline std::string to_string(ConditionOrView value) { return fmt::format("(or{})", detail::spaced(value.get_conditions())); }

inline std::string to_string(ConditionNotView value) { return fmt::format("(not {})", to_string(value.get_condition())); }

inline std::string to_string(ConditionImplyView value) { return fmt::format("(imply {} {})", to_string(value.get_left()), to_string(value.get_right())); }

namespace detail
{

// Materializes element strings so fmt::join never needs the guarded view formatters.
template<typename Range>
inline std::vector<std::string> to_strings(const Range& range)
{
    auto result = std::vector<std::string> {};
    for (auto element : range)
        result.push_back(to_string(element));
    return result;
}

}  // namespace detail

inline std::string to_string(ConditionExistsView value)
{
    return fmt::format("(exists ({}) {})", fmt::join(detail::to_strings(value.get_parameters()), " "), to_string(value.get_condition()));
}

inline std::string to_string(ConditionForallView value)
{
    return fmt::format("(forall ({}) {})", fmt::join(detail::to_strings(value.get_parameters()), " "), to_string(value.get_condition()));
}

inline std::string to_string(ConditionNumericConstraintView value)
{
    return fmt::format("({} {} {})", loki::formalism::to_string(value.get_data().comparator), to_string(value.get_left()), to_string(value.get_right()));
}

inline std::string to_string(ConditionView value)
{
    auto result = std::string {};
    visit([&](const auto& node) { result = to_string(node); }, value.get_value());
    return result;
}

inline std::string to_string(EffectLiteralView value) { return to_string(value.get_literal()); }

inline std::string to_string(EffectAndView value) { return fmt::format("(and{})", detail::spaced(value.get_effects())); }

inline std::string to_string(EffectNumericView value)
{
    return fmt::format("({} ({}{}) {})",
                       loki::formalism::to_string(value.get_data().op),
                       value.get_function().get_name(),
                       detail::spaced(value.get_terms()),
                       to_string(value.get_expression()));
}

inline std::string to_string(EffectForallView value)
{
    return fmt::format("(forall ({}) {})", fmt::join(detail::to_strings(value.get_parameters()), " "), to_string(value.get_effect()));
}

inline std::string to_string(EffectWhenView value) { return fmt::format("(when {} {})", to_string(value.get_condition()), to_string(value.get_effect())); }

inline std::string to_string(EffectOneOfView value) { return fmt::format("(oneof{})", detail::spaced(value.get_effects())); }

inline std::string to_string(EffectProbabilisticAlternativeView value)
{
    return fmt::format("{} {}", value.get_data().probability, to_string(value.get_effect()));
}

inline std::string to_string(EffectProbabilisticView value) { return fmt::format("(probabilistic{})", detail::spaced(value.get_alternatives())); }

inline std::string to_string(EffectView value)
{
    auto result = std::string {};
    visit([&](const auto& node) { result = to_string(node); }, value.get_value());
    return result;
}

inline std::string to_string(ActionView value)
{
    return fmt::format("(:action {} :parameters ({}){}{})",
                       value.get_name(),
                       fmt::join(detail::to_strings(value.get_parameters()), " "),
                       value.get_precondition() ? fmt::format(" :precondition {}", to_string(value.get_precondition().value())) : "",
                       value.get_effect() ? fmt::format(" :effect {}", to_string(value.get_effect().value())) : "");
}

inline std::string to_string(AxiomView value)
{
    auto head_variables = ygg::UnorderedSet<VariableView> {};
    for (auto term : value.get_head().get_atom().get_terms())
    {
        visit(
            [&](const auto& node)
            {
                using T = std::remove_cvref_t<decltype(node)>;
                if constexpr (std::same_as<T, VariableView>)
                    head_variables.insert(node);
            },
            term.get_value());
    }

    auto existential_parameters = std::vector<ParameterView> {};
    for (auto parameter : value.get_parameters())
    {
        if (!head_variables.contains(parameter.get_variable()))
            existential_parameters.push_back(parameter);
    }

    auto result = fmt::format("(:derived {} ", to_string(value.get_head()));
    if (!existential_parameters.empty())
        return result + fmt::format("(exists ({}) {}))", fmt::join(detail::to_strings(existential_parameters), " "), to_string(value.get_condition()));
    return result + fmt::format("{})", to_string(value.get_condition()));
}

inline std::string to_string(MetricView value)
{
    return fmt::format("(:metric {} {})", value.get_data().minimize ? "minimize" : "maximize", to_string(value.get_expression()));
}

inline std::string to_string(InitialFunctionValueView value) { return fmt::format("(= {} {})", to_string(value.get_function()), to_string(value.get_value())); }

inline std::string to_string(DomainView value)
{
    auto os = std::stringstream {};
    os << fmt::format("(define (domain {})", value.get_name());
    {
        ygg::IndentScope section(os);
        if (!value.get_requirements().empty())
        {
            os << '\n' << ygg::print_indent << "(:requirements";
            for (auto requirement : value.get_requirements())
                os << ' ' << to_string(requirement);
            os << ')';
        }
        if (!value.get_types().empty())
        {
            auto wrote_header = false;
            for (auto type : value.get_types())
            {
                if (detail::is_builtin_type(type))
                    continue;
                if (!wrote_header)
                {
                    os << '\n' << ygg::print_indent << "(:types";
                    wrote_header = true;
                }
                os << ' ' << to_string(type);
                if (!type.get_bases().empty())
                    os << detail::type_annotation(type.get_bases());
            }
            if (wrote_header)
                os << ')';
        }
        if (!value.get_constants().empty())
        {
            os << '\n' << ygg::print_indent << "(:constants ";
            auto first = true;
            for (auto object : value.get_constants())
            {
                if (!first)
                    os << ' ';
                first = false;
                os << to_string(object);
                if (!object.get_types().empty())
                    os << detail::type_annotation(object.get_types());
            }
            os << ')';
        }
        if (!value.get_predicates().empty())
        {
            os << '\n' << ygg::print_indent << "(:predicates";
            {
                ygg::IndentScope predicates(os);
                for (auto predicate : value.get_predicates())
                    os << '\n' << ygg::print_indent << to_string(predicate);
            }
            os << ')';
        }
        if (!value.get_functions().empty())
        {
            os << '\n' << ygg::print_indent << "(:functions";
            {
                ygg::IndentScope functions(os);
                for (auto function : value.get_functions())
                    os << '\n' << ygg::print_indent << to_string(function);
            }
            os << ')';
        }
        for (auto axiom : value.get_axioms())
            os << '\n' << ygg::print_indent << to_string(axiom);
        for (auto action : value.get_actions())
            os << '\n' << ygg::print_indent << to_string(action);
    }
    os << "\n)";
    return os.str();
}

inline std::string to_string(TaskView value)
{
    auto os = std::stringstream {};
    os << fmt::format("(define (problem {})", value.get_name());
    {
        ygg::IndentScope section(os);
        os << '\n' << ygg::print_indent << fmt::format("(:domain {})", value.get_domain().get_name());
        if (!value.get_requirements().empty())
        {
            os << '\n' << ygg::print_indent << "(:requirements";
            for (auto requirement : value.get_requirements())
                os << ' ' << to_string(requirement);
            os << ')';
        }
        if (!value.get_objects().empty())
        {
            os << '\n' << ygg::print_indent << "(:objects ";
            auto first = true;
            for (auto object : value.get_objects())
            {
                if (!first)
                    os << ' ';
                first = false;
                os << to_string(object);
                if (!object.get_types().empty())
                    os << detail::type_annotation(object.get_types());
            }
            os << ')';
        }
        os << '\n' << ygg::print_indent << "(:init";
        for (auto literal : value.get_initial_literals())
            os << ' ' << to_string(literal);
        for (auto initial_value : value.get_initial_function_values())
            os << ' ' << to_string(initial_value);
        os << ')';
        if (value.get_goal())
            os << '\n' << ygg::print_indent << "(:goal " << to_string(value.get_goal().value()) << ')';
        if (value.get_metric())
            os << '\n' << ygg::print_indent << to_string(value.get_metric().value());
        for (auto axiom : value.get_axioms())
            os << '\n' << ygg::print_indent << to_string(axiom);
    }
    os << "\n)";
    return os.str();
}

}  // namespace loki::formalism::format

namespace fmt
{

#define LOKI_DETAIL_FMT_DELEGATE(ViewType)                                                     \
    template<>                                                                                 \
    struct formatter<loki::formalism::ViewType, char>                                          \
    {                                                                                          \
        constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }                \
        template<typename FormatContext>                                                       \
        auto format(const loki::formalism::ViewType& value, FormatContext& ctx) const          \
        {                                                                                      \
            return fmt::format_to(ctx.out(), "{}", loki::formalism::format::to_string(value)); \
        }                                                                                      \
    };

LOKI_DETAIL_FMT_DELEGATE(RequirementView)
LOKI_DETAIL_FMT_DELEGATE(TypeView)
LOKI_DETAIL_FMT_DELEGATE(ObjectView)
LOKI_DETAIL_FMT_DELEGATE(VariableView)
LOKI_DETAIL_FMT_DELEGATE(ParameterView)
LOKI_DETAIL_FMT_DELEGATE(PredicateView)
LOKI_DETAIL_FMT_DELEGATE(FunctionSkeletonView)
LOKI_DETAIL_FMT_DELEGATE(TermView)
LOKI_DETAIL_FMT_DELEGATE(AtomView)
LOKI_DETAIL_FMT_DELEGATE(LiteralView)
LOKI_DETAIL_FMT_DELEGATE(FunctionExpressionNumberView)
LOKI_DETAIL_FMT_DELEGATE(FunctionTermView)
LOKI_DETAIL_FMT_DELEGATE(UnaryFunctionExpressionView)
LOKI_DETAIL_FMT_DELEGATE(BinaryFunctionExpressionView)
LOKI_DETAIL_FMT_DELEGATE(MultiFunctionExpressionView)
LOKI_DETAIL_FMT_DELEGATE(FunctionExpressionView)
LOKI_DETAIL_FMT_DELEGATE(ConditionLiteralView)
LOKI_DETAIL_FMT_DELEGATE(ConditionAndView)
LOKI_DETAIL_FMT_DELEGATE(ConditionOrView)
LOKI_DETAIL_FMT_DELEGATE(ConditionNotView)
LOKI_DETAIL_FMT_DELEGATE(ConditionImplyView)
LOKI_DETAIL_FMT_DELEGATE(ConditionExistsView)
LOKI_DETAIL_FMT_DELEGATE(ConditionForallView)
LOKI_DETAIL_FMT_DELEGATE(ConditionNumericConstraintView)
LOKI_DETAIL_FMT_DELEGATE(ConditionView)
LOKI_DETAIL_FMT_DELEGATE(EffectLiteralView)
LOKI_DETAIL_FMT_DELEGATE(EffectAndView)
LOKI_DETAIL_FMT_DELEGATE(EffectNumericView)
LOKI_DETAIL_FMT_DELEGATE(EffectForallView)
LOKI_DETAIL_FMT_DELEGATE(EffectWhenView)
LOKI_DETAIL_FMT_DELEGATE(EffectOneOfView)
LOKI_DETAIL_FMT_DELEGATE(EffectProbabilisticAlternativeView)
LOKI_DETAIL_FMT_DELEGATE(EffectProbabilisticView)
LOKI_DETAIL_FMT_DELEGATE(EffectView)
LOKI_DETAIL_FMT_DELEGATE(ActionView)
LOKI_DETAIL_FMT_DELEGATE(AxiomView)
LOKI_DETAIL_FMT_DELEGATE(MetricView)
LOKI_DETAIL_FMT_DELEGATE(InitialFunctionValueView)
LOKI_DETAIL_FMT_DELEGATE(DomainView)
LOKI_DETAIL_FMT_DELEGATE(TaskView)

#undef LOKI_DETAIL_FMT_DELEGATE

}  // namespace fmt

#endif
