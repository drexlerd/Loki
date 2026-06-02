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
#include <loki/formalism/formatter.hpp>

#include <algorithm>
#include <filesystem>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_set>

namespace loki::tests
{
namespace fs = std::filesystem;

namespace
{

template<typename ObjectList>
std::unordered_set<std::string> object_names(ObjectList objects)
{
    auto result = std::unordered_set<std::string> {};
    for (auto object : objects)
        result.insert(std::string(object.get_name()));
    return result;
}


std::optional<size_t> generated_index(std::string_view name, std::string_view prefix)
{
    if (!name.starts_with(prefix) || name.size() == prefix.size())
        return std::nullopt;

    auto value = size_t { 0 };
    for (const auto c : name.substr(prefix.size()))
    {
        if (c < '0' || c > '9')
            return std::nullopt;
        value = value * 10 + static_cast<size_t>(c - '0');
    }
    return value;
}

size_t next_universal_index(auto predicates)
{
    auto next = size_t { 0 };
    for (auto predicate : predicates)
        if (const auto index = generated_index(std::string_view(predicate.get_name()), "_universal_"))
            next = std::max(next, *index + 1);
    return next;
}


void expect_translated_pddl_reparses(const fs::path& domain_file, const fs::path& problem_file)
{
    auto parser = loki::Parser(domain_file);

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(problem_file);

    const auto domain_translation = loki::translate(domain);
    const auto task_translation = loki::translate(task, domain_translation);

    const auto domain_text = formalism::format::domain(domain_translation.get_translated_domain());
    const auto task_text = formalism::format::task(task_translation.get_translated_task());

    auto reparsed = loki::Parser(domain_text);


    const auto reparsed_domain = reparsed.get_domain();
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain_translation.get_translated_domain().get_name());
    EXPECT_EQ(reparsed_task.get_name(), task_translation.get_translated_task().get_name());
    EXPECT_EQ(reparsed_task.get_domain().get_name(), reparsed_domain.get_name());
}

} // namespace

TEST(LokiTests, LokiPddlTranslatorReparseTest)
{
    expect_translated_pddl_reparses(fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/domain.pddl"),
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/test-1.pddl"));
    expect_translated_pddl_reparses(fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl"),
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl"));
    expect_translated_pddl_reparses(fs::path(std::string(DATA_DIR) + "planning-benchmarks/profiling/ipc2023-numeric/delivery/domain.pddl"),
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/profiling/ipc2023-numeric/delivery/pfile1.pddl"));
}


TEST(LokiTests, TaskGeneratedAxiomsDoNotReuseDomainGeneratedPredicateNames)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl");

    auto parser = loki::Parser(domain_file);
    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(problem_file);

    const auto domain_translation = loki::translate(domain);
    const auto translated_domain = domain_translation.get_translated_domain();
    const auto task_translation = loki::translate(task, domain_translation);
    const auto translated_task = task_translation.get_translated_task();

    auto domain_generated_names = std::unordered_set<std::string> {};
    for (auto predicate : translated_domain.get_predicates())
    {
        const auto name = std::string(predicate.get_name());
        if (generated_index(name, "_universal_") || generated_index(name, "_condition_") || generated_index(name, "_goal_"))
            domain_generated_names.insert(name);
    }

    auto has_task_generated_name = false;
    for (auto predicate : translated_task.get_domain().get_predicates())
    {
        const auto name = std::string(predicate.get_name());
        if ((generated_index(name, "_universal_") || generated_index(name, "_condition_") || generated_index(name, "_goal_"))
            && !domain_generated_names.contains(name))
        {
            has_task_generated_name = true;
        }
    }

    EXPECT_TRUE(has_task_generated_name);
}

TEST(LokiTests, LokiPddlTranslatorTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl");

    auto parser = loki::Parser(domain_file);


    const auto domain = parser.get_domain();
    const auto problem = parser.parse_task(problem_file);
    const auto domain_translation_result = loki::translate(domain);
    const auto translated_domain = domain_translation_result.get_translated_domain();
    const auto problem_translation_result = loki::translate(problem, domain_translation_result);
    const auto translated_problem = problem_translation_result.get_translated_task();

    {
        const auto problem_objects = object_names(translated_problem.get_objects());
        for (auto constant : translated_domain.get_constants())
        {
            EXPECT_TRUE(problem_objects.contains(std::string(constant.get_name())));
        }
    }

    {
        EXPECT_EQ(translated_problem.get_domain().get_name(), translated_domain.get_name());
        EXPECT_EQ(&translated_problem.get_domain().get_context(), &translated_domain.get_context());
    }

    {
        auto names = std::unordered_set<std::string> {};
        for (auto object : translated_problem.get_objects())
        {
            EXPECT_TRUE(names.insert(std::string(object.get_name())).second);
        }
    }

    {
        EXPECT_FALSE(translated_domain.get_actions().empty());
        EXPECT_FALSE(translated_problem.get_initial_literals().empty());
        EXPECT_TRUE(translated_problem.get_goal().has_value());
    }
}

} // namespace loki::tests
