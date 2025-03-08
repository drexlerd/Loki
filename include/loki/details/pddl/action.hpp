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

#ifndef LOKI_INCLUDE_LOKI_PDDL_ACTION_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_ACTION_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <optional>
#include <string>

namespace loki
{
class ActionImpl
{
private:
    size_t m_index;
    std::string m_name;
    // Indicate the original subseteq of variables before adding parameters during translations
    size_t m_original_arity;
    ParameterList m_parameters;
    std::optional<Condition> m_condition;
    std::optional<Effect> m_effect;

    ActionImpl(size_t index,
               std::string name,
               size_t original_arity,
               ParameterList parameters,
               std::optional<Condition> condition,
               std::optional<Effect> effect);

    // Give access to the constructor.
    template<typename T, typename Hash, typename EqualTo>
    friend class SegmentedRepository;

public:
    // moveable but not copyable
    ActionImpl(const ActionImpl& other) = delete;
    ActionImpl& operator=(const ActionImpl& other) = delete;
    ActionImpl(ActionImpl&& other) = default;
    ActionImpl& operator=(ActionImpl&& other) = default;

    size_t get_index() const;
    const std::string& get_name() const;
    size_t get_original_arity() const;
    const ParameterList& get_parameters() const;
    const std::optional<Condition>& get_condition() const;
    const std::optional<Effect>& get_effect() const;

    auto identifying_members() const
    {
        return std::tuple(std::cref(get_name()), get_original_arity(), std::cref(get_parameters()), get_condition(), get_effect());
    }
};

extern std::ostream& operator<<(std::ostream& out, const ActionImpl& element);

extern std::ostream& operator<<(std::ostream& out, Action element);

}

#endif
