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

#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/problem.hpp"

namespace loki
{

/// @brief `ITranslator` is the base class for translators.
///
/// We implement a four-level CRTP-based class hierarchy.
/// At level 0, we have the `ITranslator` interface.
/// At level 1, we have base translators that provide translation functionality that is shared among domain and problem.
/// At level 3, we have concrete translators that provide translation functionality that is shared among domain and problem.
/// At level 4, we have domain and problem specific translators that provide their specific translation steps.
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
    ///////////////////////////////////////////////////////
    /// Prepare
    ///////////////////////////////////////////////////////

    template<typename T>
    auto prepare_level_0(const T& element)
    {
        self().prepare_level_1(element);
    }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    template<typename T, typename Builder>
    auto translate_level_0(const T& element, Builder& builder)
    {
        return self().translate_level_1(element, builder);
    }

    /**
     * For domain and problem we provide specialized functions since they are treated fundamentally different,
     * i.e., there is no entry in the HanaRepositories and we need to pass additional information to create default behaviors.
     */

    auto translate_level_0(const Domain& domain, DomainBuilder& builder) { return self().translate_level_1(domain, builder); }

    auto translate_level_0(const Problem& problem, ProblemBuilder& builder) { return self().translate_level_1(problem, builder); }
};
}

#endif
