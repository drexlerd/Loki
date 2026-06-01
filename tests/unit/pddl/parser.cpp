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

#include <loki/loki.hpp>

#include <filesystem>
#include <string>

namespace loki::tests
{
namespace fs = std::filesystem;

TEST(LokiTests, ParserTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/test-1.pddl");
    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(domain_file);
    const auto problem = parser.parse_task(problem_file);

    EXPECT_EQ(domain.get_constants().size(), 2);
    EXPECT_EQ(domain.get_predicates().size(), 7);
    EXPECT_EQ(domain.get_actions().size(), 3);

    EXPECT_EQ(problem.get_objects().size(), 4);
    EXPECT_EQ(problem.get_initial_literals().size(), 11);
}

TEST(LokiTests, ParserStringTest)
{
    const std::string domain_str =
        "(define (domain test-domain) "
        "        (:requirements :strips) "
        "        (:predicates (p)) "
        "        (:action a "
        "         :parameters () "
        "         :precondition (and) "
        "         :effect (and (p))))";

    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(domain_str);

    EXPECT_EQ(domain.get_constants().size(), 0);
    EXPECT_EQ(domain.get_predicates().size(), 1);
    EXPECT_EQ(domain.get_actions().size(), 1);
}

TEST(LokiTests, ParserNonDeterministicTest)
{
    const auto domain_str = std::string { R"(
(define (domain nondet)
  (:requirements :strips :non-deterministic)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :effect (oneof (p) (q)))
)
)" };

    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(domain_str);

    EXPECT_EQ(domain.get_constants().size(), 0);
    EXPECT_EQ(domain.get_predicates().size(), 2);
    EXPECT_EQ(domain.get_actions().size(), 1);
}

TEST(LokiTests, ParserNonDeterministicMissingRequirementTest)
{
    const auto domain_str = std::string { R"(
(define (domain missing-requirement)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
)" };

    auto options = loki::ParserOptions {};
    options.strict = true;
    auto parser = loki::Parser(options);

    try
    {
        static_cast<void>(parser.parse_domain(domain_str));
        FAIL() << "Expected missing requirement diagnostic";
    }
    catch (const loki::SemanticError& error)
    {
        EXPECT_EQ(error.code(), loki::SemanticErrorCode::MissingRequirement);
        ASSERT_TRUE(error.source_range().has_value());
        EXPECT_EQ(error.source_range()->begin.line, 6);
        EXPECT_EQ(error.source_range()->begin.column, 19);
    }
}

} // namespace loki::tests
