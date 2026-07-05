/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#include "../benchmark_utils.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <loki/loki.hpp>
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

    EXPECT_EQ(domain.get_constants().size(), 2);
    EXPECT_EQ(domain.get_predicates().size(), 7);
    EXPECT_EQ(domain.get_actions().size(), 3);

    EXPECT_EQ(problem.get_objects().size(), 4);
    EXPECT_EQ(problem.get_initial_literals().size(), 11);
}

TEST(LokiTests, ParserStringTest)
{
    auto parser = loki::Parser(read_text(fixture_path("strips-minimal")));

    const auto domain = parser.get_domain();

    EXPECT_EQ(domain.get_constants().size(), 0);
    EXPECT_EQ(domain.get_predicates().size(), 1);
    EXPECT_EQ(domain.get_actions().size(), 1);
}

TEST(LokiTests, ParserNonDeterministicTest)
{
    auto parser = loki::Parser(fixture_path("alternative-effects"), loki::ParserOptions { .strict = false, .add_action_costs = false });

    const auto domain = parser.get_domain();

    EXPECT_EQ(domain.get_constants().size(), 0);
    EXPECT_EQ(domain.get_predicates().size(), 2);
    EXPECT_EQ(domain.get_actions().size(), 2);
}

TEST(LokiTests, ParserNonDeterministicMissingRequirementTest)
{
    auto options = loki::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = loki::Parser(fixture_path("missing-disjunctive-requirement"), options);
        static_cast<void>(parser.get_domain());
        FAIL() << "Expected missing requirement diagnostic";
    }
    catch (const loki::MissingRequirementError&)
    {
    }
}

}  // namespace loki::tests
