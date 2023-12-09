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

#include "../../../include/loki/problem/pddl/ground_conditions.hpp"

#include "../../../include/loki/problem/pddl/ground_literal.hpp"


namespace loki::pddl {
/* Literal */
ConditionGroundLiteralImpl::ConditionGroundLiteralImpl(int identifier, const GroundLiteral& literal)
    : ConditionImpl(identifier)
    , m_literal(literal) { }

bool ConditionGroundLiteralImpl::are_equal_impl(const ConditionImpl& other) const {
    // https://stackoverflow.com/questions/11332075/comparing-polymorphic-base-types-in-c-without-rtti
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const ConditionGroundLiteralImpl&>(other);
        return m_literal == other_derived.m_literal;
    }
    return false;
}

size_t ConditionGroundLiteralImpl::hash_impl() const {
    return std::hash<GroundLiteral>()(m_literal);
}

void ConditionGroundLiteralImpl::str_impl(std::ostringstream& out, const FormattingOptions& options) const {
    out << *m_literal;
}

void ConditionGroundLiteralImpl::accept(ConditionVisitor& visitor) const {
    visitor.visit(this->shared_from_this());
}

const GroundLiteral& ConditionGroundLiteralImpl::get_literal() const {
    return m_literal;
}

}


namespace std {
    std::size_t hash<loki::pddl::ConditionGroundLiteralImpl>::operator()(const loki::pddl::ConditionGroundLiteralImpl& condition) const {
        return condition.hash_impl();
    }
}
