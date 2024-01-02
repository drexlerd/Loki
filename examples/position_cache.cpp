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

#include "../include/loki/domain/parser.hpp"
#include "../include/loki/problem/parser.hpp"

#include <cassert>
#include <iostream>


/// @brief Prints an error message when encourtering an And-Condition.
struct TestUnsupportedAndConditionVisitor {
    const loki::Position position;
    const loki::PDDLPositionCache& position_cache;
    const loki::PDDLErrorHandler& error_handler; 

    TestUnsupportedAndConditionVisitor(
        const loki::Position position_,
        const loki::PDDLPositionCache& position_cache_,
        const loki::PDDLErrorHandler& error_handler_)
        : position(position_)
        , position_cache(position_cache_)
        , error_handler(error_handler_) { }

    /// @brief For leaf nodes we do not need to do anything 
    template<typename Node>
    void operator()(const Node&) { }

    /// @brief For inner nodes we need to recursively call the visitor
    void operator()(const loki::pddl::ConditionOrImpl& node) {
        for (const auto& child_node : node.get_conditions()) {
            // We call front() to obtain the first occurence.
            const auto child_position = position_cache.get<loki::pddl::ConditionImpl>(child_node).front();
            std::visit(TestUnsupportedAndConditionVisitor(child_position, position_cache, error_handler), *child_node);
        }
    }

    /// @brief For the unsupported And-Condition, 
    ///        we print an clang-style error message and throw an exception.
    void operator()(const loki::pddl::ConditionAndImpl& node) {
        std::cout << error_handler(position, "Your awesome error message.") << std::endl;
        throw std::runtime_error("Unexpected And-Condition.");
    }
};


/// @brief In this example, we show how to print custom error message for unsupported PDDL types.
int main() {
    // Parse the domain
    auto domain_parser = loki::DomainParser("benchmarks/gripper/domain.pddl");
    const auto domain = domain_parser.get_domain();
    std::cout << *domain << std::endl << std::endl;

    const loki::PDDLPositionCache& position_cache = domain_parser.get_position_cache();
    const loki::PDDLErrorHandler& error_handler = position_cache.get_error_handler();

    for (const auto& action : domain->get_actions()) {
        const auto condition = action->get_condition();
        if (!condition.has_value()) {
            continue;
        }
        // We call front() to obtain the first occurence.
        auto condition_position = position_cache.get<loki::pddl::ConditionImpl>(condition.value()).front();
        std::visit(
            TestUnsupportedAndConditionVisitor(condition_position, position_cache, error_handler), 
            *condition.value());
    }

    return 0;
}
