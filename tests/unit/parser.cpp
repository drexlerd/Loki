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

#include <gtest/gtest.h>
#include <loki/details/parser.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, ParserTest)
{
    return;
    const auto domain_file = fs::path(std::string(DATA_DIR) + "gripper/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "gripper/p-2-0.pddl");
    auto domain_parser = DomainParser(domain_file);
    auto problem_parser = ProblemParser(problem_file, domain_parser);

    const auto domain = domain_parser.get_domain();
    EXPECT_EQ(domain->get_constants().size(), 2);
    EXPECT_EQ(domain->get_predicates().size(), 7);
    EXPECT_EQ(domain->get_actions().size(), 3);

    const auto problem = problem_parser.get_problem();
    EXPECT_EQ(problem->get_objects().size(), 4);
    EXPECT_EQ(problem->get_initial_literals().size(), 11);
}

TEST(LokiTests, ParserNonDeterministicTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "blocks-non-deterministic/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "blocks-non-deterministic/p003-1.pddl");
    auto domain_parser = DomainParser(domain_file);
    auto problem_parser = ProblemParser(problem_file, domain_parser);

    const auto domain = domain_parser.get_domain();
    EXPECT_EQ(domain->get_constants().size(), 0);
    EXPECT_EQ(domain->get_predicates().size(), 3);
    EXPECT_EQ(domain->get_actions().size(), 3);

    const auto problem = problem_parser.get_problem();
    EXPECT_EQ(problem->get_objects().size(), 3);
    EXPECT_EQ(problem->get_initial_literals().size(), 4);
}

}
