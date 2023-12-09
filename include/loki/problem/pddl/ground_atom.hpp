/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_ATOM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_ATOM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundAtomImpl : public Base<AtomImpl> {
private:
    Predicate m_predicate;
    ObjectList m_objects;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    GroundAtomImpl(int identifier, const Predicate& predicate, const ObjectList& objects);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const AtomImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const Predicate& get_predicate() const;
    const TermList& get_terms() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundAtom>
    {
        bool operator()(const loki::pddl::GroundAtom& left_atom, const loki::pddl::GroundAtom& right_atom) const;
    };

    template<>
    struct hash<loki::pddl::GroundAtomImpl>
    {
        std::size_t operator()(const loki::pddl::GroundAtomImpl& atom) const;
    };
}

#endif
