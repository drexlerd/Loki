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

int main()
{
    // Parse the domain
    auto domain_parser = loki::DomainParser(std::string(DATA_DIR) + "gripper/domain.pddl");
    const auto domain = domain_parser.get_domain();
    std::cout << *domain << std::endl;

    // Parse first problem
    const auto problem_parser = loki::ProblemParser(std::string(DATA_DIR) + "gripper/p-2-0.pddl", domain_parser);
    const auto problem1 = problem_parser.get_problem();
    std::cout << *problem1 << std::endl;

    // Parse second problem where the constants are reordered
    const auto problem_parser2 = loki::ProblemParser(std::string(DATA_DIR) + "gripper/p-2-1.pddl", domain_parser);
    const auto problem2 = problem_parser2.get_problem();
    std::cout << *problem2 << std::endl;

    /* Both problems are structurally equivalent */
    assert(problem1 == problem2);

    /* Note: since the PDDL objects are shared over the whole class of problems,
             the idexing scheme is most likely fragmented per problem.
             (In this specific case, it is not fragmented because both problems are structurally equivalent) */

    return 0;
}
