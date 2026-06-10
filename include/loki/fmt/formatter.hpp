/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_FMT_FORMATTER_HPP_
#define LOKI_FMT_FORMATTER_HPP_

#include "loki/ast/ast.hpp"
#include "loki/config.hpp"

#include <fmt/format.h>

#include <boost/variant/apply_visitor.hpp>

#include <cmath>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

namespace loki::format
{

struct Options
{
    int indent = 0;
    int indent_step = 2;
    bool multiline_sections = true;
};

namespace detail
{

template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

inline void spaces(std::ostream& out, int n)
{
    for (int i = 0; i < n; ++i)
        out.put(' ');
}

inline Options nested(Options options)
{
    options.indent += options.indent_step;
    return options;
}

inline bool is_atom_char(char c) noexcept
{
    return c != '(' && c != ')' && c != ';' && c != ' ' && c != '\t' && c != '\r' && c != '\n';
}

inline void write_atom_text(std::string_view text, std::ostream& out)
{
    auto wrote = false;
    for (const auto c : text)
    {
        if (is_atom_char(c))
        {
            out << c;
            wrote = true;
        }
        else
            out << '_';
    }
    if (!wrote)
        out << '_';
}

inline void write_identifier(const ast::Identifier& id, std::ostream& out)
{
    write_atom_text(id.text, out);
}

inline void write_variable_identifier(const ast::Identifier& id, std::ostream& out)
{
    if (id.text.empty())
    {
        out << "?_";
        return;
    }
    if (id.text.front() != '?')
        out << '?';
    write_atom_text(id.text, out);
}

inline void write_keyword(std::string_view keyword, std::ostream& out)
{
    out << ':';
    if (!keyword.empty() && keyword.front() == ':')
        keyword.remove_prefix(1);
    write_atom_text(keyword, out);
}

inline void write_number(double value, std::ostream& out)
{
    if (!std::isfinite(value))
    {
        out << '0';
        return;
    }
    const auto precision = out.precision();
    out.precision(std::numeric_limits<double>::max_digits10);
    out << value;
    out.precision(precision);
}

inline bool ascii_ieq(std::string_view lhs, std::string_view rhs) noexcept
{
    if (lhs.size() != rhs.size())
        return false;
    for (std::size_t i = 0; i < lhs.size(); ++i)
    {
        auto l = lhs[i];
        auto r = rhs[i];
        if (l >= 'A' && l <= 'Z')
            l = static_cast<char>(l - 'A' + 'a');
        if (r >= 'A' && r <= 'Z')
            r = static_cast<char>(r - 'A' + 'a');
        if (l != r)
            return false;
    }
    return true;
}

inline std::string_view unary_arithmetic_operator(std::string_view op) noexcept
{
    return ascii_ieq(op, "minus") ? "minus" : "-";
}

inline std::string_view binary_arithmetic_operator(std::string_view op) noexcept
{
    if (op == "+" || op == "-" || op == "*" || op == "/")
        return op;
    return "-";
}

inline std::string_view multi_arithmetic_operator(std::string_view op) noexcept
{
    return op == "*" ? "*" : "+";
}

inline std::string_view comparator(std::string_view op) noexcept
{
    if (op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=")
        return op;
    return "=";
}

inline std::string_view numeric_effect_operator(std::string_view op) noexcept
{
    if (ascii_ieq(op, "increase"))
        return "increase";
    if (ascii_ieq(op, "decrease"))
        return "decrease";
    if (ascii_ieq(op, "scale-up"))
        return "scale-up";
    if (ascii_ieq(op, "scale-down"))
        return "scale-down";
    return "assign";
}

inline std::string_view metric_optimization(std::string_view optimization) noexcept
{
    return ascii_ieq(optimization, "maximize") ? "maximize" : "minimize";
}

} // namespace detail

inline void write(const ast::TypeExpression& type, std::ostream& out, Options options = {});
inline void write(const ast::Term& term, std::ostream& out, Options options = {});
inline void write(const ast::Atom& atom, std::ostream& out, Options options = {});
inline void write(const ast::Literal& literal, std::ostream& out, Options options = {});
inline void write(const ast::FunctionTerm& term, std::ostream& out, Options options = {});
inline void write(const ast::FunctionExpression& expression, std::ostream& out, Options options = {});
inline void write(const ast::Condition& condition, std::ostream& out, Options options = {});
inline void write(const ast::Effect& effect, std::ostream& out, Options options = {});
inline void write(const ast::InitialFunctionValue& value, std::ostream& out, Options options = {});
inline void write(const ast::InitialElement& element, std::ostream& out, Options options = {});
inline void write(const ast::Domain& domain, std::ostream& out, Options options = {});
inline void write(const ast::Task& task, std::ostream& out, Options options = {});

inline void write(const ast::TypeExpression& type, std::ostream& out, Options options)
{
    boost::apply_visitor(detail::Overloaded {
                   [&](const ast::TypeReference& node) { detail::write_identifier(node.name, out); },
                   [&](const ast::EitherType& node)
                   {
                       out << "(either";
                       for (const auto& alternative : node.alternatives)
                       {
                           out << ' ';
                           write(alternative.get(), out, options);
                       }
                       out << ')';
                   },
               },
               type);
}

inline void write_typed_names(const std::vector<ast::TypedName>& names, std::ostream& out, Options options = {})
{
    bool first = true;
    for (const auto& name : names)
    {
        if (!first)
            out << ' ';
        first = false;
        detail::write_identifier(name.name, out);
        if (name.type)
        {
            out << " - ";
            write(*name.type, out, options);
        }
    }
}

inline void write_typed_variables(const std::vector<ast::TypedVariable>& variables, std::ostream& out, Options options = {})
{
    bool first = true;
    for (const auto& variable : variables)
    {
        if (!first)
            out << ' ';
        first = false;
        detail::write_variable_identifier(variable.variable, out);
        if (variable.type)
        {
            out << " - ";
            write(*variable.type, out, options);
        }
    }
}

inline void write(const ast::Term& term, std::ostream& out, Options)
{
    if (term.variable)
        detail::write_variable_identifier(term.name, out);
    else
        detail::write_identifier(term.name, out);
}

inline void write(const ast::Atom& atom, std::ostream& out, Options options)
{
    out << '(';
    detail::write_identifier(atom.predicate, out);
    for (const auto& term : atom.terms)
    {
        out << ' ';
        write(term, out, options);
    }
    out << ')';
}

inline void write(const ast::Literal& literal, std::ostream& out, Options options)
{
    if (literal.positive)
        write(literal.atom, out, options);
    else
    {
        out << "(not ";
        write(literal.atom, out, options);
        out << ')';
    }
}

inline void write(const ast::FunctionTerm& term, std::ostream& out, Options options)
{
    out << '(';
    detail::write_identifier(term.function, out);
    for (const auto& arg : term.terms)
    {
        out << ' ';
        write(arg, out, options);
    }
    out << ')';
}

inline void write(const ast::FunctionExpression& expression, std::ostream& out, Options options)
{
    boost::apply_visitor(detail::Overloaded {
                   [&](const ast::FunctionExpressionNumber& node) { detail::write_number(node.value, out); },
                   [&](const ast::FunctionExpressionFunction& node) { write(node.term, out, options); },
                   [&](const ast::FunctionExpressionUnary& node)
                   {
                       out << '(' << detail::unary_arithmetic_operator(node.op) << ' ';
                       write(node.expression.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::FunctionExpressionBinary& node)
                   {
                       out << '(' << detail::binary_arithmetic_operator(node.op) << ' ';
                       write(node.left.get(), out, options);
                       out << ' ';
                       write(node.right.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::FunctionExpressionMulti& node)
                   {
                       out << '(' << detail::multi_arithmetic_operator(node.op);
                       for (const auto& child : node.expressions)
                       {
                           out << ' ';
                           write(child.get(), out, options);
                       }
                       out << ')';
                   },
               },
               expression);
}

inline void write(const ast::Condition& condition, std::ostream& out, Options options)
{
    boost::apply_visitor(detail::Overloaded {
                   [&](const ast::ConditionLiteral& node) { write(node.literal, out, options); },
                   [&](const ast::ConditionAnd& node)
                   {
                       out << "(and";
                       for (const auto& child : node.conditions)
                       {
                           out << ' ';
                           write(child.get(), out, options);
                       }
                       out << ')';
                   },
                   [&](const ast::ConditionOr& node)
                   {
                       out << "(or";
                       for (const auto& child : node.conditions)
                       {
                           out << ' ';
                           write(child.get(), out, options);
                       }
                       out << ')';
                   },
                   [&](const ast::ConditionNot& node)
                   {
                       out << "(not ";
                       write(node.condition.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::ConditionImply& node)
                   {
                       out << "(imply ";
                       write(node.left.get(), out, options);
                       out << ' ';
                       write(node.right.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::ConditionExists& node)
                   {
                       out << "(exists (";
                       write_typed_variables(node.parameters, out, options);
                       out << ") ";
                       write(node.condition.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::ConditionForall& node)
                   {
                       out << "(forall (";
                       write_typed_variables(node.parameters, out, options);
                       out << ") ";
                       write(node.condition.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::ConditionNumericConstraint& node)
                   {
                       out << '(' << detail::comparator(node.comparator) << ' ';
                       write(node.left.get(), out, options);
                       out << ' ';
                       write(node.right.get(), out, options);
                       out << ')';
                   },
               },
               condition);
}

inline void write(const ast::Effect& effect, std::ostream& out, Options options)
{
    boost::apply_visitor(detail::Overloaded {
                   [&](const ast::EffectLiteral& node) { write(node.literal, out, options); },
                   [&](const ast::EffectAnd& node)
                   {
                       out << "(and";
                       for (const auto& child : node.effects)
                       {
                           out << ' ';
                           write(child.get(), out, options);
                       }
                       out << ')';
                   },
                   [&](const ast::EffectNumeric& node)
                   {
                       out << '(' << detail::numeric_effect_operator(node.op) << ' ';
                       write(node.function, out, options);
                       out << ' ';
                       write(node.expression.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::EffectForall& node)
                   {
                       out << "(forall (";
                       write_typed_variables(node.parameters, out, options);
                       out << ") ";
                       write(node.effect.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::EffectWhen& node)
                   {
                       out << "(when ";
                       write(node.condition.get(), out, options);
                       out << ' ';
                       write(node.effect.get(), out, options);
                       out << ')';
                   },
                   [&](const ast::EffectOneOf& node)
                   {
                       out << "(oneof";
                       for (const auto& child : node.effects)
                       {
                           out << ' ';
                           write(child.get(), out, options);
                       }
                       out << ')';
                   },
                   [&](const ast::EffectProbabilistic& node)
                   {
                       out << "(probabilistic";
                       for (const auto& alternative : node.alternatives)
                       {
                           out << ' ';
                           detail::write_number(alternative.probability, out);
                           out << ' ';
                           write(alternative.effect.get(), out, options);
                       }
                       out << ')';
                   },
               },
               effect);
}

inline void write_predicate(const ast::PredicateDeclaration& predicate, std::ostream& out, Options options)
{
    out << '(';
    detail::write_identifier(predicate.name, out);
    if (!predicate.parameters.empty())
    {
        out << ' ';
        write_typed_variables(predicate.parameters, out, options);
    }
    out << ')';
}

inline void write_function(const ast::FunctionDeclaration& function, std::ostream& out, Options options)
{
    out << '(';
    detail::write_identifier(function.name, out);
    if (!function.parameters.empty())
    {
        out << ' ';
        write_typed_variables(function.parameters, out, options);
    }
    out << ')';
    if (function.type)
    {
        out << " - ";
        write(*function.type, out, options);
    }
}

inline void write_derived_head(const ast::Axiom& axiom, std::ostream& out, Options options)
{
    out << '(';
    detail::write_identifier(axiom.head.name, out);
    if (!axiom.head.parameters.empty())
    {
        out << ' ';
        write_typed_variables(axiom.head.parameters, out, options);
    }
    out << ')';
}

inline void write(const ast::Action& action, std::ostream& out, Options options)
{
    auto nested_options = detail::nested(options);
    detail::spaces(out, options.indent);
    out << "(:action ";
    detail::write_identifier(action.name, out);
    out << '\n';
    detail::spaces(out, nested_options.indent);
    out << ":parameters (";
    write_typed_variables(action.parameters, out, nested_options);
    out << ")\n";
    if (action.precondition)
    {
        detail::spaces(out, nested_options.indent);
        out << ":precondition ";
        write(*action.precondition, out, nested_options);
        out << '\n';
    }
    if (action.effect)
    {
        detail::spaces(out, nested_options.indent);
        out << ":effect ";
        write(*action.effect, out, nested_options);
        out << '\n';
    }
    detail::spaces(out, options.indent);
    out << ')';
}

inline void write(const ast::Axiom& axiom, std::ostream& out, Options options)
{
    detail::spaces(out, options.indent);
    out << "(:derived ";
    write_derived_head(axiom, out, options);
    out << ' ';
    write(axiom.condition, out, options);
    out << ')';
}

inline void write(const ast::Metric& metric, std::ostream& out, Options options)
{
    out << "(:metric " << detail::metric_optimization(metric.optimization.text) << ' ';
    write(metric.expression, out, options);
    out << ')';
}

inline void write(const ast::InitialFunctionValue& value, std::ostream& out, Options options)
{
    out << "(= ";
    write(value.function, out, options);
    out << ' ';
    write(value.value, out, options);
    out << ')';
}

inline void write(const ast::InitialElement& element, std::ostream& out, Options options)
{
    boost::apply_visitor(detail::Overloaded {
                             [&](const ast::Literal& literal) { write(literal, out, options); },
                             [&](const ast::InitialFunctionValue& value) { write(value, out, options); },
                         },
                         element);
}

inline void write_section_line(std::string_view header, std::ostream& out, Options options)
{
    detail::spaces(out, options.indent);
    out << header;
}

inline void write(const ast::Domain& domain, std::ostream& out, Options options)
{
    out << "(define (domain ";
    detail::write_identifier(domain.name, out);
    out << ")\n";
    auto section = detail::nested(options);

    if (!domain.requirements.empty())
    {
        write_section_line("(:requirements", out, section);
        for (const auto& requirement : domain.requirements)
        {
            out << ' ';
            detail::write_keyword(requirement.name.text, out);
        }
        out << ")\n";
    }
    if (!domain.types.empty())
    {
        write_section_line("(:types ", out, section);
        write_typed_names(domain.types, out, section);
        out << ")\n";
    }
    if (!domain.constants.empty())
    {
        write_section_line("(:constants ", out, section);
        write_typed_names(domain.constants, out, section);
        out << ")\n";
    }
    if (!domain.predicates.empty())
    {
        write_section_line("(:predicates", out, section);
        for (const auto& predicate : domain.predicates)
        {
            out << '\n';
            detail::spaces(out, section.indent + section.indent_step);
            write_predicate(predicate, out, section);
        }
        out << ")\n";
    }
    if (!domain.functions.empty())
    {
        write_section_line("(:functions", out, section);
        for (const auto& function : domain.functions)
        {
            out << '\n';
            detail::spaces(out, section.indent + section.indent_step);
            write_function(function, out, section);
        }
        out << ")\n";
    }
    for (const auto& axiom : domain.axioms)
    {
        write(axiom, out, section);
        out << '\n';
    }
    for (const auto& action : domain.actions)
    {
        write(action, out, section);
        out << '\n';
    }
    out << ")\n";
}

inline void write(const ast::Task& task, std::ostream& out, Options options)
{
    out << "(define (problem ";
    detail::write_identifier(task.name, out);
    out << ")\n";
    auto section = detail::nested(options);
    write_section_line("(:domain ", out, section);
    detail::write_identifier(task.domain_name, out);
    out << ")\n";

    if (!task.requirements.empty())
    {
        write_section_line("(:requirements", out, section);
        for (const auto& requirement : task.requirements)
        {
            out << ' ';
            detail::write_keyword(requirement.name.text, out);
        }
        out << ")\n";
    }
    if (!task.objects.empty())
    {
        write_section_line("(:objects ", out, section);
        write_typed_names(task.objects, out, section);
        out << ")\n";
    }
    write_section_line("(:init", out, section);
    for (const auto& element : task.initial)
    {
        out << ' ';
        write(element, out, section);
    }
    out << ")\n";

    if (task.goal)
    {
        write_section_line("(:goal ", out, section);
        write(*task.goal, out, section);
        out << ")\n";
    }
    if (task.metric)
    {
        detail::spaces(out, section.indent);
        write(*task.metric, out, section);
        out << '\n';
    }
    for (const auto& axiom : task.axioms)
    {
        write(axiom, out, section);
        out << '\n';
    }
    out << ")\n";
}

inline void write(const ast::File& file, std::ostream& out, Options options = {})
{
    boost::apply_visitor([&](const auto& node) { write(node, out, options); }, file);
}

namespace detail
{
template<typename T>
std::string to_string(const T& node, Options options)
{
    std::ostringstream out;
    write(node, out, options);
    return out.str();
}
}

inline std::string type_expression(const ast::TypeExpression& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string term(const ast::Term& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string atom(const ast::Atom& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string literal(const ast::Literal& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string function_term(const ast::FunctionTerm& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string function_expression(const ast::FunctionExpression& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string condition(const ast::Condition& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string effect(const ast::Effect& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string initial_function_value(const ast::InitialFunctionValue& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string initial_element(const ast::InitialElement& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string action(const ast::Action& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string axiom(const ast::Axiom& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string metric(const ast::Metric& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string domain(const ast::Domain& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string task(const ast::Task& node, Options options = {}) { return detail::to_string(node, options); }
inline std::string file(const ast::File& node, Options options = {}) { return detail::to_string(node, options); }

} // namespace loki::format

#if LOKI_ENABLE_FMT_FORMATTERS
#define LOKI_DEFINE_AST_FORMATTER(Type, Function) \
    template<> \
    struct fmt::formatter<Type, char> : fmt::formatter<std::string_view> \
    { \
        template<typename FormatContext> \
        auto format(const Type& value, FormatContext& ctx) const \
        { \
            const auto text = Function(value); \
            return fmt::formatter<std::string_view>::format(text, ctx); \
        } \
    };

LOKI_DEFINE_AST_FORMATTER(loki::ast::TypeExpression, loki::format::type_expression)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Term, loki::format::term)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Atom, loki::format::atom)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Literal, loki::format::literal)
LOKI_DEFINE_AST_FORMATTER(loki::ast::FunctionTerm, loki::format::function_term)
LOKI_DEFINE_AST_FORMATTER(loki::ast::FunctionExpression, loki::format::function_expression)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Condition, loki::format::condition)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Effect, loki::format::effect)
LOKI_DEFINE_AST_FORMATTER(loki::ast::InitialFunctionValue, loki::format::initial_function_value)
LOKI_DEFINE_AST_FORMATTER(loki::ast::InitialElement, loki::format::initial_element)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Action, loki::format::action)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Axiom, loki::format::axiom)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Metric, loki::format::metric)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Domain, loki::format::domain)
LOKI_DEFINE_AST_FORMATTER(loki::ast::Task, loki::format::task)
LOKI_DEFINE_AST_FORMATTER(loki::ast::File, loki::format::file)

#undef LOKI_DEFINE_AST_FORMATTER
#endif

#endif
