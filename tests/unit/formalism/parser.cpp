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

#include "../benchmark_utils.hpp"

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
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_file);
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(problem_file);

    auto parser = loki::Parser(domain_file);

    const auto domain = parser.get_domain();
    const auto problem = parser.parse_task(problem_file);

    EXPECT_EQ(domain.get_num_constants(), 2);
    EXPECT_EQ(domain.get_num_predicates(), 7);
    EXPECT_EQ(domain.get_num_actions(), 3);

    EXPECT_EQ(problem.get_num_objects(), 4);
    EXPECT_EQ(problem.get_num_initial_literals(), 11);
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

    auto parser = loki::Parser(domain_str);


    const auto domain = parser.get_domain();

    EXPECT_EQ(domain.get_num_constants(), 0);
    EXPECT_EQ(domain.get_num_predicates(), 1);
    EXPECT_EQ(domain.get_num_actions(), 1);
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

    auto parser = loki::Parser(domain_str);


    const auto domain = parser.get_domain();

    EXPECT_EQ(domain.get_num_constants(), 0);
    EXPECT_EQ(domain.get_num_predicates(), 2);
    EXPECT_EQ(domain.get_num_actions(), 1);
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

    try
    {
        auto parser = loki::Parser(domain_str, options);
        static_cast<void>(parser.get_domain());
        FAIL() << "Expected missing requirement diagnostic";
    }
    catch (const loki::MissingRequirementError&)
    {
    }
}

} // namespace loki::tests
