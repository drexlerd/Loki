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

#include "loki/pddl/derived_predicate.hpp"

#include "loki/pddl/conditions.hpp"
#include "loki/pddl/effects.hpp"
#include "loki/pddl/predicate.hpp"
#include "loki/pddl/visitors.hpp"
#include "loki/utils/collections.hpp"
#include "loki/utils/hash.hpp"

namespace loki::pddl
{
DerivedPredicateImpl::DerivedPredicateImpl(int identifier, Predicate predicate, Condition condition) :
    Base(identifier),
    m_predicate(std::move(predicate)),
    m_condition(std::move(condition))
{
}

bool DerivedPredicateImpl::is_structurally_equivalent_to_impl(const DerivedPredicateImpl& other) const
{
    return (m_predicate == other.m_predicate) && (m_condition == other.m_condition);
}

size_t DerivedPredicateImpl::hash_impl() const { return hash_combine(m_predicate, m_condition); }

void DerivedPredicateImpl::str_impl(std::ostream& out, const FormattingOptions& options) const
{
    out << std::string(options.indent, ' ') << "(:derived ";
    out << *m_predicate << " ";
    std::visit(StringifyVisitor(out, options), *m_condition);
    out << ")";
}

const Predicate& DerivedPredicateImpl::get_predicate() const { return m_predicate; }

const Condition& DerivedPredicateImpl::get_condition() const { return m_condition; }

}
