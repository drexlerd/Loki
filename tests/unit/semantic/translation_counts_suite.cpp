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
#include <loki/semantic.hpp>
#include <string>
#include <vector>
#include <yggdrasil/serialization/json_suite.hpp>

namespace loki::tests
{
namespace fs = std::filesystem;
namespace
{

struct ParserSuiteCase
{
    std::string name;
    fs::path domain_file;
    fs::path task_file;
};

struct TranslationCountExpectation
{
    std::string name;
    std::size_t domain_predicates;
    std::size_t domain_actions;
    std::size_t domain_axioms;
    std::size_t task_predicates;
    std::size_t task_actions;
    std::size_t task_axioms;
};

ParserSuiteCase parse_parser_case(const boost::json::object& suite, const boost::json::object& object)
{
    return ParserSuiteCase { ygg::common::as_string(object, "name", "case"),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "domain_file", "case")),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "task_file", "case")) };
}

std::vector<ParserSuiteCase> load_parser_cases()
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<ParserSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_parser_case(suite, ygg::common::as_object(case_value, "case")));
    return result;
}

TranslationCountExpectation parse_expectation(const boost::json::object& object)
{
    const auto& domain = ygg::common::as_object(object, "translated_domain", "case");
    const auto& task = ygg::common::as_object(object, "translated_task", "case");
    return TranslationCountExpectation { ygg::common::as_string(object, "name", "case"),
                                         ygg::common::as_size(domain, "predicates", "case.translated_domain"),
                                         ygg::common::as_size(domain, "actions", "case.translated_domain"),
                                         ygg::common::as_size(domain, "axioms", "case.translated_domain"),
                                         ygg::common::as_size(task, "predicates", "case.translated_task"),
                                         ygg::common::as_size(task, "actions", "case.translated_task"),
                                         ygg::common::as_size(task, "axioms", "case.translated_task") };
}

std::vector<TranslationCountExpectation> load_expectations()
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/semantic/translation_counts_suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<TranslationCountExpectation> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_expectation(ygg::common::as_object(case_value, "case")));
    return result;
}

}  // namespace

TEST(LokiSemanticTranslationCountsSuite, TranslatedBenchmarkCountsStayStable)
{
    const auto cases = load_parser_cases();
    const auto expectations = load_expectations();
    ASSERT_EQ(cases.size(), expectations.size());
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (auto i = std::size_t { 0 }; i < cases.size(); ++i)
    {
        const auto& item = cases[i];
        const auto& expected = expectations[i];
        SCOPED_TRACE(item.name);
        ASSERT_EQ(item.name, expected.name);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);

        auto parser = semantic::Parser(item.domain_file);
        // Expected counts were recorded with the full normalization pipeline.
        const auto options = semantic::TranslatorOptions { .compile_typing = true, .materialize_equality = true };
        const auto domain_translation = semantic::translate(parser.get_domain(), options);
        const auto translated_domain = domain_translation.get_translated_domain();
        const auto task_translation = semantic::translate(parser.parse_task(item.task_file), domain_translation, options);
        const auto translated_task = task_translation.get_translated_task();

        EXPECT_EQ(translated_domain.get_predicates().size(), expected.domain_predicates);
        EXPECT_EQ(translated_domain.get_actions().size(), expected.domain_actions);
        EXPECT_EQ(translated_domain.get_axioms().size(), expected.domain_axioms);
        EXPECT_EQ(translated_task.get_predicates().size(), expected.task_predicates);
        EXPECT_EQ(translated_task.get_domain().get_actions().size(), expected.task_actions);
        EXPECT_EQ(translated_task.get_axioms().size(), expected.task_axioms);
    }
}

}  // namespace loki::tests
