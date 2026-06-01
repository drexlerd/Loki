/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include <loki/loki.hpp>
#include <loki/pddl/formatter.hpp>

#include <string>

namespace loki::tests
{

TEST(LokiPublicFacade, CanonicalLokiUmbrellaExposesLokiFacade)
{
    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(std::string { R"(
        (define (domain umbrella)
          (:requirements :strips)
          (:predicates (ready)))
    )" });

    EXPECT_EQ(domain.get_name(), "umbrella");
}

TEST(LokiPublicFacade, ExposesParserAndTranslatorThroughLokiNamespace)
{
    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(std::string { R"(
        (define (domain facade)
          (:requirements :strips)
          (:predicates (ready))
          (:action wait
            :parameters ()
            :precondition (ready)
            :effect (ready)))
    )" });

    const auto task = parser.parse_task(std::string { R"(
        (define (problem facade-task)
          (:domain facade)
          (:init (ready))
          (:goal (ready)))
    )" });

    const auto domain_translation = loki::translate(domain);
    const auto task_translation = loki::translate(task, domain_translation);

    EXPECT_EQ(domain_translation.get_original_domain().get_name(), domain.get_name());
    EXPECT_EQ(task_translation.get_original_task().get_name(), task.get_name());
    EXPECT_EQ(task_translation.get_translated_task().get_domain().get_name(), domain_translation.get_translated_domain().get_name());
}

TEST(LokiPublicFacade, FormatsSemanticDomainAndTaskAsReparseablePddl)
{
    auto parser = loki::Parser();
    const auto domain = parser.parse_domain(std::string { R"(
        (define (domain facade-format)
          (:requirements :strips)
          (:predicates (ready) (seen ?x))
          (:action wait
            :parameters (?x)
            :precondition (ready)
            :effect (and (ready) (seen ?x)))
        )
    )" });
    const auto task = parser.parse_task(std::string { R"(
        (define (problem facade-format-task)
          (:domain facade-format)
          (:objects item)
          (:init (ready))
          (:goal (seen item))
        )
    )" });

    const auto domain_text = loki::pddl::format::domain(domain);
    const auto task_text = loki::pddl::format::task(task);

    auto reparsed = loki::Parser();
    const auto reparsed_domain = reparsed.parse_domain(domain_text);
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain.get_name());
    EXPECT_EQ(reparsed_task.get_name(), task.get_name());
    EXPECT_EQ(reparsed_task.get_domain().get_name(), reparsed_domain.get_name());
}

} // namespace loki::tests
