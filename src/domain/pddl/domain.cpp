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

#include "../../../include/loki/domain/pddl/domain.hpp"

#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"

#include <deque>

using namespace std;


namespace loki::pddl {
DomainImpl::DomainImpl(int identifier,
    const std::string& name,
    const Requirements& requirements,
    const TypeList& types,
    const ObjectList& constants,
    const PredicateList& predicates)
    : Base(identifier)
    , m_name(name)
    , m_requirements(requirements)
    , m_types(types)
    , m_constants(constants)
    , m_predicates(predicates)
{
}

bool DomainImpl::are_equal_impl(const DomainImpl& other) const {
    return (m_name == other.m_name)
        && (m_requirements == other.m_requirements)
        && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types))
        && (get_sorted_vector(m_constants) == get_sorted_vector(other.m_constants))
        && (get_sorted_vector(m_predicates) == get_sorted_vector(other.m_predicates));
}

size_t DomainImpl::hash_impl() const {
    return hash_combine(
        m_name,
        m_requirements,
        hash_vector(get_sorted_vector(m_types)),
        hash_vector(get_sorted_vector(m_constants)),
        hash_vector(get_sorted_vector(m_predicates)));
}

void DomainImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << string(options.indent, ' ') << "(define (domain " << m_name << ")\n";
    auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent};
    if (!m_requirements->get_requirements().empty()) {
        out << string(nested_options.indent, ' ') << m_requirements->str() << "\n";
    }
    out << string(nested_options.indent, ' ') << "(:types ";
    std::unordered_map<std::vector<pddl::Type>, std::vector<pddl::Type>, hash_vector_type<pddl::Type>> subtypes_by_parent_types;
    for (const auto& type : m_types) {
        subtypes_by_parent_types[type->get_bases()].push_back(type);
    }
    size_t i = 0;
    for (const auto& pair : subtypes_by_parent_types) {
        if (i != 0) out << "\n" << string(nested_options.indent, ' ');
        for (const auto& subtype : pair.second) {
            //out <<
        }
        out << "\n";
        ++i;
    }
    out << ")\n";

    /*
    if (node.types.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.types.value(), nested_options) << "\n";
    }
    if (node.constants.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.constants.value(), nested_options) << "\n";
    }
    if (node.predicates.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.predicates.value(), nested_options) << "\n";
    }
    if (node.functions.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.functions.value(), nested_options) << "\n";
    }
    if (node.constraints.has_value()) {
        ss << string(nested_options.indent, ' ') << parse_text(node.constraints.value(), nested_options) << "\n";
    }
    for (size_t i = 0; i < node.structures.size(); ++i) {
        ss << string(nested_options.indent, ' ') << parse_text(node.structures[i], nested_options) << "\n";
    }
    */

    out << std::string(options.indent, ' ') << ")";
}

const std::string& DomainImpl::get_name() const {
    return m_name;
}

const Requirements& DomainImpl::get_requirements() const {
    return m_requirements;
}

const TypeList& DomainImpl::get_types() const {
    return m_types;
}

const ObjectList& DomainImpl::get_constants() const {
    return m_constants;
}

const PredicateList& DomainImpl::get_predicates() const {
    return m_predicates;
}

}

namespace std {
    bool less<loki::pddl::Domain>::operator()(
        const loki::pddl::Domain& left_domain,
        const loki::pddl::Domain& right_domain) const {
        return *left_domain < *right_domain;
    }

    std::size_t hash<loki::pddl::DomainImpl>::operator()(const loki::pddl::DomainImpl& domain) const {
        return domain.hash_impl();
    }
}
