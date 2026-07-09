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

#include "benchmark_utils.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <loki/ast.hpp>
#include <loki/ast/ast.hpp>
#include <loki/ast/ast_fwd.hpp>
#include <loki/formalism/builder.hpp>
#include <loki/formalism/canonicalization.hpp>
#include <loki/formalism/datas.hpp>
#include <loki/formalism/enums.hpp>
#include <loki/formalism/formalism.hpp>
#include <loki/formalism/formatter.hpp>
#include <loki/formalism/indices.hpp>
#include <loki/formalism/repository.hpp>
#include <loki/formalism/views.hpp>
#include <loki/loki.hpp>
#include <loki/parser.hpp>
#include <loki/parser/error_handler.hpp>
#include <loki/parser/parser.hpp>
#include <loki/semantic.hpp>
#include <loki/semantic/errors.hpp>
#include <loki/semantic/parser.hpp>
#include <loki/semantic/translator.hpp>
#include <string>

namespace loki::tests
{

TEST(LokiPublicHeaders, FormalismBuilderHeaderIsSelfContained)
{
    auto factory = formalism::RepositoryFactory {};
    auto repository = factory.create();

    const auto requirement = formalism::get_or_create<formalism::Requirement>(repository, formalism::RequirementKind::Strips);
    const auto number = formalism::get_or_create<formalism::FunctionExpressionNumber>(repository, 1.0);
    const auto expression =
        formalism::get_or_create<formalism::FunctionExpression>(repository,
                                                                ygg::Data<formalism::FunctionExpression>::ViewVariant<formalism::Repository> { number });

    EXPECT_EQ(requirement.get_kind(), formalism::RequirementKind::Strips);
    static_cast<void>(expression.get_variant());
}

TEST(LokiPublicHeaders, ParserAndFormatterHeadersAreSelfContained)
{
    const auto domain_source = read_text(fixture_path("facade"));
    auto first = domain_source.cbegin();
    auto error_handler = parser::ErrorHandlerType(first, domain_source.cend(), std::cerr);
    auto ast_domain = ast::Domain {};

    ASSERT_TRUE(parser::parse_domain(domain_source, ast_domain, error_handler));
    EXPECT_EQ(fmt::format("{}", ast_domain).find("facade") != std::string::npos, true);
}

TEST(LokiPublicHeaders, LokiUmbrellaHeaderExposesFacadeHelpers)
{
    auto parser = loki::Parser(read_text(fixture_path("facade")));
    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(read_text(fixture_path("facade", "task.pddl")));

    const auto domain_translation = loki::translate_domain(domain);
    const auto task_translation = loki::translate_task(task, domain_translation);
    const auto translated_domain = domain_translation.get_translated_domain();
    const auto translated_task = task_translation.get_translated_task();
    const auto domain_text = loki::format_domain(translated_domain);
    const auto task_text = loki::format_task(translated_task);

    EXPECT_NE(domain_text.find("facade"), std::string::npos);
    EXPECT_NE(task_text.find("facade-task"), std::string::npos);
    EXPECT_EQ(fmt::format("{}", translated_domain), domain_text);
    EXPECT_EQ(fmt::format("{}", translated_task), task_text);
}

TEST(LokiPublicHeaders, SemanticHeaderIsSelfContained)
{
    auto parser = semantic::Parser(read_text(fixture_path("facade")));

    EXPECT_EQ(parser.get_domain().get_name(), "facade");
}

}  // namespace loki::tests
