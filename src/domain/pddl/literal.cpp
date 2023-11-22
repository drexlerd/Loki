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

#include "../../../include/loki/domain/pddl/literal.hpp"
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/common/collections.hpp"


namespace loki::pddl {
LiteralImpl::LiteralImpl(int identifier, bool is_negated, const Predicate& predicate)
    : Base(identifier)
    , m_is_negated(is_negated)
    , m_predicate(predicate)
{
}

bool LiteralImpl::are_equal_impl(const LiteralImpl& other) const {
    return (m_is_negated == other.m_is_negated) && (m_predicate == other.m_predicate);
}

size_t LiteralImpl::hash_impl() const {
    return std::hash<Predicate>()(m_predicate);
}

bool LiteralImpl::is_negated() const {
    return m_is_negated;
}

const Predicate& LiteralImpl::get_predicate() const {
    return m_predicate;
}

}

namespace std {
    bool less<loki::pddl::Literal>::operator()(
        const loki::pddl::Literal& left_literal,
        const loki::pddl::Literal& right_literal) const {
        return *left_literal < *right_literal;
    }

    std::size_t hash<loki::pddl::LiteralImpl>::operator()(const loki::pddl::LiteralImpl& literal) const {
        return literal.hash_impl();
    }
}
