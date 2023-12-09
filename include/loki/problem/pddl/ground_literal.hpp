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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_LITERAL_HPP_

#include "declarations.hpp"

#include "ground_atom.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class GroundLiteralImpl : public Base<GroundLiteralImpl> {
private:
    bool m_is_negated;
    GroundAtom m_atom;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    GroundLiteralImpl(int identifier, bool is_negated, const GroundAtom& atom);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const GroundLiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    bool is_negated() const;
    const GroundAtom& get_atom() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundLiteral>
    {
        bool operator()(const loki::pddl::GroundLiteral& left_literal, const loki::pddl::GroundLiteral& right_literal) const;
    };

    template<>
    struct hash<loki::pddl::GroundLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::GroundLiteralImpl& literal) const;
    };
}

#endif
