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


/// @brief This example illustrates incorrect ownership handling
int main() {
    auto domain = loki::pddl::Domain{nullptr};
    {
        // Parse the domain
        auto domain_parser = loki::DomainParser("benchmarks/gripper/domain.pddl");
        domain = domain_parser.get_domain();
        std::cout << *domain << std::endl;

        /* Destructor of DomainParser is called and all domain and problem specific PDDL objects are destroyed. */
    }

    std::cout << "Bye ;(" << std::endl;

    /* Undefined behavior when accessing the domain, usually the program crashes because memory was overwritten */
    std::cout << *domain << std::endl;

    return 0;
}
