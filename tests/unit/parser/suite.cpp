/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include <loki/fmt.hpp>
#include <loki/parser.hpp>

#include <yggdrasil/serialization/json.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
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

fs::path suite_root_path()
{
    return fs::path(std::string(DATA_DIR)) / "..";
}

fs::path suite_path(const boost::json::object& suite, const std::string& path)
{
    const auto prefix = ygg::common::find_string(suite, "prefix", "suite").value_or("");
    return suite_root_path() / prefix / path;
}

ParserSuiteCase parse_case(const boost::json::object& suite, const boost::json::object& object)
{
    return ParserSuiteCase { ygg::common::as_string(object, "name", "case"),
                             suite_path(suite, ygg::common::as_string(object, "domain_file", "case")),
                             suite_path(suite, ygg::common::as_string(object, "task_file", "case")) };
}

std::vector<ParserSuiteCase> load_cases()
{
    const auto suite_value = ygg::common::load_json_file(suite_root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<ParserSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_case(suite, ygg::common::as_object(case_value, "case")));
    return result;
}

TEST(LokiParserSuite, JsonSuiteCoversEveryBenchmarkProblem)
{
    const auto suite_value = ygg::common::load_json_file(suite_root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    const auto prefix = ygg::common::find_string(suite, "prefix", "suite").value_or("");
    const auto benchmark_root = suite_root_path() / prefix;

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
        const auto source = read_file(item.task_file);
        auto first = source.cbegin();
        parser::ErrorHandlerType error_handler(first, source.cend(), std::cerr);
        ast::Task ast;
        EXPECT_TRUE(parser::parse_task(source, ast, error_handler)) << item.name << ": " << item.task_file;
    }
}

TEST(LokiParserSuite, ParsesAndReparsesRepresentativeBenchmarkDomains)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests";
    const std::vector<fs::path> domains = {
        root / "classical" / "gripper" / "domain.pddl",
        root / "classical" / "miconic-fulladl" / "domain.pddl",
        root / "numeric" / "delivery" / "domain.pddl",
    };

    for (const auto& path : domains)
    {
        ASSERT_TRUE(fs::exists(path)) << path;
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

    for (const auto& path : tasks)
    {
        ASSERT_TRUE(fs::exists(path)) << path;
        expect_parse_format_reparse<ast::Task>(path, parser::parse_task, format::task);
    }
}

}
