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

#include "../../../include/loki/problem/pddl/ground_atom.hpp"
#include "../../../include/loki/common/hash.hpp"


namespace loki::pddl {
GroundAtomImpl::GroundAtomImpl(int identifier, const Predicate& predicate, const ObjectList& arguments)
    : Base<GroundAtomImpl>(identifier)
    , m_predicate(predicate)
    , m_arguments(arguments)
{
}

bool GroundAtomImpl::are_equal_impl(const GroundAtomImpl& other) const {
    return (m_predicate == other.m_predicate) && (m_arguments == other.m_arguments);
}

size_t GroundAtomImpl::hash_impl() const {
    return hash_combine(m_predicate, hash_vector(m_arguments));
}

const Predicate& GroundAtomImpl::get_predicate() const {
    return m_predicate;
}

const ObjectList& GroundAtomImpl::get_arguments() const {
    return m_arguments;
}

}

namespace std {
    bool less<loki::pddl::GroundAtom>::operator()(
        const loki::pddl::GroundAtom& left_atom,
        const loki::pddl::GroundAtom& right_atom) const {
        return *left_atom < *right_atom;
    }

    std::size_t hash<loki::pddl::GroundAtomImpl>::operator()(const loki::pddl::GroundAtomImpl& atom) const {
        return atom.hash();
    }
}
