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

#include <iostream>
#include <loki/loki.hpp>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: interpreter <domain:str>" << std::endl;
        return 1;
    }
    const auto domain_file = std::string { argv[1] };

    // 1. Parse the domain
    const auto domain_parser = loki::DomainParser(domain_file);
    const auto domain = domain_parser.get_domain();
    std::cout << *domain << std::endl;

    return 0;
}
