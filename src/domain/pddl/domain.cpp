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

#include "../../../include/loki/domain/pddl/action.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/type.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"

#include <iostream>

using namespace std;


namespace loki::pddl {
DomainImpl::DomainImpl(int identifier,
    const std::string& name,
    const Requirements& requirements,
    const TypeList& types,
    const ObjectList& constants,
    const PredicateList& predicates,
    const ActionList& actions)
    : Base(identifier)
    , m_name(name)
    , m_requirements(requirements)
    , m_types(types)
    , m_constants(constants)
    , m_predicates(predicates)
    , m_actions(actions)
{
}

bool DomainImpl::are_equal_impl(const DomainImpl& other) const {
    return (m_name == other.m_name)
        && (m_requirements == other.m_requirements)
        && (get_sorted_vector(m_types) == get_sorted_vector(other.m_types))
        && (get_sorted_vector(m_constants) == get_sorted_vector(other.m_constants))
        && (get_sorted_vector(m_predicates) == get_sorted_vector(other.m_predicates))
        && (get_sorted_vector(m_actions) == get_sorted_vector(other.m_actions));
}

size_t DomainImpl::hash_impl() const {
    return hash_combine(
        m_name,
        m_requirements,
        hash_vector(get_sorted_vector(m_types)),
        hash_vector(get_sorted_vector(m_constants)),
        hash_vector(get_sorted_vector(m_predicates)),
        hash_vector(get_sorted_vector(m_actions)));
}

void DomainImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << string(options.indent, ' ') << "(define (domain " << m_name << ")\n";
    auto nested_options = FormattingOptions{options.indent + options.add_indent, options.add_indent};
    if (!m_requirements->get_requirements().empty()) {
        out << string(nested_options.indent, ' ') << m_requirements->str() << "\n";
    }
    if (!m_types.empty()) {
        out << string(nested_options.indent, ' ') << "(:types ";
        std::unordered_map<std::vector<pddl::Type>, std::vector<pddl::Type>, hash_vector_type<pddl::Type>> subtypes_by_parent_types;
        for (const auto& type : m_types) {
            subtypes_by_parent_types[type->get_bases()].push_back(type);
        }
        size_t i = 0;
        for (const auto& pair : subtypes_by_parent_types) {
            if (i != 0) out << "\n" << string(nested_options.indent, ' ');
            const auto& sub_types = pair.second;
            for (size_t i = 0; i < sub_types.size(); ++i) {
                if (i != 0) out << " ";
                out << *sub_types[i];
            }
            out << " - ";
            const auto& types = pair.first;
            for (size_t i = 0; i < types.size(); ++i) {
                if (i != 0) out << " ";
                out << *types[i];
            }
            ++i;
        }
        out << ")\n";
    }
    if (!m_constants.empty()) {
        out << string(nested_options.indent, ' ') << "(:constants ";
        std::unordered_map<std::vector<pddl::Type>, std::vector<pddl::Object>, hash_vector_type<pddl::Type>> constants_by_types;
        for (const auto& constant : m_constants) {
            constants_by_types[constant->get_bases()].push_back(constant);
        }
        size_t i = 0;
        for (const auto& pair : constants_by_types) {
            if (i != 0) out << "\n" << string(nested_options.indent, ' ');
            const auto& constants = pair.second;
            for (size_t i = 0; i < constants.size(); ++i) {
                if (i != 0) out << " ";
                out << *constants[i];
            }
            if (m_requirements->test(RequirementEnum::TYPING)) {
                out << " - ";
                const auto& types = pair.first;
                for (size_t i = 0; i < types.size(); ++i) {
                    if (i != 0) out << " ";
                    out << *types[i];
                }
            }
            ++i;
        }
        out << ")\n";
    }
    if (!m_predicates.empty()) {
        out << string(nested_options.indent, ' ') << "(:predicates ";
        for (size_t i = 0; i < m_predicates.size(); ++i) {
            if (i != 0) out << " ";
            m_predicates[i]->str(out, nested_options, m_requirements->test(RequirementEnum::TYPING));
        }
        out << ")\n";
    }

    /*
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

    for (const auto& action : m_actions) {
        action->str(out, nested_options);
    }

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
