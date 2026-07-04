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

#include <cista/containers/string.h>
#include <cmath>
#include <limits>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

namespace loki::formalism::format
{

struct Options
{
    int indent = 0;
    int indent_step = 2;
};

namespace detail
{

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

inline std::string text(const cista::offset::string& value) { return std::string(value); }

inline void atom_text(const cista::offset::string& value, std::ostream& out) { out << text(value); }
inline void variable_text(const cista::offset::string& value, std::ostream& out) { out << '?' << text(value); }

inline const char* requirement(RequirementKind kind) noexcept
{
    switch (kind)
    {
        case RequirementKind::Strips:
            return ":strips";
        case RequirementKind::Typing:
            return ":typing";
        case RequirementKind::NegativePreconditions:
            return ":negative-preconditions";
        case RequirementKind::DisjunctivePreconditions:
            return ":disjunctive-preconditions";
        case RequirementKind::Equality:
            return ":equality";
        case RequirementKind::ExistentialPreconditions:
            return ":existential-preconditions";
        case RequirementKind::UniversalPreconditions:
            return ":universal-preconditions";
        case RequirementKind::QuantifiedPreconditions:
            return ":quantified-preconditions";
        case RequirementKind::ConditionalEffects:
            return ":conditional-effects";
        case RequirementKind::Fluents:
            return ":fluents";
        case RequirementKind::NumericFluents:
            return ":numeric-fluents";
        case RequirementKind::DurativeActions:
            return ":durative-actions";
        case RequirementKind::DerivedPredicates:
            return ":derived-predicates";
        case RequirementKind::NonDeterministic:
            return ":non-deterministic";
        case RequirementKind::ProbabilisticEffects:
            return ":probabilistic-effects";
    }
    return ":strips";
}

inline const char* comparator(BinaryComparator op) noexcept
{
    switch (op)
    {
        case BinaryComparator::Equal:
            return "=";
        case BinaryComparator::NotEqual:
            return "!=";
        case BinaryComparator::Less:
            return "<";
        case BinaryComparator::LessEqual:
            return "<=";
        case BinaryComparator::Greater:
            return ">";
        case BinaryComparator::GreaterEqual:
            return ">=";
    }
    return "=";
}

inline const char* binary_operator(BinaryArithmeticOperator op) noexcept
{
    switch (op)
    {
        case BinaryArithmeticOperator::Add:
            return "+";
        case BinaryArithmeticOperator::Subtract:
            return "-";
        case BinaryArithmeticOperator::Multiply:
            return "*";
        case BinaryArithmeticOperator::Divide:
            return "/";
    }
    return "+";
}

inline const char* multi_operator(MultiArithmeticOperator op) noexcept
{
    switch (op)
    {
        case MultiArithmeticOperator::Add:
            return "+";
        case MultiArithmeticOperator::Multiply:
            return "*";
    }
    return "+";
}

inline const char* numeric_effect_operator(NumericEffectOperator op) noexcept
{
    switch (op)
    {
        case NumericEffectOperator::Assign:
            return "assign";
        case NumericEffectOperator::Increase:
            return "increase";
        case NumericEffectOperator::Decrease:
            return "decrease";
        case NumericEffectOperator::ScaleUp:
            return "scale-up";
        case NumericEffectOperator::ScaleDown:
            return "scale-down";
    }
    return "assign";
}

inline void number(double value, std::ostream& out)
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

inline bool is_builtin_type(TypeView type)
{
    const auto name = text(type.get_name());
    return name == "object" || name == "number";
}

inline void type_list(EntityListView<Type> types, std::ostream& out)
{
    auto first = true;
    for (auto type : types)
    {
        if (!first)
            out << ' ';
        first = false;
        atom_text(type.get_name(), out);
    }
}

inline void term(TermView term, std::ostream& out)
{
    std::visit(
        [&](const auto& value)
        {
            using T = std::remove_cvref_t<decltype(value)>;
            if constexpr (std::same_as<T, ygg::Index<Variable>>)
                variable_text(term.get_context()[value].name, out);
            else
                atom_text(term.get_context()[value].name, out);
        },
        term.get_data().value);
}

inline void term_list(EntityListView<Term> terms, std::ostream& out)
{
    for (auto term_view : terms)
    {
        out << ' ';
        term(term_view, out);
    }
}

inline void parameter(ParameterView parameter, std::ostream& out)
{
    variable_text(parameter.get_variable().get_name(), out);
    if (!parameter.get_types().empty())
    {
        out << " - ";
        type_list(parameter.get_types(), out);
    }
}

inline void parameters(EntityListView<Parameter> parameters, std::ostream& out)
{
    auto first = true;
    for (auto parameter_view : parameters)
    {
        if (!first)
            out << ' ';
        first = false;
        parameter(parameter_view, out);
    }
}

inline void objects(EntityListView<Object> objects, std::ostream& out)
{
    auto first = true;
    for (auto object : objects)
    {
        if (!first)
            out << ' ';
        first = false;
        atom_text(object.get_name(), out);
        if (!object.get_types().empty())
        {
            out << " - ";
            type_list(object.get_types(), out);
        }
    }
}

inline void atom(AtomView atom, std::ostream& out)
{
    out << '(';
    atom_text(atom.get_predicate().get_name(), out);
    term_list(atom.get_terms(), out);
    out << ')';
}

inline void literal(LiteralView literal, std::ostream& out)
{
    if (literal.get_data().m_polarity)
        atom(literal.get_atom(), out);
    else
    {
        out << "(not ";
        atom(literal.get_atom(), out);
        out << ')';
    }
}

inline void function_term(FunctionTermView function, std::ostream& out)
{
    out << '(';
    atom_text(function.get_function().get_name(), out);
    term_list(function.get_terms(), out);
    out << ')';
}

inline void function_expression(FunctionExpressionView expression, std::ostream& out);

inline void function_expression_node(FunctionExpressionNumberView expression, std::ostream& out) { number(expression.get_data().value, out); }
inline void function_expression_node(FunctionTermView expression, std::ostream& out) { function_term(expression, out); }
inline void function_expression_node(UnaryFunctionExpressionView expression, std::ostream& out)
{
    out << "(- ";
    function_expression(expression.get_expression(), out);
    out << ')';
}
inline void function_expression_node(BinaryFunctionExpressionView expression, std::ostream& out)
{
    out << '(' << binary_operator(expression.get_data().op) << ' ';
    function_expression(expression.get_left(), out);
    out << ' ';
    function_expression(expression.get_right(), out);
    out << ')';
}
inline void function_expression_node(MultiFunctionExpressionView expression, std::ostream& out)
{
    out << '(' << multi_operator(expression.get_data().op);
    for (auto child : expression.get_expressions())
    {
        out << ' ';
        function_expression(child, out);
    }
    out << ')';
}

inline void function_expression(FunctionExpressionView expression, std::ostream& out)
{
    std::visit([&](const auto& value) { function_expression_node(ygg::make_view(value, expression.get_context()), out); }, expression.get_data().value);
}

inline void condition(ConditionView condition, std::ostream& out);

inline void condition_list(EntityListView<Condition> conditions, std::ostream& out)
{
    for (auto child : conditions)
    {
        out << ' ';
        condition(child, out);
    }
}

inline void condition_node(ConditionLiteralView node, std::ostream& out) { literal(node.get_literal(), out); }
inline void condition_node(ConditionAndView node, std::ostream& out)
{
    out << "(and";
    condition_list(node.get_conditions(), out);
    out << ')';
}
inline void condition_node(ConditionOrView node, std::ostream& out)
{
    out << "(or";
    condition_list(node.get_conditions(), out);
    out << ')';
}
inline void condition_node(ConditionNotView node, std::ostream& out)
{
    out << "(not ";
    condition(node.get_condition(), out);
    out << ')';
}
inline void condition_node(ConditionImplyView node, std::ostream& out)
{
    out << "(imply ";
    condition(node.get_left(), out);
    out << ' ';
    condition(node.get_right(), out);
    out << ')';
}
inline void condition_node(ConditionExistsView node, std::ostream& out)
{
    out << "(exists (";
    parameters(node.get_parameters(), out);
    out << ") ";
    condition(node.get_condition(), out);
    out << ')';
}
inline void condition_node(ConditionForallView node, std::ostream& out)
{
    out << "(forall (";
    parameters(node.get_parameters(), out);
    out << ") ";
    condition(node.get_condition(), out);
    out << ')';
}
inline void condition_node(ConditionNumericConstraintView node, std::ostream& out)
{
    out << '(' << comparator(node.get_data().comparator) << ' ';
    function_expression(node.get_left(), out);
    out << ' ';
    function_expression(node.get_right(), out);
    out << ')';
}

inline void condition(ConditionView condition, std::ostream& out)
{
    std::visit([&](const auto& value) { condition_node(ygg::make_view(value, condition.get_context()), out); }, condition.get_data().value);
}

inline void effect(EffectView effect, std::ostream& out);

inline void effect_list(EntityListView<Effect> effects, std::ostream& out)
{
    for (auto child : effects)
    {
        out << ' ';
        effect(child, out);
    }
}

inline void effect_node(EffectLiteralView node, std::ostream& out) { literal(node.get_literal(), out); }
inline void effect_node(EffectAndView node, std::ostream& out)
{
    out << "(and";
    effect_list(node.get_effects(), out);
    out << ')';
}
inline void effect_node(EffectNumericView node, std::ostream& out)
{
    out << '(' << numeric_effect_operator(node.get_data().op) << ' ';
    out << '(';
    atom_text(node.get_function().get_name(), out);
    term_list(node.get_terms(), out);
    out << ") ";
    function_expression(node.get_expression(), out);
    out << ')';
}
inline void effect_node(EffectForallView node, std::ostream& out)
{
    out << "(forall (";
    parameters(node.get_parameters(), out);
    out << ") ";
    effect(node.get_effect(), out);
    out << ')';
}
inline void effect_node(EffectWhenView node, std::ostream& out)
{
    out << "(when ";
    condition(node.get_condition(), out);
    out << ' ';
    effect(node.get_effect(), out);
    out << ')';
}
inline void effect_node(EffectOneOfView node, std::ostream& out)
{
    out << "(oneof";
    effect_list(node.get_effects(), out);
    out << ')';
}
inline void effect_node(EffectProbabilisticView node, std::ostream& out)
{
    out << "(probabilistic";
    for (auto alternative : node.get_alternatives())
    {
        out << ' ';
        number(alternative.get_data().probability, out);
        out << ' ';
        effect(alternative.get_effect(), out);
    }
    out << ')';
}

inline void effect(EffectView effect, std::ostream& out)
{
    std::visit([&](const auto& value) { effect_node(ygg::make_view(value, effect.get_context()), out); }, effect.get_data().value);
}

inline void predicate(PredicateView predicate, std::ostream& out)
{
    out << '(';
    atom_text(predicate.get_name(), out);
    if (!predicate.get_parameters().empty())
    {
        out << ' ';
        parameters(predicate.get_parameters(), out);
    }
    out << ')';
}

inline void function_skeleton(FunctionSkeletonView function, std::ostream& out)
{
    out << '(';
    atom_text(function.get_name(), out);
    if (!function.get_parameters().empty())
    {
        out << ' ';
        parameters(function.get_parameters(), out);
    }
    out << ") - ";
    atom_text(function.get_type().get_name(), out);
}

inline void action(ActionView action, std::ostream& out, Options options)
{
    spaces(out, options.indent);
    out << "(:action ";
    atom_text(action.get_name(), out);
    const auto section = nested(options);
    out << '\n';
    spaces(out, section.indent);
    out << ":parameters (";
    parameters(action.get_parameters(), out);
    out << ')';
    if (action.get_precondition())
    {
        out << '\n';
        spaces(out, section.indent);
        out << ":precondition ";
        condition(action.get_precondition().value(), out);
    }
    if (action.get_effect())
    {
        out << '\n';
        spaces(out, section.indent);
        out << ":effect ";
        effect(action.get_effect().value(), out);
    }
    out << ")";
}

inline void axiom(AxiomView axiom, std::ostream& out, Options options)
{
    auto head_variables = ygg::UnorderedSet<ygg::uint_t> {};
    for (auto term_view : axiom.get_head().get_atom().get_terms())
    {
        std::visit(
            [&](const auto& value)
            {
                using T = std::remove_cvref_t<decltype(value)>;
                if constexpr (std::same_as<T, ygg::Index<Variable>>)
                    head_variables.insert(value.get_value());
            },
            term_view.get_data().value);
    }

    auto existential_parameters = std::vector<ParameterView> {};
    for (auto parameter_view : axiom.get_parameters())
    {
        if (!head_variables.contains(parameter_view.get_variable().get_index().get_value()))
            existential_parameters.push_back(parameter_view);
    }

    spaces(out, options.indent);
    out << "(:derived ";
    literal(axiom.get_head(), out);
    out << ' ';
    if (!existential_parameters.empty())
    {
        out << "(exists (";
        auto first = true;
        for (auto parameter_view : existential_parameters)
        {
            if (!first)
                out << ' ';
            first = false;
            parameter(parameter_view, out);
        }
        out << ") ";
        condition(axiom.get_condition(), out);
        out << ')';
    }
    else
    {
        condition(axiom.get_condition(), out);
    }
    out << ')';
}

inline void initial_function_value(InitialFunctionValueView value, std::ostream& out)
{
    out << "(= ";
    function_term(value.get_function(), out);
    out << ' ';
    function_expression(value.get_value(), out);
    out << ')';
}

inline void metric(MetricView metric, std::ostream& out)
{
    out << "(:metric " << (metric.get_data().minimize ? "minimize" : "maximize") << ' ';
    function_expression(metric.get_expression(), out);
    out << ')';
}

}  // namespace detail

inline void write(DomainView domain, std::ostream& out, Options options = {})
{
    out << "(define (domain ";
    detail::atom_text(domain.get_name(), out);
    out << ')';
    const auto section = detail::nested(options);
    if (!domain.get_requirements().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:requirements";
        for (auto requirement : domain.get_requirements())
            out << ' ' << detail::requirement(requirement.get_data().kind);
        out << ')';
    }
    if (!domain.get_types().empty())
    {
        auto wrote_header = false;
        for (auto type : domain.get_types())
        {
            if (detail::is_builtin_type(type))
                continue;
            if (!wrote_header)
            {
                out << '\n';
                detail::spaces(out, section.indent);
                out << "(:types";
                wrote_header = true;
            }
            out << ' ';
            detail::atom_text(type.get_name(), out);
            if (!type.get_bases().empty())
            {
                out << " - ";
                detail::type_list(type.get_bases(), out);
            }
        }
        if (wrote_header)
            out << ')';
    }
    if (!domain.get_constants().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:constants ";
        detail::objects(domain.get_constants(), out);
        out << ')';
    }
    if (!domain.get_predicates().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:predicates";
        for (auto pred : domain.get_predicates())
        {
            out << '\n';
            detail::spaces(out, section.indent + section.indent_step);
            detail::predicate(pred, out);
        }
        out << ')';
    }
    if (!domain.get_functions().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:functions";
        for (auto function : domain.get_functions())
        {
            out << '\n';
            detail::spaces(out, section.indent + section.indent_step);
            detail::function_skeleton(function, out);
        }
        out << ')';
    }
    for (auto axiom : domain.get_axioms())
    {
        out << '\n';
        detail::axiom(axiom, out, section);
    }
    for (auto action : domain.get_actions())
    {
        out << '\n';
        detail::action(action, out, section);
    }
    out << "\n)";
}

inline void write(TaskView task, std::ostream& out, Options options = {})
{
    out << "(define (problem ";
    detail::atom_text(task.get_name(), out);
    out << ")";
    const auto section = detail::nested(options);
    out << '\n';
    detail::spaces(out, section.indent);
    out << "(:domain ";
    detail::atom_text(task.get_domain().get_name(), out);
    out << ')';
    if (!task.get_requirements().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:requirements";
        for (auto requirement : task.get_requirements())
            out << ' ' << detail::requirement(requirement.get_data().kind);
        out << ')';
    }
    if (!task.get_objects().empty())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:objects ";
        detail::objects(task.get_objects(), out);
        out << ')';
    }
    out << '\n';
    detail::spaces(out, section.indent);
    out << "(:init";
    for (auto literal : task.get_initial_literals())
    {
        out << ' ';
        detail::literal(literal, out);
    }
    for (auto value : task.get_initial_function_values())
    {
        out << ' ';
        detail::initial_function_value(value, out);
    }
    out << ')';
    if (task.get_goal())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        out << "(:goal ";
        detail::condition(task.get_goal().value(), out);
        out << ')';
    }
    if (task.get_metric())
    {
        out << '\n';
        detail::spaces(out, section.indent);
        detail::metric(task.get_metric().value(), out);
    }
    for (auto axiom : task.get_axioms())
    {
        out << '\n';
        detail::axiom(axiom, out, section);
    }
    out << "\n)";
}

inline std::string domain(DomainView domain, Options options = {})
{
    auto out = std::ostringstream {};
    write(domain, out, options);
    return out.str();
}

inline std::string task(TaskView task, Options options = {})
{
    auto out = std::ostringstream {};
    write(task, out, options);
    return out.str();
}

}  // namespace loki::formalism::format

#endif
