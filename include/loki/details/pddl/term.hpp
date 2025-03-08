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

#ifndef LOKI_INCLUDE_LOKI_PDDL_TERM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_TERM_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <string>

namespace loki
{

/* TermImpl */

class TermImpl
{
private:
    size_t m_index;
    std::variant<Object, Variable> m_object_or_variable;

    TermImpl(size_t index, std::variant<Object, Variable> object_or_variable);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    TermImpl(const TermImpl& other) = delete;
    TermImpl& operator=(const TermImpl& other) = delete;
    TermImpl(TermImpl&& other) = default;
    TermImpl& operator=(TermImpl&& other) = default;

    size_t get_index() const;
    const std::variant<Object, Variable>& get_object_or_variable() const;

    auto identifying_members() const { return std::tuple(get_object_or_variable()); }
};

extern std::ostream& operator<<(std::ostream& out, const TermImpl& element);

extern std::ostream& operator<<(std::ostream& out, Term element);
}

#endif
