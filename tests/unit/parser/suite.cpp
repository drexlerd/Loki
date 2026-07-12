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

#include <gtest/gtest.h>

#include "../benchmark_utils.hpp"

#include <loki/ast.hpp>
#include <loki/parser.hpp>

#include <yggdrasil/serialization/json_suite.hpp>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace loki::tests
{
namespace fs = std::filesystem;

std::string read_file(const fs::path& path)
{
    std::ifstream in(path);
    std::ostringstream out;
    out << in.rdbuf();
    return out.str();
}

template<typename Node, typename ParseFn>
void expect_parse_format_reparse(const fs::path& path, ParseFn parse)
{
    const auto source = read_file(path);
    auto first = source.cbegin();
    const auto last = source.cend();
    parser::ErrorHandlerType error_handler(first, last, std::cerr);

    Node ast;
    ASSERT_TRUE(parse(source, ast, error_handler)) << path;

    const auto printed = fmt::format("{}", ast);
    auto printed_first = printed.cbegin();
    const auto printed_last = printed.cend();
    parser::ErrorHandlerType printed_error_handler(printed_first, printed_last, std::cerr);

    Node reparsed;
    EXPECT_TRUE(parse(printed, reparsed, printed_error_handler)) << path << "\n" << printed;
}

std::vector<SuiteCase> load_cases() { return benchmark_suite_cases(); }

TEST(LokiParserSuite, JsonSuiteCoversEveryBenchmarkProblem)
{
    const auto benchmark_root = fs::path(std::string(BENCHMARKS_DIR));
    auto listed = std::set<std::string> {};
    for (const auto& item : load_cases())
    {
        listed.insert(fs::relative(item.domain_file, benchmark_root).generic_string() + "|" + fs::relative(*item.task_file, benchmark_root).generic_string());
    }

    auto discovered = std::set<std::string> {};
    for (const auto& tree : { benchmark_root / "classical" / "tests", benchmark_root / "numeric" / "tests" })
    {
        for (const auto& entry : fs::recursive_directory_iterator(tree))
        {
            if (!entry.is_regular_file() || entry.path().filename() != "domain.pddl")
                continue;
            for (const auto& task_entry : fs::directory_iterator(entry.path().parent_path()))
            {
                if (!task_entry.is_regular_file() || task_entry.path().extension() != ".pddl" || task_entry.path().filename() == "domain.pddl")
                    continue;
                discovered.insert(fs::relative(entry.path(), benchmark_root).generic_string() + "|" + fs::relative(task_entry.path(), benchmark_root).generic_string());
            }
        }
    }

    EXPECT_EQ(listed, discovered);
}

TEST(LokiParserSuite, ParsesAllBenchmarkDomainAsts)
{
    const auto cases = load_cases();
    ASSERT_FALSE(cases.empty());
    for (const auto& item : cases)
    {
        const auto source = read_file(item.domain_file);
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Domain ast;
        EXPECT_TRUE(parser::parse_domain(source, ast, error_handler)) << item.name << ": " << item.domain_file;
    }
}

TEST(LokiParserSuite, ParsesAllBenchmarkTaskAsts)
{
    const auto cases = load_cases();
    ASSERT_FALSE(cases.empty());
    for (const auto& item : cases)
    {
        const auto source = read_file(*item.task_file);
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task ast;
        EXPECT_TRUE(parser::parse_task(source, ast, error_handler)) << item.name << ": " << *item.task_file;
    }
}

TEST(LokiParserSuite, FormatterPrintsConstructedAstAsParseablePddl)
{
    const auto identifier = [](std::string text)
    {
        auto result = ast::Identifier {};
        result.text = std::move(text);
        return result;
    };
    const auto type_reference = [&](std::string name)
    {
        auto result = ast::TypeReference {};
        result.name = identifier(std::move(name));
        return ast::TypeExpression { result };
    };

    auto domain = ast::Domain {};
    domain.name = identifier("valid-domain");

    auto strips = ast::Requirement {};
    strips.name = identifier("strips");
    domain.requirements.push_back(strips);

    auto item_type = ast::TypedName {};
    item_type.name = identifier("item-type");
    item_type.type = type_reference("object");
    domain.types.push_back(item_type);

    auto parameter = ast::TypedVariable {};
    parameter.variable = identifier("?x");
    parameter.type = type_reference("item-type");

    auto predicate = ast::PredicateDeclaration {};
    predicate.name = identifier("ready-predicate");
    predicate.parameters.push_back(parameter);
    domain.predicates.push_back(predicate);

    auto literal = ast::Literal {};
    literal.atom.predicate = identifier("ready-predicate");
    auto term = ast::Term {};
    term.name = identifier("?x");
    term.variable = true;
    literal.atom.terms.push_back(term);

    auto precondition = ast::ConditionLiteral {};
    precondition.literal = literal;

    auto number = ast::FunctionExpressionNumber {};
    number.value = 0.0;
    auto number_expression = ast::FunctionExpression {};
    number_expression = number;

    auto numeric_effect = ast::EffectNumeric {};
    numeric_effect.op = "assign";
    numeric_effect.function.function = identifier("total-cost");
    numeric_effect.expression = number_expression;

    auto action = ast::Action {};
    action.name = identifier("act");
    action.parameters.push_back(parameter);
    action.precondition = ast::Condition { precondition };
    action.effect = ast::Effect { numeric_effect };
    domain.actions.push_back(action);

    const auto printed = fmt::format("{}", domain);

    EXPECT_NE(printed.find("valid-domain"), std::string::npos);
    EXPECT_NE(printed.find("item-type - object"), std::string::npos);
    EXPECT_NE(printed.find("?x - item-type"), std::string::npos);
    EXPECT_NE(printed.find("(ready-predicate ?x)"), std::string::npos);
    EXPECT_NE(printed.find("(assign (total-cost) 0)"), std::string::npos);

    auto reparsed = ast::Domain {};
    auto first = printed.cbegin();
    parser::ErrorHandlerType error_handler(first, printed.cend(), std::cerr);
    EXPECT_TRUE(parser::parse_domain(printed, reparsed, error_handler)) << printed;
}

TEST(LokiParserSuite, ParsesAndReparsesRepresentativeBenchmarkDomains)
{
    const auto root = fs::path(std::string(BENCHMARKS_DIR));
    const std::vector<fs::path> domains = {
        root / "classical" / "tests" / "gripper" / "domain.pddl",
        root / "classical" / "tests" / "miconic-fulladl" / "domain.pddl",
        root / "numeric" / "tests" / "delivery" / "domain.pddl",
    };

    for (const auto& path : domains)
    {
        expect_parse_format_reparse<ast::Domain>(path, parser::parse_domain);
    }
}

TEST(LokiParserSuite, ParsesAndReparsesRepresentativeBenchmarkTasks)
{
    const auto root = fs::path(std::string(BENCHMARKS_DIR));
    const std::vector<fs::path> tasks = {
        root / "classical" / "tests" / "gripper" / "test-1.pddl",
        root / "classical" / "tests" / "miconic-fulladl" / "test-1.pddl",
        root / "numeric" / "tests" / "delivery" / "test-1.pddl",
    };

    for (const auto& path : tasks)
    {
        expect_parse_format_reparse<ast::Task>(path, parser::parse_task);
    }
}

}
