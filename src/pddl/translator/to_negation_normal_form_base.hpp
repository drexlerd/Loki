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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_BASE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_BASE_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{
template<typename Derived>
class ToNegationNormalFormTranslatorBase : public RecursiveCachedBaseTranslator<ToNegationNormalFormTranslatorBase<Derived>>
{
private:
    ToNegationNormalFormTranslatorBase() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

protected:
    friend class RecursiveCachedBaseTranslator<ToNegationNormalFormTranslatorBase<Derived>>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslatorBase<Derived>>::prepare_level_2;
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslatorBase<Derived>>::translate_level_2;

    ///////////////////////////////////////////////////////
    /// Prepare
    ///////////////////////////////////////////////////////

    template<std::ranges::forward_range Range>
    void prepare_level_2(const Range& range)
    {
        self().prepare_level_3(range);
    }
    template<std::ranges::forward_range Range>
    void prepare_level_3(const Range& range)
    {
        std::ranges::for_each(range, [this](auto&& arg) { this->prepare_level_0(arg); });
    }
    template<typename T>
    void prepare_level_2(const T& element)
    {
        self().prepare_level_3(element);
    }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    /// @brief Translate a container of elements into a container of elements.
    ///        Default behavior reparses it into the pddl_repositories.
    template<IsBackInsertibleRange Range>
    auto translate_level_2(const Range& input)
    {
        return self().translate_level_3(input);
    }
    template<IsBackInsertibleRange Range>
    auto translate_level_3(const Range& input)
    {
        std::remove_cvref_t<Range> output;

        if constexpr (requires { output.reserve(std::ranges::size(input)); })
        {
            output.reserve(std::ranges::size(input));
        }

        std::ranges::transform(input, std::back_inserter(output), [this](auto&& arg) { return this->translate_level_0(arg); });

        return output;
    }

    /// @brief Translate a single element.
    ///        Default behavior reparses it into the pddl_repositories.
    template<typename T>
    auto translate_level_2(const T& element)
    {
        return self().translate_level_3(element);
    }
};
}

#endif
