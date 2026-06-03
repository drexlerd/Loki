/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include <loki/semantic.hpp>

#include <yggdrasil/serialization/json_suite.hpp>

#include <string>
#include <vector>

namespace loki::tests
{
namespace
{

struct NegativeSuiteCase
{
    std::string name;
    std::string entry;
    bool strict = false;
    std::string domain_source;
    std::string task_source;
};

std::string source_from_lines(const boost::json::object& object, const std::string& key)
{
    auto result = std::string {};
    for (const auto& line : ygg::common::as_array(object, key, "case"))
    {
        result += std::string(line.as_string());
        result += '\n';
    }
    return result;
}

NegativeSuiteCase parse_case(const boost::json::object& object)
{
    auto result = NegativeSuiteCase {};
    result.name = ygg::common::as_string(object, "name", "case");
    result.entry = ygg::common::as_string(object, "entry", "case");
    result.strict = ygg::common::find_bool(object, "strict", "case").value_or(false);
    result.domain_source = source_from_lines(object, "domain");
    if (object.if_contains("task"))
        result.task_source = source_from_lines(object, "task");
    return result;
}

std::vector<NegativeSuiteCase> load_cases()
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/semantic/negative_suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<NegativeSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_case(ygg::common::as_object(case_value, "case")));
    return result;
}

} // namespace

TEST(LokiSemanticNegativeSuite, ReportsExpectedSemanticErrors)
{
    for (const auto& item : load_cases())
    {
        SCOPED_TRACE(item.name);
        auto options = parser::ParserOptions {};
        options.strict = item.strict;
        try
        {
            auto parser = semantic::Parser(item.domain_source, options);
            if (item.entry == "task")
                parser.parse_task(item.task_source);
            FAIL() << "Expected semantic error";
        }
        catch (const semantic::SemanticError&)
        {
        }
    }
}

} // namespace loki::tests
