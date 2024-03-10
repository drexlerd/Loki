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

#ifndef LOKI_INCLUDE_LOKI_PDDL_VISITORS_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_VISITORS_HPP_

#include "loki/utils/printer.hpp"

#include <sstream>

namespace loki::pddl
{
struct LessComparatorVisitor
{
    template<typename T1, typename T2>
    bool operator()(const T1& object_left, const T2& object_right) const
    {
        return object_left.get_identifier() < object_right.get_identifier();
    }
};

struct StringifyVisitor
{
    std::ostringstream& out;
    const FormattingOptions& options;

    StringifyVisitor(std::ostringstream& out_, const FormattingOptions& options_) : out(out_), options(options_) {}

    template<typename T>
    void operator()(const T& object)
    {
        object.str(out, options);
    }
};

}

#endif
