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

#include "loki/config.hpp"
#include "loki/formalism/repository.hpp"
#include "loki/formalism/views.hpp"

#include <cmath>
#include <fmt/core.h>
#include <fmt/ostream.h>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/io/iostream.hpp>

namespace loki::formalism::format
{

template<typename T>
std::string to_string(const T& value)
{
    return fmt::format("{}", value);
}

namespace detail
{

inline bool is_builtin_type(TypeView type)
{
    const auto name = std::string_view(type.get_name().data(), type.get_name().size());
    return name == "object" || name == "number";
}

// Renders " - type" or " - (either t1 t2 ...)"; multiple types require an either wrapper to reparse.
template<typename Types>
inline std::string type_annotation(const Types& types)
{
    if (types.size() == 1)
        return fmt::format(" - {}", types[0]);
    auto text = std::string { " - (either" };
    for (auto type : types)
        text += fmt::format(" {}", type);
    return text + ")";
}

}  // namespace detail

}  // namespace loki::formalism::format

#if LOKI_ENABLE_FMT_FORMATTERS
namespace fmt
{

template<>
struct formatter<loki::formalism::RequirementView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::RequirementView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", loki::formalism::to_string(value.get_kind()));
    }
};

template<>
struct formatter<loki::formalism::TypeView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::TypeView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", std::string_view(value.get_name().data(), value.get_name().size()));
    }
};

template<>
struct formatter<loki::formalism::ObjectView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ObjectView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", std::string_view(value.get_name().data(), value.get_name().size()));
    }
};

template<>
struct formatter<loki::formalism::VariableView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::VariableView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", std::string_view(value.get_name().data(), value.get_name().size()));
    }
};

template<>
struct formatter<loki::formalism::ParameterView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ParameterView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "{}", value.get_variable());
        if (!value.get_types().empty())
            out = fmt::format_to(out, "{}", loki::formalism::format::detail::type_annotation(value.get_types()));
        return out;
    }
};

template<>
struct formatter<loki::formalism::PredicateView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::PredicateView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "({}", std::string_view(value.get_name().data(), value.get_name().size()));
        for (auto parameter : value.get_parameters())
            out = fmt::format_to(out, " {}", parameter);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::FunctionSkeletonView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::FunctionSkeletonView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "({}", std::string_view(value.get_name().data(), value.get_name().size()));
        for (auto parameter : value.get_parameters())
            out = fmt::format_to(out, " {}", parameter);
        return fmt::format_to(out, ") - {}", value.get_type());
    }
};

template<>
struct formatter<loki::formalism::TermView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::TermView& value, FormatContext& ctx) const
    {
        auto out = ctx.out();
        visit([&](const auto& node) { out = fmt::format_to(out, "{}", node); }, value.get_value());
        return out;
    }
};

template<>
struct formatter<loki::formalism::AtomView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::AtomView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "({}", std::string_view(value.get_predicate().get_name().data(), value.get_predicate().get_name().size()));
        for (auto term : value.get_terms())
            out = fmt::format_to(out, " {}", term);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::LiteralView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::LiteralView& value, FormatContext& ctx) const
    {
        if (value.get_polarity())
            return fmt::format_to(ctx.out(), "{}", value.get_atom());
        return fmt::format_to(ctx.out(), "(not {})", value.get_atom());
    }
};

template<>
struct formatter<loki::formalism::FunctionExpressionNumberView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::FunctionExpressionNumberView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", value.get_value());
    }
};

template<>
struct formatter<loki::formalism::FunctionTermView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::FunctionTermView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "({}", std::string_view(value.get_function().get_name().data(), value.get_function().get_name().size()));
        for (auto term : value.get_terms())
            out = fmt::format_to(out, " {}", term);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::UnaryFunctionExpressionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::UnaryFunctionExpressionView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(- {})", value.get_expression());
    }
};

template<>
struct formatter<loki::formalism::BinaryFunctionExpressionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::BinaryFunctionExpressionView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({} {} {})", loki::formalism::to_string(value.get_data().op), value.get_left(), value.get_right());
    }
};

template<>
struct formatter<loki::formalism::MultiFunctionExpressionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::MultiFunctionExpressionView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "({}", loki::formalism::to_string(value.get_data().op));
        for (auto expression : value.get_expressions())
            out = fmt::format_to(out, " {}", expression);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::FunctionExpressionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::FunctionExpressionView& value, FormatContext& ctx) const
    {
        auto out = ctx.out();
        visit([&](const auto& node) { out = fmt::format_to(out, "{}", node); }, value.get_value());
        return out;
    }
};

template<>
struct formatter<loki::formalism::ConditionLiteralView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionLiteralView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", value.get_literal());
    }
};

template<>
struct formatter<loki::formalism::ConditionAndView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionAndView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(and");
        for (auto condition : value.get_conditions())
            out = fmt::format_to(out, " {}", condition);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::ConditionOrView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionOrView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(or");
        for (auto condition : value.get_conditions())
            out = fmt::format_to(out, " {}", condition);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::ConditionNotView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionNotView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(not {})", value.get_condition());
    }
};

template<>
struct formatter<loki::formalism::ConditionImplyView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionImplyView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(imply {} {})", value.get_left(), value.get_right());
    }
};

template<>
struct formatter<loki::formalism::ConditionExistsView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionExistsView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(exists (");
        auto first = true;
        for (auto parameter : value.get_parameters())
        {
            if (!first)
                out = fmt::format_to(out, " ");
            first = false;
            out = fmt::format_to(out, "{}", parameter);
        }
        return fmt::format_to(out, ") {})", value.get_condition());
    }
};

template<>
struct formatter<loki::formalism::ConditionForallView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionForallView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(forall (");
        auto first = true;
        for (auto parameter : value.get_parameters())
        {
            if (!first)
                out = fmt::format_to(out, " ");
            first = false;
            out = fmt::format_to(out, "{}", parameter);
        }
        return fmt::format_to(out, ") {})", value.get_condition());
    }
};

template<>
struct formatter<loki::formalism::ConditionNumericConstraintView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionNumericConstraintView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "({} {} {})", loki::formalism::to_string(value.get_data().comparator), value.get_left(), value.get_right());
    }
};

template<>
struct formatter<loki::formalism::ConditionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ConditionView& value, FormatContext& ctx) const
    {
        auto out = ctx.out();
        visit([&](const auto& node) { out = fmt::format_to(out, "{}", node); }, value.get_value());
        return out;
    }
};

template<>
struct formatter<loki::formalism::EffectLiteralView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectLiteralView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{}", value.get_literal());
    }
};

template<>
struct formatter<loki::formalism::EffectAndView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectAndView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(and");
        for (auto effect : value.get_effects())
            out = fmt::format_to(out, " {}", effect);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::EffectNumericView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectNumericView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(),
                                  "({} ({}",
                                  loki::formalism::to_string(value.get_data().op),
                                  std::string_view(value.get_function().get_name().data(), value.get_function().get_name().size()));
        for (auto term : value.get_terms())
            out = fmt::format_to(out, " {}", term);
        return fmt::format_to(out, ") {})", value.get_expression());
    }
};

template<>
struct formatter<loki::formalism::EffectForallView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectForallView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(forall (");
        auto first = true;
        for (auto parameter : value.get_parameters())
        {
            if (!first)
                out = fmt::format_to(out, " ");
            first = false;
            out = fmt::format_to(out, "{}", parameter);
        }
        return fmt::format_to(out, ") {})", value.get_effect());
    }
};

template<>
struct formatter<loki::formalism::EffectWhenView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectWhenView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(when {} {})", value.get_condition(), value.get_effect());
    }
};

template<>
struct formatter<loki::formalism::EffectOneOfView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectOneOfView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(oneof");
        for (auto effect : value.get_effects())
            out = fmt::format_to(out, " {}", effect);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::EffectProbabilisticAlternativeView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectProbabilisticAlternativeView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "{} {}", value.get_data().probability, value.get_effect());
    }
};

template<>
struct formatter<loki::formalism::EffectProbabilisticView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectProbabilisticView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(probabilistic");
        for (auto alternative : value.get_alternatives())
            out = fmt::format_to(out, " {}", alternative);
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::EffectView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::EffectView& value, FormatContext& ctx) const
    {
        auto out = ctx.out();
        visit([&](const auto& node) { out = fmt::format_to(out, "{}", node); }, value.get_value());
        return out;
    }
};

template<>
struct formatter<loki::formalism::ActionView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::ActionView& value, FormatContext& ctx) const
    {
        auto out = fmt::format_to(ctx.out(), "(:action {} :parameters (", std::string_view(value.get_name().data(), value.get_name().size()));
        auto first = true;
        for (auto parameter : value.get_parameters())
        {
            if (!first)
                out = fmt::format_to(out, " ");
            first = false;
            out = fmt::format_to(out, "{}", parameter);
        }
        out = fmt::format_to(out, ")");
        if (value.get_precondition())
            out = fmt::format_to(out, " :precondition {}", value.get_precondition().value());
        if (value.get_effect())
            out = fmt::format_to(out, " :effect {}", value.get_effect().value());
        return fmt::format_to(out, ")");
    }
};

template<>
struct formatter<loki::formalism::AxiomView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::AxiomView& value, FormatContext& ctx) const
    {
        auto head_variables = ygg::UnorderedSet<loki::formalism::VariableView> {};
        for (auto term : value.get_head().get_atom().get_terms())
        {
            visit(
                [&](const auto& node)
                {
                    using T = std::remove_cvref_t<decltype(node)>;
                    if constexpr (std::same_as<T, loki::formalism::VariableView>)
                        head_variables.insert(node);
                },
                term.get_value());
        }

        auto existential_parameters = std::vector<loki::formalism::ParameterView> {};
        for (auto parameter : value.get_parameters())
        {
            if (!head_variables.contains(parameter.get_variable()))
                existential_parameters.push_back(parameter);
        }

        auto out = fmt::format_to(ctx.out(), "(:derived {} ", value.get_head());
        if (!existential_parameters.empty())
        {
            out = fmt::format_to(out, "(exists (");
            auto first = true;
            for (auto parameter : existential_parameters)
            {
                if (!first)
                    out = fmt::format_to(out, " ");
                first = false;
                out = fmt::format_to(out, "{}", parameter);
            }
            return fmt::format_to(out, ") {}))", value.get_condition());
        }
        return fmt::format_to(out, "{})", value.get_condition());
    }
};

template<>
struct formatter<loki::formalism::MetricView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::MetricView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(:metric {} {})", value.get_data().minimize ? "minimize" : "maximize", value.get_expression());
    }
};

template<>
struct formatter<loki::formalism::InitialFunctionValueView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::InitialFunctionValueView& value, FormatContext& ctx) const
    {
        return fmt::format_to(ctx.out(), "(= {} {})", value.get_function(), value.get_value());
    }
};

template<>
struct formatter<loki::formalism::DomainView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::DomainView& value, FormatContext& ctx) const
    {
        auto os = std::stringstream {};
        os << "(define (domain " << std::string_view(value.get_name().data(), value.get_name().size()) << ')';
        {
            ygg::IndentScope section(os);
            if (!value.get_requirements().empty())
            {
                os << '\n' << ygg::print_indent << "(:requirements";
                for (auto requirement : value.get_requirements())
                    fmt::print(os, " {}", requirement);
                os << ')';
            }
            if (!value.get_types().empty())
            {
                auto wrote_header = false;
                for (auto type : value.get_types())
                {
                    if (loki::formalism::format::detail::is_builtin_type(type))
                        continue;
                    if (!wrote_header)
                    {
                        os << '\n' << ygg::print_indent << "(:types";
                        wrote_header = true;
                    }
                    fmt::print(os, " {}", type);
                    if (!type.get_bases().empty())
                        os << loki::formalism::format::detail::type_annotation(type.get_bases());
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
                    fmt::print(os, "{}", object);
                    if (!object.get_types().empty())
                        os << loki::formalism::format::detail::type_annotation(object.get_types());
                }
                os << ')';
            }
            if (!value.get_predicates().empty())
            {
                os << '\n' << ygg::print_indent << "(:predicates";
                {
                    ygg::IndentScope predicates(os);
                    for (auto predicate : value.get_predicates())
                    {
                        os << '\n' << ygg::print_indent;
                        fmt::print(os, "{}", predicate);
                    }
                }
                os << ')';
            }
            if (!value.get_functions().empty())
            {
                os << '\n' << ygg::print_indent << "(:functions";
                {
                    ygg::IndentScope functions(os);
                    for (auto function : value.get_functions())
                    {
                        os << '\n' << ygg::print_indent;
                        fmt::print(os, "{}", function);
                    }
                }
                os << ')';
            }
            for (auto axiom : value.get_axioms())
            {
                os << '\n' << ygg::print_indent;
                fmt::print(os, "{}", axiom);
            }
            for (auto action : value.get_actions())
            {
                os << '\n' << ygg::print_indent;
                fmt::print(os, "{}", action);
            }
        }
        os << "\n)";
        return fmt::format_to(ctx.out(), "{}", os.str());
    }
};

template<>
struct formatter<loki::formalism::TaskView, char>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }
    template<typename FormatContext>
    auto format(const loki::formalism::TaskView& value, FormatContext& ctx) const
    {
        auto os = std::stringstream {};
        os << "(define (problem " << std::string_view(value.get_name().data(), value.get_name().size()) << ')';
        {
            ygg::IndentScope section(os);
            os << '\n'
               << ygg::print_indent << "(:domain " << std::string_view(value.get_domain().get_name().data(), value.get_domain().get_name().size()) << ')';
            if (!value.get_requirements().empty())
            {
                os << '\n' << ygg::print_indent << "(:requirements";
                for (auto requirement : value.get_requirements())
                    fmt::print(os, " {}", requirement);
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
                    fmt::print(os, "{}", object);
                    if (!object.get_types().empty())
                        os << loki::formalism::format::detail::type_annotation(object.get_types());
                }
                os << ')';
            }
            os << '\n' << ygg::print_indent << "(:init";
            for (auto literal : value.get_initial_literals())
                fmt::print(os, " {}", literal);
            for (auto initial_value : value.get_initial_function_values())
                fmt::print(os, " {}", initial_value);
            os << ')';
            if (value.get_goal())
            {
                os << '\n' << ygg::print_indent;
                fmt::print(os, "(:goal {})", value.get_goal().value());
            }
            if (value.get_metric())
            {
                os << '\n' << ygg::print_indent;
                fmt::print(os, "{}", value.get_metric().value());
            }
            for (auto axiom : value.get_axioms())
            {
                os << '\n' << ygg::print_indent;
                fmt::print(os, "{}", axiom);
            }
        }
        os << "\n)";
        return fmt::format_to(ctx.out(), "{}", os.str());
    }
};

}  // namespace fmt
#endif

#endif
