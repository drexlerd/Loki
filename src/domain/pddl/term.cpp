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

#include "../../../include/loki/domain/pddl/term.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
/* TermImpl */
TermImpl::TermImpl(int identifier)
    : Base(identifier)
{
}

TermImpl::~TermImpl() = default;


/* TermConstantImpl */
TermConstantImpl::TermConstantImpl(int identifier, const Object& object)
    : TermImpl(identifier), m_object(object) { }

bool TermConstantImpl::are_equal_impl(const TermImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TermConstantImpl&>(other);
        return m_object == other_derived.m_object;
    }
    return false;
}

size_t TermConstantImpl::hash_impl() const {
    return hash_combine(m_object);
}

void TermConstantImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void TermConstantImpl::accept(TermVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Object& TermConstantImpl::get_object() const {
    return m_object;
}


/* TermVariableImpl */

TermVariableImpl::TermVariableImpl(int identifier, const Variable& variable)
    : TermImpl(identifier), m_variable(variable) { }

bool TermVariableImpl::are_equal_impl(const TermImpl& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TermVariableImpl&>(other);
        return m_variable == other_derived.m_variable;
    }
    return false;
}

size_t TermVariableImpl::hash_impl() const {
    return hash_combine(m_variable);
}

void TermVariableImpl::str_impl(std::stringstream& out, const FormattingOptions& options) const {
    out << "TODO";
}

void TermVariableImpl::accept(TermVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const Variable& TermVariableImpl::get_variable() const {
    return m_variable;
}

}

namespace std {
    bool less<loki::pddl::Term>::operator()(
        const loki::pddl::Term& left_term,
        const loki::pddl::Term& right_term) const {
        return *left_term < *right_term;
    }

    std::size_t hash<loki::pddl::TermConstantImpl>::operator()(const loki::pddl::TermConstantImpl& term) const {
        return term.hash_impl();
    }

    std::size_t hash<loki::pddl::TermVariableImpl>::operator()(const loki::pddl::TermVariableImpl& term) const {
        return term.hash_impl();
    }
}
