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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_INTERFACE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_INTERFACE_HPP_

namespace loki
{

/// @brief `ITranslator` is the base class for translators.
///
/// We implement a three-level CRTP-based class hierarchy.
/// The first level is the `ITranslator` interface.
/// The second level are base translators that provide translation functionality that is shared among domain and problem.
/// The third level are domain and problem specific translators that provide their specific translation steps.
/// @tparam Derived
template<typename Derived>
class ITranslator
{
private:
    ITranslator() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

public:
    /// @brief Traverse recusively and collect information.
    template<typename T>
    auto prepare(const T& element)
    {
        self().prepare_base(element);
    }

    /// @brief Translate recursively.
    template<typename T>
    auto translate(const T& element)
    {
        return self().translate_base(element);
    }
};
}

#endif
