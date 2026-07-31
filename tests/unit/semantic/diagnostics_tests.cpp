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

#include <gtest/gtest.h>
#include <loki/formalism/task_view.hpp>
#include <loki/semantic/errors.hpp>
#include <loki/semantic/parser.hpp>
#include <string>

namespace loki::tests
{

TEST(LokiSemanticDiagnostics, ReportsSyntaxFailureMessage)
{
    try
    {
        auto parser = semantic::Parser(read_text(fixture_path("broken-syntax")));
        FAIL() << "Expected parse error";
    }
    catch (const semantic::ParseError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("Could not parse PDDL domain"), std::string::npos);
        EXPECT_NE(message.find("In line 1:"), std::string::npos);
        EXPECT_NE(message.find("^_"), std::string::npos);
    }
}

TEST(LokiSemanticDiagnostics, IncludesFileNameForPathDiagnostics)
{
    const auto domain_path = fixture_path("broken-syntax");
    try
    {
        auto parser = semantic::Parser(domain_path);
        FAIL() << "Expected parse error";
    }
    catch (const semantic::ParseError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("Could not parse PDDL domain"), std::string::npos);
        EXPECT_NE(message.find("In file " + domain_path.string() + ", line 1:"), std::string::npos);
    }

    const auto task_path = fixture_path("repo-mismatch", "task.pddl");
    try
    {
        auto parser = semantic::Parser(fixture_path("facade"));
        parser.parse_task(task_path);
        FAIL() << "Expected mismatched domain error";
    }
    catch (const semantic::MismatchedDomainError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("Task references domain 'second-translation-domain'"), std::string::npos);
        EXPECT_NE(message.find("In file " + task_path.string() + ", line 2:"), std::string::npos);
    }
}

}  // namespace loki::tests
