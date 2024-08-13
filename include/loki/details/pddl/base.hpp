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

#ifndef LOKI_INCLUDE_LOKI_PDDL_BASE_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_BASE_HPP_

#include "loki/details/utils/printer.hpp"

#include <algorithm>
#include <cstddef>
#include <sstream>
#include <vector>

namespace loki
{
/// @brief `Base` implements a common base class for PDDL objects.
/// PDDL objects are uniquely created through a factory and are therefore made uncopieable.
template<typename Derived>
class Base
{
private:
    constexpr const auto& self() const { return static_cast<Derived const&>(*this); }

    size_t m_index;

    explicit Base(size_t index) : m_index(index) {}
    friend Derived;

public:
    // moveable but not copyable
    Base(const Base& other) = delete;
    Base& operator=(const Base& other) = delete;
    Base(Base&& other) = default;
    Base& operator=(Base&& other) = default;

    /// @brief Overload of the output stream insertion operator (operator<<).
    friend std::ostream& operator<<(std::ostream& os, const Base& element)
    {
        os << element.str();
        return os;
    }

    /// @brief Compute a string representation of this object.
    void str(std::ostream& out, const FormattingOptions& options) const { self().str_impl(out, options); }

    /// @brief Compute a string representation of this object.
    std::string str() const
    {
        std::ostringstream out;
        FormattingOptions options { 0, 4 };
        str(out, options);
        return out.str();
    }

    /// @brief Get the index.
    size_t get_index() const { return m_index; }
};

}

#endif