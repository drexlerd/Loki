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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ACTION_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_ACTION_HPP_

#include "declarations.hpp"
#include "conditions.hpp"
#include "effects.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <functional>
#include <optional>


namespace loki {
template<typename... Ts>
class PersistentFactory;
}


namespace loki::pddl {
class ActionImpl : public Base<ActionImpl> {
private:
    std::string m_name;
    ParameterList m_parameters;
    std::optional<Condition> m_condition;
    std::optional<Effect> m_effect;

    ActionImpl(int identifier, const std::string& name, const ParameterList& parameters, const std::optional<Condition>& condition, const std::optional<Effect>& effect);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    /// @brief Test for structural equivalence
    bool are_equal_impl(const ActionImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const std::string& get_name() const;
    const ParameterList& get_parameters() const;
    const std::optional<Condition>& get_condition() const;
    const std::optional<Effect>& get_effect() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Action>
    {
        bool operator()(const loki::pddl::Action& left_action, const loki::pddl::Action& right_action) const;
    };

    template<>
    struct hash<loki::pddl::ActionImpl>
    {
        std::size_t operator()(const loki::pddl::ActionImpl& action) const;
    };
}


#endif
