/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>
#include <loki/formalism/formatter.hpp>
#include <loki/loki.hpp>
#include <string>
#include <type_traits>

namespace loki::tests
{

TEST(LokiPublicFacade, CanonicalLokiUmbrellaExposesLokiFacade)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain umbrella)
          (:requirements :strips)
          (:predicates (ready)))
    )" });

    const auto domain = parser.get_domain();

    EXPECT_EQ(domain.get_name(), "umbrella");
}

TEST(LokiPublicFacade, ExposesParserAndTranslatorThroughLokiNamespace)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain facade)
          (:requirements :strips)
          (:predicates (ready))
          (:action wait
            :parameters ()
            :precondition (ready)
            :effect (ready)))
    )" });

    const auto domain = parser.get_domain();

    const auto task = parser.parse_task(std::string { R"(
        (define (problem facade-task)
          (:domain facade)
          (:init (ready))
          (:goal (ready)))
    )" });

    const auto domain_translation = loki::translate(domain);
    const auto task_translation = loki::translate(task, domain_translation);
    const auto named_domain_translation = loki::translate_domain(domain);
    const auto named_task_translation = loki::translate_task(task, named_domain_translation);

    EXPECT_EQ(domain.get_num_requirements(), 1);
    EXPECT_EQ(domain.get_num_predicates(), 1);
    EXPECT_EQ(domain.get_num_actions(), 1);

    const auto action = domain.get_actions()[0];
    EXPECT_EQ(action.get_num_parameters(), 0);
    EXPECT_TRUE(action.has_precondition());
    EXPECT_TRUE(action.has_effect());

    EXPECT_EQ(task.get_num_initial_literals(), 1);
    EXPECT_TRUE(task.has_goal());
    EXPECT_FALSE(task.has_metric());

    EXPECT_EQ(domain_translation.get_original_domain().get_name(), domain.get_name());
    EXPECT_EQ(task_translation.get_original_task().get_name(), task.get_name());
    EXPECT_EQ(task_translation.get_translated_task().get_domain().get_name(), domain_translation.get_translated_domain().get_name());
    EXPECT_EQ(named_domain_translation.get_original_domain().get_name(), domain.get_name());
    EXPECT_EQ(named_task_translation.get_original_task().get_name(), task.get_name());
    EXPECT_EQ(named_task_translation.get_translated_task().get_domain().get_name(), named_domain_translation.get_translated_domain().get_name());
}

TEST(LokiPublicFacade, ExposesSemanticErrorsThroughLokiNamespace)
{
    static_assert(std::is_same_v<loki::SourcePosition, loki::semantic::SourcePosition>);
    static_assert(std::is_same_v<loki::SourceRange, loki::semantic::SourceRange>);

    const auto position = loki::SourcePosition { .line = 2, .column = 3, .offset = 4 };
    EXPECT_EQ(position, (loki::SourcePosition { .line = 2, .column = 3, .offset = 4 }));
    EXPECT_NE(position, (loki::SourcePosition { .line = 2, .column = 4, .offset = 4 }));

    const auto range = loki::SourceRange { position, loki::SourcePosition { .line = 2, .column = 8, .offset = 9 } };
    EXPECT_EQ(range, (loki::SourceRange { position, loki::SourcePosition { .line = 2, .column = 8, .offset = 9 } }));
    EXPECT_NE(range, (loki::SourceRange { position, position }));
    static_assert(std::is_same_v<loki::SemanticError, loki::semantic::SemanticError>);
    static_assert(std::is_same_v<loki::ParseError, loki::semantic::ParseError>);
    static_assert(std::is_same_v<loki::UnsupportedRequirementError, loki::semantic::UnsupportedRequirementError>);
    static_assert(std::is_same_v<loki::MissingRequirementError, loki::semantic::MissingRequirementError>);
    static_assert(std::is_same_v<loki::UndefinedTypeError, loki::semantic::UndefinedTypeError>);
    static_assert(std::is_same_v<loki::UndefinedPredicateError, loki::semantic::UndefinedPredicateError>);
    static_assert(std::is_same_v<loki::UndefinedObjectError, loki::semantic::UndefinedObjectError>);
    static_assert(std::is_same_v<loki::UndefinedVariableError, loki::semantic::UndefinedVariableError>);
    static_assert(std::is_same_v<loki::UndefinedFunctionError, loki::semantic::UndefinedFunctionError>);
    static_assert(std::is_same_v<loki::DuplicateDefinitionError, loki::semantic::DuplicateDefinitionError>);
    static_assert(std::is_same_v<loki::DuplicateTypeError, loki::semantic::DuplicateTypeError>);
    static_assert(std::is_same_v<loki::DuplicatePredicateError, loki::semantic::DuplicatePredicateError>);
    static_assert(std::is_same_v<loki::DuplicateObjectError, loki::semantic::DuplicateObjectError>);
    static_assert(std::is_same_v<loki::DuplicateVariableError, loki::semantic::DuplicateVariableError>);
    static_assert(std::is_same_v<loki::DuplicateFunctionError, loki::semantic::DuplicateFunctionError>);
    static_assert(std::is_same_v<loki::ArityMismatchError, loki::semantic::ArityMismatchError>);
    static_assert(std::is_same_v<loki::TypeMismatchError, loki::semantic::TypeMismatchError>);
    static_assert(std::is_same_v<loki::InvalidMetricError, loki::semantic::InvalidMetricError>);
    static_assert(std::is_same_v<loki::InvalidNumericConstraintError, loki::semantic::InvalidNumericConstraintError>);
    static_assert(std::is_same_v<loki::InvalidNumericEffectError, loki::semantic::InvalidNumericEffectError>);
    static_assert(std::is_same_v<loki::InvalidProbabilisticEffectError, loki::semantic::InvalidProbabilisticEffectError>);
    static_assert(std::is_same_v<loki::InvalidEqualityError, loki::semantic::InvalidEqualityError>);
    static_assert(std::is_same_v<loki::MismatchedDomainError, loki::semantic::MismatchedDomainError>);
    static_assert(std::is_same_v<loki::MissingDomainError, loki::semantic::MissingDomainError>);

    auto error = loki::SemanticError("base diagnostic");
    EXPECT_FALSE(error.has_source_range());
    EXPECT_FALSE(error.source_range().has_value());
    EXPECT_EQ(std::string(error.what()), "base diagnostic");

    error.set_source_range(range);
    ASSERT_TRUE(error.has_source_range());
    ASSERT_TRUE(error.source_range().has_value());
    EXPECT_EQ(*error.source_range(), range);
    EXPECT_EQ(std::string(error.what()), "base diagnostic at line 2, column 3");

    try
    {
        static_cast<void>(loki::Parser(std::string { "(define (domain broken) (:predicates (p))" }));
        FAIL() << "Expected loki::ParseError through the public facade.";
    }
    catch (const loki::ParseError& error)
    {
        EXPECT_NE(std::string(error.what()).find("line 1, column"), std::string::npos);
        ASSERT_TRUE(error.has_source_range());
        ASSERT_TRUE(error.source_range().has_value());
        EXPECT_EQ(error.source_range()->begin.line, 1);
        EXPECT_EQ(error.source_range()->begin.line, error.source_range()->end.line);
        EXPECT_EQ(error.source_range()->begin.column, error.source_range()->end.column);
    }

    auto options = loki::ParserOptions {};
    options.strict = true;
    try
    {
        static_cast<void>(loki::Parser(std::string { R"(
            (define (domain missing-requirement)
              (:predicates (p) (q))
              (:action a
                :parameters ()
                :precondition (or (p) (q))
                :effect (and)))
        )" },
                                       options));
    }
    catch (const loki::MissingRequirementError& error)
    {
        EXPECT_NE(std::string(error.what()).find(":disjunctive-preconditions"), std::string::npos);
        EXPECT_NE(std::string(error.what()).find("line 6, column 31"), std::string::npos);
        ASSERT_TRUE(error.has_source_range());
        ASSERT_TRUE(error.source_range().has_value());
        const loki::SourceRange source_range = *error.source_range();
        EXPECT_EQ(source_range.begin.line, 6);
        EXPECT_EQ(source_range.begin.column, 31);
    }
    catch (...)
    {
        FAIL() << "Expected loki::MissingRequirementError through the public facade.";
    }
}

TEST(LokiPublicFacade, ViewConvenienceMethodsCoverTypedSymbolsAndLiterals)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain typed-facade)
          (:requirements :strips :typing)
          (:types base - object item - base)
          (:predicates (ready ?x - item))
          (:action mark
            :parameters (?x - item)
            :precondition (ready ?x)
            :effect (ready ?x)))
    )" });

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(std::string { R"(
        (define (problem typed-facade-task)
          (:domain typed-facade)
          (:objects item-1 - item)
          (:init (ready item-1))
          (:goal (ready item-1)))
    )" });

    ASSERT_GE(domain.get_num_types(), 2);
    auto saw_item_type = false;
    for (const auto type : domain.get_types())
    {
        if (type.get_name() == "item")
        {
            saw_item_type = true;
            EXPECT_EQ(type.get_num_bases(), 1);
            EXPECT_EQ(type.get_bases()[0].get_name(), "base");
        }
    }
    EXPECT_TRUE(saw_item_type);

    ASSERT_EQ(domain.get_num_predicates(), 1);
    const auto predicate = domain.get_predicates()[0];
    ASSERT_EQ(predicate.get_num_parameters(), 1);
    EXPECT_EQ(predicate.get_parameters()[0].get_num_types(), 1);

    ASSERT_EQ(domain.get_num_actions(), 1);
    const auto action = domain.get_actions()[0];
    ASSERT_EQ(action.get_num_parameters(), 1);
    EXPECT_EQ(action.get_parameters()[0].get_num_types(), 1);

    ASSERT_EQ(task.get_num_objects(), 1);
    EXPECT_EQ(task.get_objects()[0].get_num_types(), 1);

    ASSERT_EQ(task.get_num_initial_literals(), 1);
    const auto literal = task.get_initial_literals()[0];
    EXPECT_TRUE(literal.is_positive());
    EXPECT_EQ(literal.get_atom().get_num_terms(), 1);
    static_cast<void>(action.get_precondition().value().get_variant());
    static_cast<void>(action.get_effect().value().get_variant());
    static_cast<void>(literal.get_atom().get_terms()[0].get_variant());
}

TEST(LokiPublicFacade, ViewConvenienceMethodsCoverNumericEffectsAndMetrics)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain numeric-facade)
          (:requirements :strips :fluents)
          (:predicates (ready))
          (:functions (total-cost))
          (:action spend
            :parameters ()
            :precondition (ready)
            :effect (and (ready) (increase (total-cost) 1))))
    )" });

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(std::string { R"(
        (define (problem numeric-facade-task)
          (:domain numeric-facade)
          (:init (ready) (= (total-cost) 0))
          (:goal (ready))
          (:metric minimize (total-cost)))
    )" });

    ASSERT_EQ(domain.get_num_functions(), 1);
    ASSERT_EQ(domain.get_num_actions(), 1);
    const auto effect = domain.get_actions()[0].get_effect();
    ASSERT_TRUE(effect.has_value());
    auto saw_numeric_effect = false;
    ygg::visit(
        [&](const auto effect_node)
        {
            using EffectHandle = std::decay_t<decltype(effect_node.get_index())>;
            if constexpr (std::is_same_v<EffectHandle, ygg::Index<loki::formalism::EffectAnd>>)
            {
                ASSERT_EQ(effect_node.get_num_effects(), 2);
                ygg::visit(
                    [&](const auto nested_effect_node)
                    {
                        using NestedEffectHandle = std::decay_t<decltype(nested_effect_node.get_index())>;
                        if constexpr (std::is_same_v<NestedEffectHandle, ygg::Index<loki::formalism::EffectNumeric>>)
                        {
                            saw_numeric_effect = true;
                            EXPECT_EQ(nested_effect_node.get_operator(), loki::formalism::NumericEffectOperator::Increase);
                            EXPECT_EQ(nested_effect_node.get_function().get_name(), "total-cost");
                            EXPECT_EQ(nested_effect_node.get_num_terms(), 0);

                            ygg::visit(
                                [](const auto expression_node)
                                {
                                    using ExpressionHandle = std::decay_t<decltype(expression_node.get_index())>;
                                    if constexpr (std::is_same_v<ExpressionHandle, ygg::Index<loki::formalism::FunctionExpressionNumber>>)
                                    {
                                        EXPECT_EQ(expression_node.get_value(), 1);
                                    }
                                    else
                                    {
                                        ADD_FAILURE() << "Expected numeric effect expression to be a number.";
                                    }
                                },
                                nested_effect_node.get_expression().get_variant());
                        }
                    },
                    effect_node.get_effects()[1].get_variant());
            }
            else
            {
                ADD_FAILURE() << "Expected action effect to be an effect conjunction.";
            }
        },
        effect.value().get_variant());
    EXPECT_TRUE(saw_numeric_effect);

    ASSERT_TRUE(task.has_metric());
    const auto metric = task.get_metric().value();
    EXPECT_TRUE(metric.is_minimize());
    ygg::visit(
        [](const auto expression_node)
        {
            using ExpressionHandle = std::decay_t<decltype(expression_node.get_index())>;
            EXPECT_TRUE((std::is_same_v<ExpressionHandle, ygg::Index<loki::formalism::FunctionTerm>>) );
        },
        metric.get_expression().get_variant());
}

TEST(LokiPublicFacade, FormatsSemanticDomainAndTaskAsReparseablePddl)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain facade-format)
          (:requirements :strips)
          (:predicates (ready) (seen ?x))
          (:action wait
            :parameters (?x)
            :precondition (ready)
            :effect (and (ready) (seen ?x)))
        )
    )" });

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(std::string { R"(
        (define (problem facade-format-task)
          (:domain facade-format)
          (:objects item)
          (:init (ready))
          (:goal (seen item))
        )
    )" });

    const auto domain_text = loki::format_domain(domain);
    const auto task_text = loki::format_task(task);

    auto reparsed = loki::Parser(domain_text);

    const auto reparsed_domain = reparsed.get_domain();
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain.get_name());
    EXPECT_EQ(reparsed_task.get_name(), task.get_name());
    EXPECT_EQ(reparsed_task.get_domain().get_name(), reparsed_domain.get_name());
}

TEST(LokiPublicFacade, FormatsAlternativeEffectsAsReparseablePddl)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain facade-format-alternatives)
          (:requirements :strips :non-deterministic :probabilistic-effects)
          (:predicates (p) (q))
          (:action choose
            :parameters ()
            :effect (oneof (p) (q)))
          (:action sample
            :parameters ()
            :effect (probabilistic 0.25 (p) 0.75 (q)))
        )
    )" });

    const auto domain = parser.get_domain();
    const auto domain_text = loki::format_domain(domain);

    EXPECT_NE(domain_text.find("(oneof (p) (q))"), std::string::npos);
    EXPECT_NE(domain_text.find("(probabilistic 0.25 (p) 0.75 (q))"), std::string::npos);

    auto reparsed = loki::Parser(domain_text);
    const auto reparsed_domain = reparsed.get_domain();

    EXPECT_EQ(reparsed_domain.get_name(), domain.get_name());
    ASSERT_EQ(reparsed_domain.get_num_actions(), 2);
    ASSERT_TRUE(reparsed_domain.get_actions()[0].has_effect());
    ASSERT_TRUE(reparsed_domain.get_actions()[1].has_effect());
    ygg::visit(
        [](const auto effect_node)
        {
            using EffectHandle = std::decay_t<decltype(effect_node.get_index())>;
            EXPECT_TRUE((std::is_same_v<EffectHandle, ygg::Index<loki::formalism::EffectOneOf>>) );
        },
        reparsed_domain.get_actions()[0].get_effect().value().get_variant());
    ygg::visit(
        [](const auto effect_node)
        {
            using EffectHandle = std::decay_t<decltype(effect_node.get_index())>;
            EXPECT_TRUE((std::is_same_v<EffectHandle, ygg::Index<loki::formalism::EffectProbabilistic>>) );
        },
        reparsed_domain.get_actions()[1].get_effect().value().get_variant());
}

TEST(LokiPublicFacade, FormatsTypedNumericTaskSectionsAsReparseablePddl)
{
    auto parser = loki::Parser(std::string { R"(
        (define (domain facade-format-numeric)
          (:requirements :strips :typing :numeric-fluents)
          (:types item)
          (:constants depot - item)
          (:predicates (ready ?x - item))
          (:functions (cost ?x - item))
          (:action spend
            :parameters (?x - item)
            :precondition (ready ?x)
            :effect (and (ready ?x) (increase (cost ?x) 1.5)))
        )
    )" });

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(std::string { R"(
        (define (problem facade-format-numeric-task)
          (:domain facade-format-numeric)
          (:objects package - item)
          (:init (ready package) (= (cost package) 0))
          (:goal (ready package))
          (:metric minimize (cost package))
        )
    )" });

    const auto domain_text = loki::format_domain(domain);
    const auto task_text = loki::format_task(task);

    EXPECT_NE(domain_text.find("(:types item - object)"), std::string::npos);
    EXPECT_NE(domain_text.find("(:constants depot - item)"), std::string::npos);
    EXPECT_NE(domain_text.find("(cost ?x - item) - number"), std::string::npos);
    EXPECT_NE(domain_text.find("(increase (cost ?x) 1.5)"), std::string::npos);
    EXPECT_NE(task_text.find("(:objects package - item)"), std::string::npos);
    EXPECT_NE(task_text.find("(= (cost package) 0)"), std::string::npos);
    EXPECT_NE(task_text.find("(:metric minimize (cost package))"), std::string::npos);

    auto reparsed = loki::Parser(domain_text);
    const auto reparsed_domain = reparsed.get_domain();
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain.get_name());
    EXPECT_EQ(reparsed_domain.get_num_constants(), 1);
    ASSERT_EQ(reparsed_domain.get_num_functions(), 1);
    EXPECT_EQ(reparsed_domain.get_functions()[0].get_num_parameters(), 1);
    EXPECT_EQ(reparsed_domain.get_functions()[0].get_type().get_name(), "number");
    EXPECT_EQ(reparsed_task.get_num_objects(), 1);
    EXPECT_EQ(reparsed_task.get_num_initial_literals(), 1);
    EXPECT_EQ(reparsed_task.get_num_initial_function_values(), 1);
    ASSERT_TRUE(reparsed_task.has_metric());
    EXPECT_TRUE(reparsed_task.get_metric().value().is_minimize());
}

}  // namespace loki::tests
