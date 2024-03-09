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
#include <loki/domain/parser.hpp>
#include <loki/problem/parser.hpp>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: interpreter <domain:str> <problem:str>" << std::endl;
        return 1;
    }
    const auto domain_file = std::string { argv[1] };
    const auto problem_file = std::string { argv[2] };

    // 1. Parse the domain
    auto domain_parser = loki::DomainParser(domain_file);
    const auto domain = domain_parser.get_domain();
    std::cout << *domain << std::endl;

    // 2. Parse the problem
    const auto problem_parser = loki::ProblemParser(problem_file, domain_parser);
    const auto problem = problem_parser.get_problem();
    std::cout << *problem << std::endl;

    return 0;
}
