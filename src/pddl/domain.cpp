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

#include "loki/pddl/domain.hpp"

#include "loki/pddl/action.hpp"
#include "loki/pddl/derived_predicate.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/object.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/type.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

#include <iostream>

using namespace std;

namespace loki::pddl
{
DomainImpl::DomainImpl(int identifier,
                       std::string name,
                       Requirements requirements,
                       TypeList types,
                       ObjectList constants,
                       PredicateList predicates,
                       FunctionSkeletonList functions,
                       ActionList actions,
                       DerivedPredicateList derived_predicates) :
    Base(identifier),
    m_name(std::move(name)),
    m_requirements(std::move(requirements)),
    m_types(std::move(types)),
    m_constants(std::move(constants)),
    m_predicates(std::move(predicates)),
    m_functions(std::move(functions)),
    m_actions(std::move(actions)),
    m_derived_predicates(std::move(derived_predicates))
{
}

bool DomainImpl::is_structurally_equivalent_to_impl(const DomainImpl& other) const
{
    return (m_name == other.m_name) && (m_requirements == other.m_requirements) && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types))
           && (get_sorted_vector(m_constants) == get_sorted_vector(other.m_constants))
           && (get_sorted_vector(m_predicates) == get_sorted_vector(other.m_predicates))
           && (get_sorted_vector(m_functions) == get_sorted_vector(other.m_functions)) && (get_sorted_vector(m_actions) == get_sorted_vector(other.m_actions))
           && (get_sorted_vector(m_derived_predicates) == get_sorted_vector(other.m_derived_predicates));
}

size_t DomainImpl::hash_impl() const
{
    return hash_combine(m_name,
                        m_requirements,
                        hash_container(get_sorted_vector(m_types)),
                        hash_container(get_sorted_vector(m_constants)),
                        hash_container(get_sorted_vector(m_predicates)),
                        hash_container(get_sorted_vector(m_functions)),
                        hash_container(get_sorted_vector(m_actions)),
                        hash_container(get_sorted_vector(m_derived_predicates)));
}

void DomainImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << string(options.indent, ' ') << "(define (domain " << m_name << ")\n";
    auto nested_options = FormattingOptions { options.indent + options.add_indent, options.add_indent };
    if (!m_requirements->get_requirements().empty())
    {
        out << string(nested_options.indent, ' ');
        m_requirements->str(out, nested_options);
        out << "\n";
    }
    if (!m_types.empty())
    {
        out << string(nested_options.indent, ' ') << "(:types ";
        std::unordered_map<pddl::TypeList, pddl::TypeList, hash_container_type<pddl::TypeList>> subtypes_by_parent_types;
        for (const auto& type : m_types)
        {
            subtypes_by_parent_types[type->get_bases()].push_back(type);
        }
        size_t i = 0;
        for (const auto& [types, sub_types] : subtypes_by_parent_types)
        {
            if (i != 0)
                out << "\n" << string(nested_options.indent, ' ');
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
    if (!m_constants.empty())
    {
        out << string(nested_options.indent, ' ') << "(:constants ";
        std::unordered_map<pddl::TypeList, pddl::ObjectList, hash_container_type<pddl::TypeList>> constants_by_types;
        for (const auto& constant : m_constants)
        {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t i = 0;
        for (const auto& pair : constants_by_types)
        {
            if (i != 0)
                out << "\n" << string(nested_options.indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i)
            {
                if (i != 0)
                    out << " ";
                constants[i]->str(out, nested_options);
            }
            if (m_requirements->test(RequirementEnum::TYPING))
            {
                out << " - ";
                const auto& types = pair.first;
                for (size_t i = 0; i < types.size(); ++i)
                {
                    if (i != 0)
                        out << " ";
                    types[i]->str(out, nested_options);
                }
            }
            ++i;
        }
        out << ")\n";
    }
    if (!m_predicates.empty())
    {
        out << string(nested_options.indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < m_predicates.size(); ++i)
        {
            if (i != 0)
                out << " ";
            m_predicates[i]->str(out, nested_options);
        }
        out << ")\n";
    }
    if (!m_functions.empty())
    {
        out << string(nested_options.indent, ' ') << "(:functions ";
        for (size_t i = 0; i < m_functions.size(); ++i)
        {
            if (i != 0)
                out << " ";
            m_functions[i]->str(out, nested_options);
        }
    }

    /*
    if (node.constraints.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.constraints.value(), nested_options) << "\n";
    }
    for (size_t i = 0; i < node.structures.size(); ++i) {
        ss << string(nested_options.indent, ' ') << parse_text(node.structures[i], nested_options) << "\n";
    }
    */

    for (const auto& action : m_actions)
    {
        action->str(out, nested_options);
    }

    for (const auto& derived_predicate : m_derived_predicates)
    {
        derived_predicate->str(out, nested_options);
    }

    out << std::string(options.indent, ' ') << ")";
}

std::ostream& operator<<(std::ostream& os, const DomainImpl& domain)
{
    domain.str(os, FormattingOptions { 0, 4 });
    return os;
}

const std::string& DomainImpl::get_name() const { return m_name; }

const Requirements& DomainImpl::get_requirements() const { return m_requirements; }

const TypeList& DomainImpl::get_types() const { return m_types; }

const ObjectList& DomainImpl::get_constants() const { return m_constants; }

const FunctionSkeletonList& DomainImpl::get_functions() const { return m_functions; }

const PredicateList& DomainImpl::get_predicates() const { return m_predicates; }

const ActionList& DomainImpl::get_actions() const { return m_actions; }

const DerivedPredicateList& DomainImpl::get_derived_predicates() const { return m_derived_predicates; }

}
