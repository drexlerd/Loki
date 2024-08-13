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

#include <iostream>
#include <loki/loki.hpp>

/// @brief This example illustrates incorrect ownership handling
int main()
{
    auto domain = loki::Domain { nullptr };
    {
        // Parse the domain
        auto domain_parser = loki::DomainParser(std::string(DATA_DIR) + "gripper/domain.pddl");
        domain = domain_parser.get_domain();
        std::cout << loki::StreamWriter(*domain) << std::endl;

        /* Destructor of DomainParser is called and all domain and problem specific PDDL objects are destroyed. */
    }

    std::cout << "Bye ;(" << std::endl;

    /* Undefined behavior when accessing the domain, usually the program crashes because memory was overwritten */
    std::cout << loki::StreamWriter(*domain) << std::endl;

    return 0;
}
