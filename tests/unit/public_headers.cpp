/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>
#include <iostream>
#include <loki/ast.hpp>
#include <loki/ast/ast.hpp>
#include <loki/ast/ast_fwd.hpp>
#include <loki/fmt.hpp>
#include <loki/fmt/formatter.hpp>
#include <loki/formalism/builder.hpp>
#include <loki/formalism/canonicalization.hpp>
#include <loki/formalism/datas.hpp>
#include <loki/formalism/formalism.hpp>
#include <loki/formalism/formatter.hpp>
#include <loki/formalism/indices.hpp>
#include <loki/formalism/repository.hpp>
#include <loki/formalism/views.hpp>
#include <loki/loki.hpp>
#include <loki/parser.hpp>
#include <loki/parser/error_handler.hpp>
#include <loki/parser/options.hpp>
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
    const auto domain_source = std::string { "(define (domain direct) (:predicates (p)))" };
    auto first = domain_source.cbegin();
    auto error_handler = parser::ErrorHandlerType(first, domain_source.cend(), std::cerr);
    auto ast_domain = ast::Domain {};

    ASSERT_TRUE(parser::parse_domain(domain_source, ast_domain, error_handler));
    EXPECT_EQ(format::domain(ast_domain).find("direct") != std::string::npos, true);
}

TEST(LokiPublicHeaders, LokiUmbrellaHeaderExposesFacadeHelpers)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain umbrella-header)
          (:predicates (p))
        )
    )" });
    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(std::string { R"(
        (define (problem umbrella-header-task)
          (:domain umbrella-header)
          (:init)
          (:goal (p))
        )
    )" });

    const auto domain_translation = loki::translate_domain(domain);
    const auto task_translation = loki::translate_task(task, domain_translation);
    const auto domain_text = loki::format_domain(domain_translation.get_translated_domain());
    const auto task_text = loki::format_task(task_translation.get_translated_task());

    EXPECT_NE(domain_text.find("umbrella-header"), std::string::npos);
    EXPECT_NE(task_text.find("umbrella-header-task"), std::string::npos);
}

TEST(LokiPublicHeaders, SemanticHeaderIsSelfContained)
{
    auto parser = semantic::Parser(std::string { "(define (domain semantic-direct) (:predicates (p)))" });

    EXPECT_EQ(parser.get_domain().get_name(), "semantic-direct");
}

}  // namespace loki::tests
