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

#ifndef LOKI_INCLUDE_LOKI_PDDL_LITERAL_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_LITERAL_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{
class LiteralImpl
{
private:
    size_t m_index;
    bool m_polarity;
    Atom m_atom;

    LiteralImpl(size_t index, bool polarity, Atom atom);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    LiteralImpl(const LiteralImpl& other) = delete;
    LiteralImpl& operator=(const LiteralImpl& other) = delete;
    LiteralImpl(LiteralImpl&& other) = default;
    LiteralImpl& operator=(LiteralImpl&& other) = default;

    size_t get_index() const;
    bool get_polarity() const;
    Atom get_atom() const;

    auto identifying_members() const { return std::tuple(get_polarity(), get_atom()); }
};

extern std::ostream& operator<<(std::ostream& out, const LiteralImpl& element);

extern std::ostream& operator<<(std::ostream& out, Literal element);
}

#endif
