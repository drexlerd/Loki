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

#include "../../../include/loki/common/pddl/scope.hpp"


namespace loki {

ScopeStack::ScopeStack(
    ErrorHandler&& error_handler,
    const ScopeStack* parent)
    : m_error_handler(std::move(error_handler))
    , m_parent(parent) { }

void ScopeStack::open_scope() {
    m_stack.push_back(m_stack.empty()
        ? std::make_unique<Scope>()
        : std::make_unique<Scope>(m_stack.back().get()));
}

void ScopeStack::close_scope() {
    assert(!m_stack.empty());
    m_stack.pop_back();
}

const ErrorHandler& ScopeStack::get_error_handler() const {
    return m_error_handler;
}

const std::deque<std::unique_ptr<Scope>>& ScopeStack::get_stack() const {
    return m_stack;
}

}