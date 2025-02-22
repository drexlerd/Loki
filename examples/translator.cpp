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
    auto options = loki::Options();
    options.quiet = true;
    options.strict = false;
    auto parser = loki::Parser(std::string(DATA_DIR) + "gripper/domain.pddl", options);

    const auto domain = parser.get_domain();
    std::cout << *domain << std::endl;

    const auto domain_translation_result = loki::translate(domain);

    std::cout << *domain_translation_result.get_translated_domain() << std::endl;

    const auto problem = parser.parse_problem(std::string(DATA_DIR) + "gripper/p-2-0.pddl");
    std::cout << *problem << std::endl;

    const auto problem_translation_result = loki::translate(problem, domain_translation_result);

    std::cout << *problem_translation_result.get_translated_problem() << std::endl;

    return 0;
}
