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
#include "../formalism_utils.hpp"

#include <filesystem>
#include <gtest/gtest.h>
#include <loki/loki.hpp>
#include <loki/semantic/translator/copy_translator.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <yggdrasil/containers/associative_containers.hpp>

namespace loki::tests
{
namespace fs = std::filesystem;

namespace
{

template<typename ObjectList>
ygg::UnorderedSet<std::string> object_names(ObjectList objects)
{
    auto result = ygg::UnorderedSet<std::string> {};
    for (auto object : objects)
        result.insert(std::string(object.get_name()));
    return result;
}

}  // namespace

TEST(LokiTests, GeneratedUniversalPredicateKeepsNumericFreeVariables)
{
    auto parser = loki::Parser(fixture_path("numeric-universal"));
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    auto found = false;
    for (auto predicate : domain.get_predicates())
    {
        const auto name = std::string(predicate.get_name());
        if (!name.starts_with("loki-universal-"))
            continue;

        found = true;
        EXPECT_EQ(predicate.get_parameters().size(), std::size_t { 1 });
    }
    EXPECT_TRUE(found);
}

TEST(LokiTests, CompileConditionalEffectsSplitsActions)
{
    auto parser = loki::Parser(fixture_path("conditional-multiply"));
    auto options = loki::TranslatorOptions {};
    options.compile_conditional_effects = true;

    const auto translation = loki::translate(parser.get_domain(), options);
    const auto domain = translation.get_translated_domain();

    EXPECT_FALSE(has_requirement_kind(domain, formalism::RequirementKind::ConditionalEffects));

    ASSERT_EQ(domain.get_actions().size(), std::size_t { 4 });
    for (auto action : domain.get_actions())
    {
        EXPECT_TRUE(std::string_view(action.get_name()).starts_with("a_"));
        EXPECT_EQ(std::string_view(action.get_original_name()), "a");
        ASSERT_TRUE(action.get_precondition().has_value());
        EXPECT_EQ(count_condition_nodes<formalism::ConditionLiteral>(action.get_precondition().value()), std::size_t { 3 });
        if (const auto effect = action.get_effect())
        {
            EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(effect.value()), std::size_t { 0 });
        }
    }
}

TEST(LokiTests, CompileConditionalEffectsThrowsOnOverflow)
{
    auto parser = loki::Parser(fixture_path("conditional-overflow"));
    auto options = loki::TranslatorOptions {};
    options.compile_conditional_effects = true;

    EXPECT_THROW(loki::translate(parser.get_domain(), options), loki::SemanticError);
}

TEST(LokiTests, ExistentialConditionalEffectBecomesUniversalEffect)
{
    auto parser = loki::Parser(fixture_path("conditional-exists"));
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    for (auto predicate : domain.get_predicates())
        EXPECT_FALSE(std::string_view(predicate.get_name()).starts_with("_condition_"));

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(action.get_effect().value()), std::size_t { 1 });
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(action.get_effect().value()), std::size_t { 1 });

    const auto effect_forall = action.get_effect().value().get_variant().get<ygg::Index<formalism::EffectForall>>();
    const auto effect_when = effect_forall.get_effect().get_variant().get<ygg::Index<formalism::EffectWhen>>();
    EXPECT_EQ(count_condition_nodes<formalism::ConditionExists>(effect_when.get_condition()), std::size_t { 0 });
}

TEST(LokiTests, DnfDistributesUniversalOverDisjunction)
{
    auto parser = loki::Parser(fixture_path("dnf-forall"));
    auto storage = std::make_shared<semantic::detail::TranslationStorage>(1);
    auto translator = semantic::detail::CopyTranslator(storage, true, semantic::TranslationPhase::ToDisjunctiveNormalForm);
    const auto domain = translator.copy_domain(parser.get_domain());

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_precondition().has_value());

    const auto condition_or = action.get_precondition().value().get_variant().get<ygg::Index<formalism::ConditionOr>>();
    ASSERT_EQ(condition_or.get_conditions().size(), std::size_t { 2 });
    for (auto condition : condition_or.get_conditions())
    {
        ygg::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_TRUE((std::is_same_v<Node, formalism::ConditionForallView>) );
            },
            condition.get_variant());
    }
}

TEST(LokiTests, UntypedUniversalEffectKeepsEmptyGuardWhen)
{
    auto parser = loki::Parser(fixture_path("untyped-universal-effect"));
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(action.get_effect().value()), std::size_t { 1 });
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(action.get_effect().value()), std::size_t { 1 });
}

TEST(LokiTests, KeepTypingPreservesPersistentParameters)
{
    auto parser = loki::Parser(fixture_path("typed-signatures"));
    const auto translation = loki::translate(parser.get_domain(), loki::TranslatorOptions { .compile_typing = false });
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_types().empty());

    EXPECT_TRUE(has_requirement_kind(domain, formalism::RequirementKind::Typing));

    for (auto predicate : domain.get_predicates())
    {
        if (std::string(predicate.get_name()) == "thing")
            continue;
        for (auto parameter : predicate.get_parameters())
            EXPECT_FALSE(parameter.get_types().empty());
    }

    for (auto function : domain.get_functions())
    {
        for (auto parameter : function.get_parameters())
            EXPECT_FALSE(parameter.get_types().empty());
    }

    for (auto action : domain.get_actions())
    {
        for (auto parameter : action.get_parameters())
            EXPECT_FALSE(parameter.get_types().empty());
        ASSERT_TRUE(action.get_precondition().has_value());
        EXPECT_GT(count_condition_nodes<formalism::ConditionLiteral>(action.get_precondition().value()), std::size_t { 1 });
    }
}

TEST(LokiTests, RemoveTypingStripsPersistentParameters)
{
    auto parser = loki::Parser(fixture_path("typed-signatures"));
    const auto translation = loki::translate(parser.get_domain(), loki::TranslatorOptions { .compile_typing = true });
    const auto domain = translation.get_translated_domain();
    EXPECT_TRUE(domain.get_types().empty());

    for (auto predicate : domain.get_predicates())
    {
        for (auto parameter : predicate.get_parameters())
            EXPECT_TRUE(parameter.get_types().empty());
    }

    for (auto function : domain.get_functions())
    {
        for (auto parameter : function.get_parameters())
            EXPECT_TRUE(parameter.get_types().empty());
    }

    for (auto action : domain.get_actions())
    {
        for (auto parameter : action.get_parameters())
            EXPECT_TRUE(parameter.get_types().empty());
    }
}

TEST(LokiTests, RenameQuantifiedVariablesSeparatesNestedBinders)
{
    auto parser = loki::Parser(fixture_path("variable-renaming"));
    auto storage = std::make_shared<semantic::detail::TranslationStorage>(1);
    auto translator = semantic::detail::CopyTranslator(storage, true, semantic::TranslationPhase::RenameQuantifiedVariables);
    const auto domain = translator.copy_domain(parser.get_domain());
    const auto variable_name = [](formalism::ParameterView parameter) { return std::string(parameter.get_variable().get_name()); };

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_EQ(action.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(action.get_parameters().front()), "?x");

    ASSERT_TRUE(action.get_precondition().has_value());
    const auto exists = action.get_precondition().value().get_variant().get<ygg::Index<formalism::ConditionExists>>();
    ASSERT_EQ(exists.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(exists.get_parameters().front()), "?x_0");

    const auto conjunction = exists.get_condition().get_variant().get<ygg::Index<formalism::ConditionAnd>>();
    auto checked_forall = false;
    for (auto child : conjunction.get_conditions())
    {
        ygg::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, formalism::ConditionForallView>)
                {
                    checked_forall = true;
                    ASSERT_EQ(node.get_parameters().size(), std::size_t { 1 });
                    EXPECT_EQ(variable_name(node.get_parameters().front()), "?x_1");
                }
            },
            child.get_variant());
    }
    EXPECT_TRUE(checked_forall);

    ASSERT_TRUE(action.get_effect().has_value());
    const auto effect_forall = action.get_effect().value().get_variant().get<ygg::Index<formalism::EffectForall>>();
    ASSERT_EQ(effect_forall.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(effect_forall.get_parameters().front()), "?x_2");
}

TEST(LokiTests, LokiPddlTranslatorTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl");
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_file);
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(problem_file);

    auto parser = loki::Parser(domain_file);

    const auto domain = parser.get_domain();
    const auto problem = parser.parse_task(problem_file);
    const auto domain_translation_result = loki::translate(domain);
    const auto translated_domain = domain_translation_result.get_translated_domain();
    const auto problem_translation_result = loki::translate(problem, domain_translation_result);
    const auto translated_problem = problem_translation_result.get_translated_task();

    {
        const auto problem_objects = object_names(translated_problem.get_objects());
        for (auto constant : translated_domain.get_constants())
        {
            EXPECT_TRUE(problem_objects.contains(std::string(constant.get_name())));
        }
    }

    {
        EXPECT_EQ(translated_problem.get_domain().get_name(), translated_domain.get_name());
    }

    {
        auto names = ygg::UnorderedSet<std::string> {};
        for (auto object : translated_problem.get_objects())
        {
            EXPECT_TRUE(names.insert(std::string(object.get_name())).second);
        }
    }

    {
        EXPECT_FALSE(translated_domain.get_actions().empty());
        EXPECT_FALSE(translated_problem.get_initial_literals().empty());
        EXPECT_TRUE(translated_problem.get_goal().has_value());
    }
}

}  // namespace loki::tests
