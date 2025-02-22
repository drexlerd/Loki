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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_BASE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_BASE_HPP_

#include "interface.hpp"

namespace loki
{
template<typename Derived>
class RecursiveBaseTranslator : public ITranslator<RecursiveBaseTranslator<Derived>>
{
private:
    RecursiveBaseTranslator() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

protected:
    friend class ITranslator<RecursiveBaseTranslator<Derived>>;

    ///////////////////////////////////////////////////////
    /// Prepare
    ///////////////////////////////////////////////////////

    template<std::ranges::forward_range Range>
    void prepare_level_1(const Range& range)
    {
        self().prepare_level_2(range);
    }
    template<std::ranges::forward_range Range>
    void prepare_level_2(const Range& range)
    {
        std::ranges::for_each(range, [this](auto&& arg) { this->prepare_level_0(arg); });
    }
    template<typename T>
    void prepare_level_1(const T& element)
    {
        self().prepare_level_2(element);
    }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    template<IsBackInsertibleRange Range>
    auto translate_level_1(const Range& input)
    {
        return self().translate_level_2(input);
    }
    template<IsBackInsertibleRange Range>
    auto translate_level_2(const Range& input)
    {
        std::remove_cvref_t<Range> output;

        if constexpr (requires { output.reserve(std::ranges::size(input)); })
        {
            output.reserve(std::ranges::size(input));
        }

        std::ranges::transform(input, std::back_inserter(output), [this](auto&& arg) { return this->translate_level_0(arg); });

        return output;
    }

    template<typename T>
    auto translate_level_1(const T& element)
    {
        return self().translate_level_2(element);
    }
};
}

#endif
