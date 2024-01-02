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

#include <iostream>


int main() {
    // Parse the domain
    auto domain_parser = loki::DomainParser("benchmarks/gripper/domain.pddl");
    const auto domain = domain_parser.get_domain();
    std::cout << *domain << std::endl << std::endl;

    // Print occurences of domain constants.
    const loki::PDDLPositionCache& position_cache = domain_parser.get_position_cache();
    const loki::PDDLErrorHandler& error_handler = position_cache.get_error_handler();
    for (const auto constant : domain->get_constants()) {
        std::cout << "Constant name: " << constant->get_name() << std::endl;
        const auto& positions = position_cache.get<loki::pddl::ObjectImpl>(constant);
        std::cout << "Number of occurences: " << positions.size() << std::endl;
        for (auto position : positions) {
            const auto iterator = error_handler.position_of(position);
            std::cout << "String represenation from iterator: " << std::string(iterator.begin(), iterator.end()) << std::endl;
            std::cout << "Print a custom clang-style error message:" << std::endl;
            std::cout << error_handler(position, "Your awesome error message.") << std::endl;
        }
    }

    // Note: iterators can be compared to find positions of elements that are within other elements.

    return 0;
}
