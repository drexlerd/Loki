/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include "../benchmark_utils.hpp"

#include <loki/fmt.hpp>
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

template<typename Node, typename ParseFn, typename FormatFn>
void expect_parse_format_reparse(const fs::path& path, ParseFn parse, FormatFn format)
{
    const auto source = read_file(path);
    auto first = source.cbegin();
    const auto last = source.cend();
    parser::ErrorHandlerType error_handler(first, last, std::cerr);

    Node ast;
    ASSERT_TRUE(parse(source, ast, error_handler)) << path;

    const auto printed = format(ast, format::Options {});
    auto printed_first = printed.cbegin();
    const auto printed_last = printed.cend();
    parser::ErrorHandlerType printed_error_handler(printed_first, printed_last, std::cerr);

    Node reparsed;
    EXPECT_TRUE(parse(printed, reparsed, printed_error_handler)) << path << "\n" << printed;
}

struct ParserSuiteCase
{
    std::string name;
    fs::path domain_file;
    fs::path task_file;
};

ParserSuiteCase parse_case(const boost::json::object& suite, const boost::json::object& object)
{
    return ParserSuiteCase { ygg::common::as_string(object, "name", "case"),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "domain_file", "case")),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "task_file", "case")) };
}

std::vector<ParserSuiteCase> load_cases()
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<ParserSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_case(suite, ygg::common::as_object(case_value, "case")));
    return result;
}

TEST(LokiParserSuite, JsonSuiteCoversEveryBenchmarkProblem)
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    const auto benchmark_root = ygg::common::suite_prefix_path(suite);
    if (!benchmark_tree_available(benchmark_root / "tests"))
        GTEST_SKIP() << "Benchmark data unavailable: " << benchmark_root / "tests";

    auto listed = std::set<std::string> {};
    for (const auto& item : load_cases())
    {
        listed.insert(fs::relative(item.domain_file, benchmark_root).generic_string() + "|" + fs::relative(item.task_file, benchmark_root).generic_string());
    }

    auto discovered = std::set<std::string> {};
    for (const auto& entry : fs::recursive_directory_iterator(benchmark_root / "tests"))
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

    EXPECT_EQ(listed, discovered);
}

TEST(LokiParserSuite, ParsesAllBenchmarkDomainAsts)
{
    const auto cases = load_cases();
    ASSERT_FALSE(cases.empty());
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (const auto& item : cases)
    {
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
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
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().task_file;

    for (const auto& item : cases)
    {
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);
        const auto source = read_file(item.task_file);
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task ast;
        EXPECT_TRUE(parser::parse_task(source, ast, error_handler)) << item.name << ": " << item.task_file;
    }
}


TEST(LokiParserSuite, FormatterSanitizesConstructedAstIntoParseablePddl)
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
    domain.name = identifier("bad domain;name");

    auto strips = ast::Requirement {};
    strips.name = identifier("strips");
    domain.requirements.push_back(strips);

    auto item_type = ast::TypedName {};
    item_type.name = identifier("item type");
    item_type.type = type_reference("object");
    domain.types.push_back(item_type);

    auto parameter = ast::TypedVariable {};
    parameter.variable = identifier("x y");
    parameter.type = type_reference("item type");

    auto predicate = ast::PredicateDeclaration {};
    predicate.name = identifier("ready;predicate");
    predicate.parameters.push_back(parameter);
    domain.predicates.push_back(predicate);

    auto literal = ast::Literal {};
    literal.atom.predicate = identifier("ready;predicate");
    auto term = ast::Term {};
    term.name = identifier("x y");
    term.variable = true;
    literal.atom.terms.push_back(term);

    auto precondition = ast::ConditionLiteral {};
    precondition.literal = literal;

    auto number = ast::FunctionExpressionNumber {};
    number.value = std::numeric_limits<double>::infinity();
    auto number_expression = ast::FunctionExpression {};
    number_expression = number;

    auto numeric_effect = ast::EffectNumeric {};
    numeric_effect.op = "not-an-effect-op";
    numeric_effect.function.function = identifier("");
    numeric_effect.expression = number_expression;

    auto action = ast::Action {};
    action.name = identifier("act ion");
    action.parameters.push_back(parameter);
    action.precondition = ast::Condition { precondition };
    action.effect = ast::Effect { numeric_effect };
    domain.actions.push_back(action);

    const auto printed = format::domain(domain);

    EXPECT_NE(printed.find("bad_domain_name"), std::string::npos);
    EXPECT_NE(printed.find("item_type - object"), std::string::npos);
    EXPECT_NE(printed.find("?x_y - item_type"), std::string::npos);
    EXPECT_NE(printed.find("(ready_predicate ?x_y)"), std::string::npos);
    EXPECT_NE(printed.find("(assign (_) 0)"), std::string::npos);

    auto reparsed = ast::Domain {};
    auto first = printed.cbegin();
    parser::ErrorHandlerType error_handler(first, printed.cend(), std::cerr);
    EXPECT_TRUE(parser::parse_domain(printed, reparsed, error_handler)) << printed;
}

TEST(LokiParserSuite, ParsesAndReparsesRepresentativeBenchmarkDomains)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests";
    const std::vector<fs::path> domains = {
        root / "classical" / "gripper" / "domain.pddl",
        root / "classical" / "miconic-fulladl" / "domain.pddl",
        root / "numeric" / "delivery" / "domain.pddl",
    };

    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domains.front());

    for (const auto& path : domains)
    {
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(path);
        expect_parse_format_reparse<ast::Domain>(path, parser::parse_domain, format::domain);
    }
}

TEST(LokiParserSuite, ParsesAndReparsesRepresentativeBenchmarkTasks)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests";
    const std::vector<fs::path> tasks = {
        root / "classical" / "gripper" / "test-1.pddl",
        root / "classical" / "miconic-fulladl" / "test-1.pddl",
        root / "numeric" / "delivery" / "test-1.pddl",
    };

    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(tasks.front());

    for (const auto& path : tasks)
    {
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(path);
        expect_parse_format_reparse<ast::Task>(path, parser::parse_task, format::task);
    }
}

}
