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
#include <fstream>
#include <gtest/gtest.h>
#include <loki/loki.hpp>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
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

bool has_predicate_named(formalism::DomainView domain, std::string_view name)
{
    for (auto predicate : domain.get_predicates())
        if (std::string_view(predicate.get_name()) == name)
            return true;
    return false;
}

bool has_function_named(formalism::DomainView domain, std::string_view name)
{
    for (auto function : domain.get_functions())
        if (std::string_view(function.get_name()) == name)
            return true;
    return false;
}

bool file_mentions(const fs::path& path, std::string_view needle)
{
    auto in = std::ifstream(path);
    auto text = std::ostringstream {};
    text << in.rdbuf();
    return text.str().find(needle) != std::string::npos;
}

std::size_t count_equality_init_literals(formalism::TaskView task)
{
    auto count = std::size_t { 0 };
    for (auto literal : task.get_initial_literals())
        if (std::string_view(literal.get_atom().get_predicate().get_name()) == "=")
            ++count;
    return count;
}

std::size_t count_unique_object_names(formalism::DomainView domain, formalism::TaskView task)
{
    auto names = std::set<std::string> {};
    for (auto object : domain.get_constants())
        names.insert(std::string(object.get_name()));
    for (auto object : task.get_objects())
        names.insert(std::string(object.get_name()));
    return names.size();
}

// Conjunction of literals and numeric constraints; what grounding-based consumers require.
bool is_conjunctive(formalism::ConditionView condition)
{
    return ygg::visit(
        [&](const auto& node) -> bool
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionAndView>)
            {
                for (auto child : node.get_conditions())
                    if (!is_conjunctive(child))
                        return false;
                return true;
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionLiteralView> || std::is_same_v<Node, formalism::ConditionNumericConstraintView>)
                return true;
            else
                return false;
        },
        condition.get_value());
}

bool writes_function_named(formalism::EffectView effect, std::string_view name)
{
    return ygg::visit(
        [&](const auto& node) -> bool
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::EffectNumericView>)
                return std::string_view(node.get_function().get_name()) == name;
            else if constexpr (std::is_same_v<Node, formalism::EffectAndView> || std::is_same_v<Node, formalism::EffectOneOfView>)
            {
                for (auto child : node.get_effects())
                    if (writes_function_named(child, name))
                        return true;
                return false;
            }
            else if constexpr (std::is_same_v<Node, formalism::EffectForallView> || std::is_same_v<Node, formalism::EffectWhenView>)
                return writes_function_named(node.get_effect(), name);
            else if constexpr (std::is_same_v<Node, formalism::EffectProbabilisticView>)
            {
                for (auto alternative : node.get_alternatives())
                    if (writes_function_named(alternative.get_effect(), name))
                        return true;
                return false;
            }
            else
                return false;
        },
        effect.get_value());
}

}  // namespace

TEST(LokiSemanticOptionMatrixSuite, TranslatorOptionCombinationsKeepInvariants)
{
    const auto cases = load_parser_cases();
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);

        auto parser = semantic::Parser(item.domain_file);
        const auto task = parser.parse_task(item.task_file);

        for (const auto compile_typing : { false, true })
        {
            for (const auto compile_conditional_effects : { false, true })
            {
                for (const auto materialize_equality : { false, true })
                {
                    SCOPED_TRACE("compile_typing=" + std::to_string(compile_typing) + " compile_conditional_effects="
                                 + std::to_string(compile_conditional_effects) + " materialize_equality=" + std::to_string(materialize_equality));

                    auto options = semantic::TranslatorOptions {};
                    options.compile_typing = compile_typing;
                    options.compile_conditional_effects = compile_conditional_effects;
                    options.materialize_equality = materialize_equality;

                    const auto domain_translation = semantic::translate(parser.get_domain(), options);
                    const auto translated_domain = domain_translation.get_translated_domain();
                    const auto task_translation = semantic::translate(task, domain_translation, options);
                    const auto translated_task = task_translation.get_translated_task();

                    if (compile_typing)
                    {
                        EXPECT_TRUE(translated_domain.get_types().empty());
                        for (auto action : translated_domain.get_actions())
                            for (auto parameter : action.get_parameters())
                                EXPECT_TRUE(parameter.get_types().empty());
                    }
                    else
                    {
                        EXPECT_FALSE(translated_domain.get_types().empty());
                    }

                    if (compile_conditional_effects)
                    {
                        // Multiplied-out actions must expose plain conjunctive preconditions.
                        for (auto action : translated_domain.get_actions())
                            if (const auto precondition = action.get_precondition())
                                EXPECT_TRUE(is_conjunctive(precondition.value()));
                    }

                    if (materialize_equality)
                    {
                        if (has_predicate_named(translated_domain, "="))
                            EXPECT_EQ(count_equality_init_literals(translated_task), count_unique_object_names(translated_domain, translated_task));
                    }
                    else
                    {
                        EXPECT_EQ(count_equality_init_literals(translated_task), 0);
                    }

                    // Formatted output must reparse for every option combination.
                    const auto domain_text = loki::format_domain(translated_domain);
                    const auto task_text = loki::format_task(translated_task);
                    auto reparsed = semantic::Parser(domain_text);
                    const auto reparsed_task = reparsed.parse_task(task_text);
                    EXPECT_EQ(reparsed.get_domain().get_name(), translated_domain.get_name());
                    EXPECT_EQ(reparsed_task.get_name(), translated_task.get_name());
                }
            }
        }
    }
}

TEST(LokiSemanticOptionMatrixSuite, AddActionCostsOptionHoldsAcrossFixtures)
{
    const auto cases = load_parser_cases();
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);

        auto options = parser::ParserOptions {};
        options.add_action_costs = true;
        auto parser = semantic::Parser(item.domain_file, options);
        const auto domain = parser.get_domain();
        const auto task = parser.parse_task(item.task_file);

        EXPECT_TRUE(has_function_named(domain, "total-cost"));
        // Unit-cost injection only happens when the domain did not declare :action-costs itself;
        // otherwise the option merely completes missing artifacts and leaves actions alone.
        if (!file_mentions(item.domain_file, ":action-costs"))
        {
            for (auto action : domain.get_actions())
            {
                SCOPED_TRACE(std::string(action.get_name()));
                ASSERT_TRUE(action.get_effect().has_value());
                EXPECT_TRUE(writes_function_named(action.get_effect().value(), "total-cost"));
            }
        }

        ASSERT_TRUE(task.get_metric().has_value());
        EXPECT_TRUE(task.get_metric().value().is_minimize());
        auto has_total_cost_init = false;
        for (auto value : task.get_initial_function_values())
            has_total_cost_init = has_total_cost_init || std::string_view(value.get_function().get_function().get_name()) == "total-cost";
        EXPECT_TRUE(has_total_cost_init);

        // The completed output must still reparse.
        auto reparsed = semantic::Parser(loki::format_domain(domain));
        const auto reparsed_task = reparsed.parse_task(loki::format_task(task));
        EXPECT_EQ(reparsed_task.get_name(), task.get_name());
    }
}

}  // namespace loki::tests
