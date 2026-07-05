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

#include <algorithm>
#include <filesystem>
#include <gtest/gtest.h>
#include <limits>
#include <loki/formalism/formatter.hpp>
#include <loki/loki.hpp>
#include <loki/semantic/translator/copy_translator.hpp>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <yggdrasil/containers/associative_containers.hpp>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/semantics/hash.hpp>

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

std::optional<size_t> generated_index(std::string_view name, std::string_view prefix)
{
    if (!name.starts_with(prefix) || name.size() == prefix.size())
        return std::nullopt;

    auto value = size_t { 0 };
    for (const auto c : name.substr(prefix.size()))
    {
        if (c < '0' || c > '9')
            return std::nullopt;
        value = value * 10 + static_cast<size_t>(c - '0');
    }
    return value;
}

size_t next_universal_index(auto predicates)
{
    auto next = size_t { 0 };
    for (auto predicate : predicates)
        if (const auto index = generated_index(std::string_view(predicate.get_name()), "loki-universal-"))
            next = std::max(next, *index + 1);
    return next;
}

template<typename Target>
std::size_t count_effect_nodes(formalism::EffectView effect);

template<typename Target, typename Node>
std::size_t count_nested_effect_nodes(const Node& node)
{
    if constexpr (std::is_same_v<Node, formalism::EffectAndView> || std::is_same_v<Node, formalism::EffectOneOfView>)
    {
        auto result = std::size_t {};
        for (auto child : node.get_effects())
            result += count_effect_nodes<Target>(child);
        return result;
    }
    else if constexpr (std::is_same_v<Node, formalism::EffectForallView> || std::is_same_v<Node, formalism::EffectWhenView>)
    {
        return count_effect_nodes<Target>(node.get_effect());
    }
    else if constexpr (std::is_same_v<Node, formalism::EffectProbabilisticView>)
    {
        auto result = std::size_t {};
        for (auto alternative : node.get_alternatives())
            result += count_effect_nodes<Target>(alternative.get_effect());
        return result;
    }
    else
    {
        return 0;
    }
}

template<typename Target>
std::size_t count_effect_nodes(formalism::EffectView effect)
{
    return ygg::visit(
        [&](const auto& node) -> std::size_t
        {
            using Node = std::decay_t<decltype(node)>;
            auto result = count_nested_effect_nodes<Target>(node);
            if constexpr (std::is_same_v<Node, formalism::EntityView<Target>>)
                ++result;
            return result;
        },
        effect.get_variant());
}

template<typename Target>
std::size_t count_condition_nodes(formalism::ConditionView condition)
{
    return ygg::visit(
        [&](const auto& node) -> std::size_t
        {
            using Node = std::decay_t<decltype(node)>;
            auto result = std::size_t {};
            if constexpr (std::is_same_v<Node, formalism::EntityView<Target>>)
            {
                ++result;
            }
            if constexpr (std::is_same_v<Node, formalism::ConditionAndView> || std::is_same_v<Node, formalism::ConditionOrView>)
            {
                for (auto child : node.get_conditions())
                    result += count_condition_nodes<Target>(child);
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionNotView>)
            {
                result += count_condition_nodes<Target>(node.get_condition());
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionImplyView>)
            {
                result += count_condition_nodes<Target>(node.get_left());
                result += count_condition_nodes<Target>(node.get_right());
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionExistsView> || std::is_same_v<Node, formalism::ConditionForallView>)
            {
                result += count_condition_nodes<Target>(node.get_condition());
            }
            return result;
        },
        condition.get_variant());
}

void expect_translated_pddl_reparses(const fs::path& domain_file, const fs::path& problem_file)
{
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(domain_file);
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(problem_file);

    auto parser = loki::Parser(domain_file);

    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(problem_file);

    const auto domain_translation = loki::translate(domain);
    const auto task_translation = loki::translate(task, domain_translation);

    const auto domain_text = formalism::format::to_string(domain_translation.get_translated_domain());
    const auto task_text = formalism::format::to_string(task_translation.get_translated_task());

    auto reparsed = loki::Parser(domain_text);

    const auto reparsed_domain = reparsed.get_domain();
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain_translation.get_translated_domain().get_name());
    EXPECT_EQ(reparsed_task.get_name(), task_translation.get_translated_task().get_name());
    EXPECT_EQ(reparsed_task.get_domain().get_name(), reparsed_domain.get_name());
}

}  // namespace

TEST(LokiTests, LokiPddlTranslatorReparseTest)
{
    const auto first_domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/domain.pddl");
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(first_domain_file);

    expect_translated_pddl_reparses(first_domain_file, fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/test-1.pddl"));
    expect_translated_pddl_reparses(fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl"),
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl"));
    expect_translated_pddl_reparses(fs::path(std::string(DATA_DIR) + "planning-benchmarks/profiling/ipc2023-numeric/delivery/domain.pddl"),
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/profiling/ipc2023-numeric/delivery/pfile1.pddl"));
}

TEST(LokiTests, GeneratedUniversalPredicateKeepsNumericFreeVariables)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain numeric-universal)
  (:requirements :strips :typing :universal-preconditions :numeric-fluents)
  (:types thing)
  (:predicates (done ?x - thing))
  (:functions (fuel ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (forall (?y - thing) (> (fuel ?x) 0))
    :effect (done ?x)))
)PDDL");

    auto parser = loki::Parser(domain_source);
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

TEST(LokiTests, GeneratedUniversalPredicateAvoidsExistingPredicateName)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain universal-name-collision)
  (:requirements :strips :typing :universal-preconditions)
  (:types thing)
  (:predicates (loki-universal-0) (p ?x - thing) (done))
  (:action a
    :parameters ()
    :precondition (forall (?x - thing) (p ?x))
    :effect (done)))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    auto predicate_names = ygg::UnorderedSet<std::string> {};
    for (auto predicate : domain.get_predicates())
        predicate_names.insert(std::string(predicate.get_name()));

    EXPECT_TRUE(predicate_names.contains("loki-universal-0"));
    EXPECT_TRUE(predicate_names.contains("loki-universal-1"));
}

TEST(LokiTests, CompileConditionalEffectsSplitsActions)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain conditional-multiply)
  (:requirements :strips :conditional-effects :negative-preconditions)
  (:predicates (p) (q) (r) (s) (t))
  (:action a
    :parameters ()
    :precondition (p)
    :effect (and (when (q) (r)) (when (s) (t))))
)
)PDDL");

    auto parser = loki::Parser(domain_source);
    auto options = loki::TranslatorOptions {};
    options.compile_conditional_effects = true;

    const auto translation = loki::translate(parser.get_domain(), options);
    const auto domain = translation.get_translated_domain();

    auto has_conditional_effects_requirement = false;
    for (auto requirement : domain.get_requirements())
        has_conditional_effects_requirement |= requirement.get_kind() == formalism::RequirementKind::ConditionalEffects;
    EXPECT_FALSE(has_conditional_effects_requirement);

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
    auto domain_source = std::ostringstream {};
    domain_source << "(define (domain conditional-overflow)\n";
    domain_source << "  (:requirements :strips :conditional-effects)\n";
    domain_source << "  (:predicates (p)";
    for (auto i = std::size_t { 0 }; i < std::numeric_limits<std::size_t>::digits; ++i)
        domain_source << " (c" << i << ")";
    domain_source << ")\n";
    domain_source << "  (:action a :parameters () :precondition (p) :effect (and";
    for (auto i = std::size_t { 0 }; i < std::numeric_limits<std::size_t>::digits; ++i)
        domain_source << " (when (c" << i << ") (p))";
    domain_source << "))\n)";

    auto parser = loki::Parser(domain_source.str());
    auto options = loki::TranslatorOptions {};
    options.compile_conditional_effects = true;

    EXPECT_THROW(loki::translate(parser.get_domain(), options), loki::SemanticError);
}

TEST(LokiTests, ExistentialConditionalEffectBecomesUniversalEffect)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain conditional-exists)
  (:requirements :strips :typing :conditional-effects :existential-preconditions)
  (:types thing)
  (:predicates (q ?x - thing ?y - thing) (done ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition ()
    :effect (when (exists (?y - thing) (q ?x ?y)) (done ?x))))
)PDDL");

    auto parser = loki::Parser(domain_source);
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

TEST(LokiTests, NestedUniversalRemovalEliminatesGeneratedForalls)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain nested-universal)
  (:requirements :strips :typing :universal-preconditions :existential-preconditions)
  (:types thing)
  (:predicates (p ?x - thing ?y - thing) (done ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (forall (?z - thing) (exists (?y - thing) (p ?z ?y)))
    :effect (done ?x)))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    auto num_generated_universal_predicates = std::size_t {};
    for (auto predicate : domain.get_predicates())
        if (std::string_view(predicate.get_name()).starts_with("loki-universal-"))
            ++num_generated_universal_predicates;

    for (auto action : domain.get_actions())
    {
        if (const auto precondition = action.get_precondition())
        {
            EXPECT_EQ(count_condition_nodes<formalism::ConditionForall>(precondition.value()), std::size_t { 0 });
        }
    }
    for (auto axiom : domain.get_axioms())
        EXPECT_EQ(count_condition_nodes<formalism::ConditionForall>(axiom.get_condition()), std::size_t { 0 });
    EXPECT_GE(num_generated_universal_predicates, std::size_t { 2 });
}

TEST(LokiTests, DnfDistributesUniversalOverDisjunction)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain dnf-forall)
  (:requirements :strips :typing :universal-preconditions :disjunctive-preconditions)
  (:types thing)
  (:predicates (p ?x - thing) (q ?x - thing) (done))
  (:action a
    :parameters ()
    :precondition (forall (?x - thing) (or (p ?x) (q ?x)))
    :effect (done)))
)PDDL");

    auto parser = loki::Parser(domain_source);
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

TEST(LokiTests, SplitDisjunctiveConditionalEffects)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain split-conditional-effect)
  (:requirements :strips :conditional-effects :disjunctive-preconditions)
  (:predicates (p ?x) (q ?x) (done ?x))
  (:action a
    :parameters (?x)
    :precondition ()
    :effect (when (or (p ?x) (q ?x)) (done ?x))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(action.get_effect().value()), std::size_t { 2 });

    const auto effect_and = action.get_effect().value().get_variant().get<ygg::Index<formalism::EffectAnd>>();
    ASSERT_EQ(effect_and.get_effects().size(), std::size_t { 2 });
    for (auto effect : effect_and.get_effects())
    {
        const auto effect_when = effect.get_variant().get<ygg::Index<formalism::EffectWhen>>();
        EXPECT_FALSE(effect_when.get_condition().get_variant().is<ygg::Index<formalism::ConditionOr>>());
    }
}

TEST(LokiTests, SplitDisjunctiveActionPreconditions)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain split-action-precondition)
  (:requirements :strips :disjunctive-preconditions)
  (:predicates (p) (q) (done))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (done)))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_EQ(domain.get_actions().size(), std::size_t { 2 });
    for (auto action : domain.get_actions())
    {
        ASSERT_TRUE(action.get_precondition().has_value());
        ygg::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_FALSE((std::is_same_v<Node, formalism::ConditionOrView>) );
            },
            action.get_precondition().value().get_variant());
    }
}

TEST(LokiTests, SplitDisjunctiveAxiomConditions)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain split-axiom-condition)
  (:requirements :strips :derived-predicates :disjunctive-preconditions)
  (:predicates (p) (q) (r))
  (:derived (r) (or (p) (q))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_EQ(domain.get_axioms().size(), std::size_t { 2 });
    for (auto axiom : domain.get_axioms())
    {
        ygg::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_FALSE((std::is_same_v<Node, formalism::ConditionOrView>) );
            },
            axiom.get_condition().get_variant());
    }
}

TEST(LokiTests, AggregatesNumericEffectsWithSameTarget)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain numeric-effects)
  (:requirements :strips :typing :numeric-fluents)
  (:types thing)
  (:predicates (done ?x - thing))
  (:functions (fuel ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition ()
    :effect (and (increase (fuel ?x) 1) (increase (fuel ?x) 2) (done ?x))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectNumeric>(action.get_effect().value()), std::size_t { 1 });
}

TEST(LokiTests, FlattenCanonicalizesDuplicateConditionsAndEffects)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain duplicate-flattening)
  (:requirements :strips)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (and (p) (p))
    :effect (and (q) (q))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_precondition().has_value());
    ASSERT_TRUE(action.get_effect().has_value());

    auto checked_precondition = false;
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionAndView>)
            {
                checked_precondition = true;
                EXPECT_EQ(node.get_conditions().size(), std::size_t { 1 });
            }
        },
        action.get_precondition().value().get_variant());
    EXPECT_TRUE(checked_precondition);

    auto checked_effect = false;
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::EffectAndView>)
            {
                checked_effect = true;
                EXPECT_EQ(node.get_effects().size(), std::size_t { 1 });
            }
        },
        action.get_effect().value().get_variant());
    EXPECT_TRUE(checked_effect);
}

TEST(LokiTests, NestedConditionalEffectsAreMerged)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain nested-conditional-effects)
  (:requirements :strips :conditional-effects)
  (:predicates (p ?x) (q ?x) (done ?x))
  (:action a
    :parameters (?x)
    :precondition ()
    :effect (when (p ?x) (when (q ?x) (done ?x)))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(action.get_effect().value()), std::size_t { 1 });

    const auto effect_when = action.get_effect().value().get_variant().get<ygg::Index<formalism::EffectWhen>>();
    const auto condition_and = effect_when.get_condition().get_variant().get<ygg::Index<formalism::ConditionAnd>>();
    EXPECT_EQ(condition_and.get_conditions().size(), std::size_t { 2 });
}

TEST(LokiTests, UniversalEffectDistributesOverConjunction)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain universal-effect-conjunction)
  (:requirements :strips :typing :universal-preconditions)
  (:types thing)
  (:predicates (p ?x - thing ?y - thing) (q ?x - thing ?y - thing))
  (:action a
    :parameters (?x - thing)
    :precondition ()
    :effect (forall (?y - thing) (and (p ?x ?y) (q ?x ?y)))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(action.get_effect().value()), std::size_t { 2 });
}

TEST(LokiTests, UntypedUniversalEffectKeepsEmptyGuardWhen)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain untyped-universal-effect)
  (:requirements :strips :universal-preconditions)
  (:predicates (p ?x))
  (:action a
    :parameters ()
    :precondition ()
    :effect (forall (?x) (p ?x))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(action.get_effect().value()), std::size_t { 1 });
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(action.get_effect().value()), std::size_t { 1 });
}

TEST(LokiTests, TaskGeneratedAxiomsDoNotReuseDomainGeneratedPredicateNames)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/miconic-fulladl/test-1.pddl");
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_file);
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(problem_file);

    auto parser = loki::Parser(domain_file);
    const auto domain = parser.get_domain();
    const auto task = parser.parse_task(problem_file);

    const auto domain_translation = loki::translate(domain);
    const auto translated_domain = domain_translation.get_translated_domain();
    const auto translated_domain_predicates = translated_domain.get_predicates().size();
    const auto task_translation = loki::translate(task, domain_translation);
    const auto translated_task = task_translation.get_translated_task();

    auto domain_generated_names = ygg::UnorderedSet<std::string> {};
    for (auto predicate : translated_domain.get_predicates())
    {
        const auto name = std::string(predicate.get_name());
        if (generated_index(name, "loki-universal-") || generated_index(name, "loki-goal-"))
            domain_generated_names.insert(name);
    }

    auto has_task_generated_name = false;
    for (auto predicate : translated_task.get_predicates())
    {
        const auto name = std::string(predicate.get_name());
        if ((generated_index(name, "loki-universal-") || generated_index(name, "loki-goal-")) && !domain_generated_names.contains(name))
        {
            has_task_generated_name = true;
        }
    }

    EXPECT_EQ(translated_task.get_domain().get_predicates().size(), translated_domain_predicates);
    EXPECT_TRUE(has_task_generated_name);
}

TEST(LokiTests, SimplifiedTaskGoalAddsDerivedRequirement)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain disjunctive-goal)
  (:requirements :strips :disjunctive-preconditions)
  (:predicates (p) (q))
)
)PDDL");
    const auto task_source = std::string(R"PDDL(
(define (problem disjunctive-goal-task)
  (:domain disjunctive-goal)
  (:requirements :strips :disjunctive-preconditions)
  (:init)
  (:goal (or (p) (q)))
)
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto domain_translation = loki::translate(parser.get_domain());
    const auto task_translation = loki::translate(parser.parse_task(task_source), domain_translation);
    const auto task = task_translation.get_translated_task();
    ASSERT_FALSE(task.get_axioms().empty());
    auto has_derived_requirement = false;
    for (auto requirement : task.get_requirements())
    {
        if (requirement.get_kind() == formalism::RequirementKind::DerivedPredicates)
        {
            has_derived_requirement = true;
            break;
        }
    }
    EXPECT_TRUE(has_derived_requirement);
}

TEST(LokiTests, ExistingDomainAxiomsAddDerivedRequirement)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain existing-derived)
  (:requirements :strips)
  (:predicates (p ?x) (q ?x))
  (:derived (q ?x) (p ?x))
  (:action a
    :parameters (?x)
    :precondition (q ?x)
    :effect (p ?x)))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain());
    const auto domain = translation.get_translated_domain();
    ASSERT_FALSE(domain.get_axioms().empty());
    auto has_derived_requirement = false;
    for (auto requirement : domain.get_requirements())
    {
        if (requirement.get_kind() == formalism::RequirementKind::DerivedPredicates)
        {
            has_derived_requirement = true;
            break;
        }
    }
    EXPECT_TRUE(has_derived_requirement);
}

TEST(LokiTests, KeepTypingPreservesPersistentParameters)
{
    const auto domain_source = std::string(R"PDDL(
(define (domain typed-signatures-kept)
  (:requirements :strips :typing :numeric-fluents)
  (:types thing)
  (:predicates (p ?x - thing) (done ?x - thing))
  (:functions (fuel ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (p ?x)
    :effect (and (done ?x) (increase (fuel ?x) 1))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    const auto translation = loki::translate(parser.get_domain(), loki::TranslatorOptions { .compile_typing = false });
    const auto domain = translation.get_translated_domain();

    ASSERT_FALSE(domain.get_types().empty());

    auto has_typing_requirement = false;
    for (auto requirement : domain.get_requirements())
        has_typing_requirement |= requirement.get_kind() == formalism::RequirementKind::Typing;
    EXPECT_TRUE(has_typing_requirement);

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
    const auto domain_source = std::string(R"PDDL(
(define (domain typed-signatures)
  (:requirements :strips :typing :numeric-fluents)
  (:types thing)
  (:predicates (p ?x - thing) (done ?x - thing))
  (:functions (fuel ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (p ?x)
    :effect (and (done ?x) (increase (fuel ?x) 1))))
)PDDL");

    auto parser = loki::Parser(domain_source);
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
    const auto domain_source = std::string(R"PDDL(
(define (domain variable-renaming)
  (:requirements :strips :typing :existential-preconditions :universal-preconditions)
  (:types thing)
  (:predicates (p ?x - thing) (q ?x - thing) (done ?x - thing))
  (:action a
    :parameters (?x - thing)
    :precondition (exists (?x - thing) (and (p ?x) (forall (?x - thing) (q ?x))))
    :effect (forall (?x - thing) (done ?x))))
)PDDL");

    auto parser = loki::Parser(domain_source);
    auto storage = std::make_shared<semantic::detail::TranslationStorage>(1);
    auto translator = semantic::detail::CopyTranslator(storage, true, semantic::TranslationPhase::RenameQuantifiedVariables);
    const auto domain = translator.copy_domain(parser.get_domain());
    const auto variable_name = [](formalism::ParameterView parameter) { return std::string(parameter.get_variable().get_name()); };

    ASSERT_FALSE(domain.get_actions().empty());
    const auto action = domain.get_actions().front();
    ASSERT_EQ(action.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(action.get_parameters().front()), "?x_0");

    ASSERT_TRUE(action.get_precondition().has_value());
    const auto exists = action.get_precondition().value().get_variant().get<ygg::Index<formalism::ConditionExists>>();
    ASSERT_EQ(exists.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(exists.get_parameters().front()), "?x_1");

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
                    EXPECT_EQ(variable_name(node.get_parameters().front()), "?x_2");
                }
            },
            child.get_variant());
    }
    EXPECT_TRUE(checked_forall);

    ASSERT_TRUE(action.get_effect().has_value());
    const auto effect_forall = action.get_effect().value().get_variant().get<ygg::Index<formalism::EffectForall>>();
    ASSERT_EQ(effect_forall.get_parameters().size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(effect_forall.get_parameters().front()), "?x_3");
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
