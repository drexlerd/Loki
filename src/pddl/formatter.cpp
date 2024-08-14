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

#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"

#include <cassert>
#include <sstream>

namespace loki
{

PDDLFormatter::PDDLFormatter(size_t indent, size_t add_indent) : m_indent(indent), m_add_indent(add_indent) {}

void PDDLFormatter::write(const ActionImpl& element, std::ostream& out)
{
    out << std::string(m_indent, ' ') << "(:action " << element.get_name() << "\n";

    m_indent += m_add_indent;

    out << std::string(m_indent, ' ') << ":parameters (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ")";
    out << "\n";
    out << std::string(m_indent, ' ') << ":conditions ";
    if (element.get_condition().has_value())
        write(*element.get_condition().value(), out);
    else
        out << "()";

    out << "\n";
    out << std::string(m_indent, ' ') << ":effects ";
    if (element.get_effect().has_value())
        write(*element.get_effect().value(), out);
    else
        out << "()";
    out << ")\n";

    m_indent -= m_add_indent;
}

void PDDLFormatter::write(const AtomImpl& element, std::ostream& out)
{
    out << "(" << element.get_predicate()->get_name();
    for (size_t i = 0; i < element.get_terms().size(); ++i)
    {
        out << " ";
        write(*element.get_terms()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const AxiomImpl& element, std::ostream& out)
{
    out << std::string(m_indent, ' ') << "(:derived " << element.get_derived_predicate_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << "\n";

    m_indent -= m_add_indent;

    out << std::string(m_indent, ' ');
    write(*element.get_condition(), out);
    out << ")\n";

    m_indent -= m_add_indent;
}

void PDDLFormatter::write(const ConditionLiteralImpl& element, std::ostream& out) { write(*element.get_literal(), out); }

void PDDLFormatter::write(const ConditionAndImpl& element, std::ostream& out)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_conditions()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const ConditionOrImpl& element, std::ostream& out)
{
    out << "(or ";
    for (size_t i = 0; i < element.get_conditions().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_conditions()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const ConditionNotImpl& element, std::ostream& out)
{
    out << "(not ";
    write(*element.get_condition(), out);
    out << ")";
}

void PDDLFormatter::write(const ConditionImplyImpl& element, std::ostream& out)
{
    out << "(imply ";
    write(*element.get_condition_left(), out);
    out << " ";
    write(*element.get_condition_right(), out);
    out << ")";
}

void PDDLFormatter::write(const ConditionExistsImpl& element, std::ostream& out)
{
    out << "(exists (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ") ";
    write(*element.get_condition(), out);
    out << ")";
}

void PDDLFormatter::write(const ConditionForallImpl& element, std::ostream& out)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ") ";
    write(*element.get_condition(), out);
    out << ")";
}

void PDDLFormatter::write(const ConditionImpl& element, std::ostream& out)
{
    std::visit([this, &out](const auto& arg) { this->write(arg, out); }, element);
}

void PDDLFormatter::write(const DomainImpl& element, std::ostream& out)
{
    out << std::string(m_indent, ' ') << "(define (domain " << element.get_name() << ")\n";

    m_indent += m_add_indent;

    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(m_indent, ' ');
        write(*element.get_requirements(), out);
        out << "\n";
    }
    if (!element.get_types().empty())
    {
        out << std::string(m_indent, ' ') << "(:types ";
        std::unordered_map<TypeList, TypeList, UniquePDDLHasher<TypeList>> subtypes_by_parent_types;
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
                out << "\n" << std::string(m_indent, ' ');
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
        out << std::string(m_indent, ' ') << "(:constants ";
        std::unordered_map<TypeList, ObjectList, UniquePDDLHasher<TypeList>> constants_by_types;
        for (const auto& constant : element.get_constants())
        {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t i = 0;
        for (const auto& pair : constants_by_types)
        {
            if (i != 0)
                out << "\n" << std::string(m_indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                write(*constants[i], out);
            }
            if (element.get_requirements()->test(RequirementEnum::TYPING))
            {
                out << " - ";
                const auto& types = pair.first;
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    write(*types[i], out);
                }
            }
            ++i;
        }
        out << ")\n";
    }
    if (!element.get_predicates().empty())
    {
        out << std::string(m_indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < element.get_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_predicates()[i], out);
        }
        out << ")\n";
    }
    if (!element.get_functions().empty())
    {
        out << std::string(m_indent, ' ') << "(:functions ";
        for (size_t i = 0; i < element.get_functions().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_functions()[i], out);
        }
    }

    for (const auto& action : element.get_actions())
    {
        write(*action, out);
    }

    for (const auto& axiom : element.get_axioms())
    {
        write(*axiom, out);
    }

    m_indent -= m_add_indent;

    out << std::string(m_indent, ' ') << ")";
}

void PDDLFormatter::write(const EffectLiteralImpl& element, std::ostream& out) { write(*element.get_literal(), out); }

void PDDLFormatter::write(const EffectAndImpl& element, std::ostream& out)
{
    out << "(and ";
    for (size_t i = 0; i < element.get_effects().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_effects()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const EffectNumericImpl& element, std::ostream& out)
{
    out << "(" << to_string(element.get_assign_operator()) << " ";
    write(*element.get_function(), out);
    out << " ";
    write(*element.get_function_expression(), out);
    out << ")";
}

void PDDLFormatter::write(const EffectConditionalForallImpl& element, std::ostream& out)
{
    out << "(forall (";
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        if (i != 0)
            out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ") ";
    write(*element.get_effect(), out);
    out << ")";
}

void PDDLFormatter::write(const EffectConditionalWhenImpl& element, std::ostream& out)
{
    out << "(when ";
    write(*element.get_condition(), out);
    out << " ";
    write(*element.get_effect(), out);
    out << ")";
}

void PDDLFormatter::write(const EffectImpl& element, std::ostream& out)
{
    std::visit([this, &out](const auto& arg) { this->write(arg, out); }, element);
}

void PDDLFormatter::write(const FunctionExpressionNumberImpl& element, std::ostream& out) { out << element.get_number(); }

void PDDLFormatter::write(const FunctionExpressionBinaryOperatorImpl& element, std::ostream& out)
{
    out << "(" << to_string(element.get_binary_operator()) << " ";
    write(*element.get_left_function_expression(), out);
    out << " ";
    write(*element.get_right_function_expression(), out);
    out << ")";
}

void PDDLFormatter::write(const FunctionExpressionMultiOperatorImpl& element, std::ostream& out)
{
    out << "(" << to_string(element.get_multi_operator());
    assert(!element.get_function_expressions().empty());
    for (const auto& function_expression : element.get_function_expressions())
    {
        out << " ";
        write(*function_expression, out);
    }
    out << ")";
}

void PDDLFormatter::write(const FunctionExpressionMinusImpl& element, std::ostream& out)
{
    out << "(- ";
    write(*element.get_function_expression(), out);
    out << ")";
}

void PDDLFormatter::write(const FunctionExpressionFunctionImpl& element, std::ostream& out) { write(*element.get_function(), out); }

void PDDLFormatter::write(const FunctionExpressionImpl& element, std::ostream& out)
{
    std::visit([this, &out](const auto& arg) { this->write(arg, out); }, element);
}

void PDDLFormatter::write(const FunctionSkeletonImpl& element, std::ostream& out)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const FunctionImpl& element, std::ostream& out)
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
            write(*element.get_terms()[i], out);
        }
        out << "))";
    }
}

void PDDLFormatter::write(const LiteralImpl& element, std::ostream& out)
{
    if (element.is_negated())
    {
        out << "(not ";
        write(*element.get_atom(), out);
        out << ")";
    }
    else
    {
        write(*element.get_atom(), out);
    }
}

void PDDLFormatter::write(const OptimizationMetricImpl& element, std::ostream& out)
{
    out << "(" << to_string(element.get_optimization_metric()) << " ";
    write(*element.get_function_expression(), out);
    out << ")";
}

void PDDLFormatter::write(const NumericFluentImpl& element, std::ostream& out)
{
    out << "(= ";
    write(*element.get_function(), out);
    out << " " << element.get_number() << ")";
}

void PDDLFormatter::write(const ObjectImpl& element, std::ostream& out)
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
                write(*element.get_bases()[i], out);
            }
            out << ")";
        }
        else if (element.get_bases().size() == 1)
        {
            write(*element.get_bases().front(), out);
        }
    }
}

void PDDLFormatter::write(const ParameterImpl& element, std::ostream& out)
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

void PDDLFormatter::write(const PredicateImpl& element, std::ostream& out)
{
    out << "(" << element.get_name();
    for (size_t i = 0; i < element.get_parameters().size(); ++i)
    {
        out << " ";
        write(*element.get_parameters()[i], out);
    }
    out << ")";
}

void PDDLFormatter::write(const ProblemImpl& element, std::ostream& out)
{
    out << std::string(m_indent, ' ') << "(define (problem " << element.get_name() << ")\n";

    m_indent += m_add_indent;

    out << std::string(m_indent, ' ') << "(:domain " << element.get_domain()->get_name() << ")\n";
    if (!element.get_requirements()->get_requirements().empty())
    {
        out << std::string(m_indent, ' ');
        write(*element.get_requirements(), out);
        out << "\n";
    }

    if (!element.get_objects().empty())
    {
        out << std::string(m_indent, ' ') << "(:objects ";
        std::unordered_map<TypeList, ObjectList, UniquePDDLHasher<TypeList>> objects_by_types;
        for (const auto& object : element.get_objects())
        {
            objects_by_types[object->get_bases()].push_back(object);
        }
        size_t i = 0;
        for (const auto& [types, objects] : objects_by_types)
        {
            if (i != 0)
                out << "\n" << std::string(m_indent, ' ');
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
        out << std::string(m_indent, ' ') << "(:derived-predicates ";
        for (size_t i = 0; i < element.get_derived_predicates().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_derived_predicates()[i], out);
        }
        out << ")\n";
    }

    if (!(element.get_initial_literals().empty() && element.get_numeric_fluents().empty()))
    {
        out << std::string(m_indent, ' ') << "(:init ";
        for (size_t i = 0; i < element.get_initial_literals().size(); ++i)
        {
            if (i != 0)
                out << " ";
            write(*element.get_initial_literals()[i], out);
        }
        for (size_t i = 0; i < element.get_numeric_fluents().size(); ++i)
        {
            out << " ";
            write(*element.get_numeric_fluents()[i], out);
        }
    }
    out << ")\n";

    if (element.get_goal_condition().has_value())
    {
        out << std::string(m_indent, ' ') << "(:goal ";
        write(*element.get_goal_condition().value(), out);
        out << ")\n";
    }

    if (element.get_optimization_metric().has_value())
    {
        out << std::string(m_indent, ' ') << "(:metric ";
        write(*element.get_optimization_metric().value(), out);
        out << ")\n";
    }

    for (const auto& axiom : element.get_axioms())
    {
        write(*axiom, out);
    }

    m_indent -= m_add_indent;

    out << std::string(m_indent, ' ') << ")";
}

void PDDLFormatter::write(const RequirementsImpl& element, std::ostream& out)
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

void PDDLFormatter::write(const TermObjectImpl& element, std::ostream& out) { out << element.get_object()->get_name(); }

void PDDLFormatter::write(const TermVariableImpl& element, std::ostream& out) { out << element.get_variable()->get_name(); }

void PDDLFormatter::write(const TermImpl& element, std::ostream& out)
{
    std::visit([this, &out](const auto& arg) { this->write(arg, out); }, element);
}

void PDDLFormatter::write(const TypeImpl& element, std::ostream& out)
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

void PDDLFormatter::write(const VariableImpl& element, std::ostream& out) { out << element.get_name(); }

}