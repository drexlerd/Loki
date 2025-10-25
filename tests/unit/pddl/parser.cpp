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
#include <loki/details/pddl/domain.hpp>
#include <loki/details/pddl/exceptions.hpp>
#include <loki/details/pddl/parser.hpp>
#include <loki/details/pddl/problem.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, ParserTest)
{
    return;
    const auto domain_file = fs::path(std::string(DATA_DIR) + "gripper/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "gripper/p-2-0.pddl");
    auto parser = Parser(domain_file);
    auto domain = parser.get_domain();
    auto problem = parser.parse_problem(problem_file);

    EXPECT_EQ(domain->get_constants().size(), 2);
    EXPECT_EQ(domain->get_predicates().size(), 7);
    EXPECT_EQ(domain->get_actions().size(), 3);

    EXPECT_EQ(problem->get_objects().size(), 4);
    EXPECT_EQ(problem->get_initial_literals().size(), 11);
}

TEST(LokiTests, ParserNonDeterministicTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "blocks-non-deterministic/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "blocks-non-deterministic/p20.pddl");
    auto parser = Parser(domain_file);
    auto domain = parser.get_domain();
    auto problem = parser.parse_problem(problem_file);

    EXPECT_EQ(domain->get_constants().size(), 0);
    EXPECT_EQ(domain->get_predicates().size(), 8);
    EXPECT_EQ(domain->get_actions().size(), 7);

    EXPECT_EQ(problem->get_objects().size(), 10);
    EXPECT_EQ(problem->get_initial_literals().size(), 13);
}

TEST(LokiTests, ParserNonDeterministicMissingRequirementTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "blocks-non-deterministic/domain-missing-requirement.pddl");
    EXPECT_THROW(Parser(domain_file).get_domain(), UndefinedRequirementError);
}

}
