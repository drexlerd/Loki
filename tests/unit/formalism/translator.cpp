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


#include <gtest/gtest.h>

#include "../benchmark_utils.hpp"

#include <loki/loki.hpp>
#include <loki/formalism/formatter.hpp>
#include <loki/semantic/translator/copy_translator.hpp>

#include <algorithm>
#include <filesystem>
#include <limits>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_set>

namespace loki::tests
{
namespace fs = std::filesystem;

namespace
{

template<typename ObjectList>
std::unordered_set<std::string> object_names(ObjectList objects)
{
    auto result = std::unordered_set<std::string> {};
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
std::size_t count_effect_nodes(ygg::Index<formalism::Effect> effect, const formalism::Repository& repository);

template<typename Target, typename Node>
std::size_t count_nested_effect_nodes(const Node& node, const formalism::Repository& repository)
{
    if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectAnd>>)
    {
        auto result = std::size_t {};
        for (auto child : repository[node].effects)
            result += count_effect_nodes<Target>(child, repository);
        return result;
    }
    else if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectForall>>)
    {
        return count_effect_nodes<Target>(repository[node].effect, repository);
    }
    else if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectWhen>>)
    {
        return count_effect_nodes<Target>(repository[node].effect, repository);
    }
    else if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectOneOf>>)
    {
        auto result = std::size_t {};
        for (auto child : repository[node].effects)
            result += count_effect_nodes<Target>(child, repository);
        return result;
    }
    else if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectProbabilistic>>)
    {
        auto result = std::size_t {};
        for (auto alternative : repository[node].alternatives)
            result += count_effect_nodes<Target>(repository[alternative].effect, repository);
        return result;
    }
    else
    {
        return 0;
    }
}

template<typename Target>
std::size_t count_effect_nodes(ygg::Index<formalism::Effect> effect, const formalism::Repository& repository)
{
    return std::visit(
        [&](const auto& node) -> std::size_t
        {
            using Node = std::decay_t<decltype(node)>;
            auto result = count_nested_effect_nodes<Target>(node, repository);
            if constexpr (std::is_same_v<Node, ygg::Index<Target>>)
                ++result;
            return result;
        },
        repository[effect].value);
}


template<typename Target>
std::size_t count_condition_nodes(const formalism::Repository& repository)
{
    auto result = std::size_t {};
    for (auto i = ygg::uint_t { 0 }; i < repository.size<formalism::Condition>(); ++i)
    {
        std::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, ygg::Index<Target>>)
                    ++result;
            },
            repository[ygg::Index<formalism::Condition>(i)].value);
    }
    return result;
}


template<typename Target>
std::size_t count_condition_nodes(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit(
        [&](const auto& node) -> std::size_t
        {
            using Node = std::decay_t<decltype(node)>;
            auto result = std::size_t {};
            if constexpr (std::is_same_v<Node, ygg::Index<Target>>)
            {
                ++result;
            }
            if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionAnd>> || std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
            {
                for (auto child : repository[node].conditions)
                    result += count_condition_nodes<Target>(child, repository);
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionNot>>)
            {
                result += count_condition_nodes<Target>(repository[node].condition, repository);
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionImply>>)
            {
                result += count_condition_nodes<Target>(repository[node].left, repository);
                result += count_condition_nodes<Target>(repository[node].right, repository);
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionExists>> || std::is_same_v<Node, ygg::Index<formalism::ConditionForall>>)
            {
                result += count_condition_nodes<Target>(repository[node].condition, repository);
            }
            return result;
        },
        repository[condition].value);
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

    const auto domain_text = formalism::format::domain(domain_translation.get_translated_domain());
    const auto task_text = formalism::format::task(task_translation.get_translated_task());

    auto reparsed = loki::Parser(domain_text);


    const auto reparsed_domain = reparsed.get_domain();
    const auto reparsed_task = reparsed.parse_task(task_text);

    EXPECT_EQ(reparsed_domain.get_name(), domain_translation.get_translated_domain().get_name());
    EXPECT_EQ(reparsed_task.get_name(), task_translation.get_translated_task().get_name());
    EXPECT_EQ(reparsed_task.get_domain().get_name(), reparsed_domain.get_name());
}

} // namespace

TEST(LokiTests, LokiPddlTranslatorReparseTest)
{
    const auto first_domain_file = fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/domain.pddl");
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(first_domain_file);

    expect_translated_pddl_reparses(first_domain_file,
                                    fs::path(std::string(DATA_DIR) + "planning-benchmarks/tests/classical/gripper/test-1.pddl"));
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

    auto predicate_names = std::unordered_set<std::string> {};
    for (auto predicate : domain.get_predicates())
        predicate_names.insert(std::string(predicate.get_name()));

    EXPECT_TRUE(predicate_names.contains("loki-universal-0"));
    EXPECT_TRUE(predicate_names.contains("loki-universal-1"));
}

TEST(LokiTests, MultiplyConditionalEffectsSplitsActions)
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
    options.multiply_conditional_effects = true;

    const auto translation = loki::translate(parser.get_domain(), options);
    const auto domain = translation.get_translated_domain();
    const auto& repository = domain.get_context();

    auto has_conditional_effects_requirement = false;
    for (auto requirement : domain.get_requirements())
        has_conditional_effects_requirement |= repository[requirement.get_index()].kind == formalism::RequirementKind::ConditionalEffects;
    EXPECT_FALSE(has_conditional_effects_requirement);

    ASSERT_EQ(domain.get_actions().size(), std::size_t { 4 });
    for (auto action : domain.get_actions())
    {
        const auto& data = repository[action.get_index()];
        EXPECT_TRUE(std::string_view(data.name).starts_with("a_"));
        EXPECT_EQ(std::string_view(data.original_name), "a");
        ASSERT_TRUE(data.precondition.has_value());
        EXPECT_EQ(count_condition_nodes<formalism::ConditionLiteral>(*data.precondition, repository), std::size_t { 3 });
        if (data.effect)
        {
            EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(*data.effect, repository), std::size_t { 0 });
        }
    }
}

TEST(LokiTests, MultiplyConditionalEffectsThrowsOnOverflow)
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
    options.multiply_conditional_effects = true;

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
    const auto& repository = domain.get_context();

    for (auto predicate : domain.get_predicates())
        EXPECT_FALSE(std::string_view(predicate.get_name()).starts_with("_condition_"));

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(*action.effect, repository), std::size_t { 1 });
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(*action.effect, repository), std::size_t { 1 });

    auto checked_forall = false;
    for (auto i = ygg::uint_t { 0 }; i < repository.size<formalism::EffectForall>(); ++i)
    {
        const auto& effect_forall = repository[ygg::Index<formalism::EffectForall>(i)];
        std::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectWhen>>)
                {
                    checked_forall = true;
                    EXPECT_EQ(count_condition_nodes<formalism::ConditionExists>(repository[node].condition, repository), std::size_t { 0 });
                }
            },
            repository[effect_forall.effect].value);
    }
    EXPECT_TRUE(checked_forall);
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
    const auto& repository = domain.get_context();

    auto num_generated_universal_predicates = std::size_t {};
    for (auto predicate : domain.get_predicates())
        if (std::string_view(predicate.get_name()).starts_with("loki-universal-"))
            ++num_generated_universal_predicates;

    EXPECT_EQ(count_condition_nodes<formalism::ConditionForall>(repository), std::size_t { 0 });
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.precondition.has_value());

    const auto condition_or = std::get<ygg::Index<formalism::ConditionOr>>(repository[*action.precondition].value);
    ASSERT_EQ(repository[condition_or].conditions.size(), std::size_t { 2 });
    for (auto condition : repository[condition_or].conditions)
    {
        std::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_TRUE((std::is_same_v<Node, ygg::Index<formalism::ConditionForall>>));
            },
            repository[condition].value);
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(*action.effect, repository), std::size_t { 2 });

    for (auto i = ygg::uint_t { 0 }; i < repository.size<formalism::EffectWhen>(); ++i)
    {
        const auto& effect_when = repository[ygg::Index<formalism::EffectWhen>(i)];
        const auto& condition = repository[effect_when.condition];
        std::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_FALSE((std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>));
            },
            condition.value);
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
    const auto& repository = domain.get_context();

    ASSERT_EQ(domain.get_actions().size(), std::size_t { 2 });
    for (auto action_view : domain.get_actions())
    {
        const auto& action = repository[action_view.get_index()];
        ASSERT_TRUE(action.precondition.has_value());
        std::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_FALSE((std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>));
            },
            repository[*action.precondition].value);
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
    const auto& repository = domain.get_context();

    ASSERT_EQ(domain.get_axioms().size(), std::size_t { 2 });
    for (auto axiom_view : domain.get_axioms())
    {
        const auto& axiom = repository[axiom_view.get_index()];
        std::visit(
            [](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                EXPECT_FALSE((std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>));
            },
            repository[axiom.condition].value);
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectNumeric>(*action.effect, repository), std::size_t { 1 });
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.precondition.has_value());
    ASSERT_TRUE(action.effect.has_value());

    auto checked_precondition = false;
    std::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionAnd>>)
            {
                checked_precondition = true;
                EXPECT_EQ(repository[node].conditions.size(), std::size_t { 1 });
            }
        },
        repository[*action.precondition].value);
    EXPECT_TRUE(checked_precondition);

    auto checked_effect = false;
    std::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectAnd>>)
            {
                checked_effect = true;
                EXPECT_EQ(repository[node].effects.size(), std::size_t { 1 });
            }
        },
        repository[*action.effect].value);
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(*action.effect, repository), std::size_t { 1 });

    auto checked_when = false;
    for (auto i = ygg::uint_t { 0 }; i < repository.size<formalism::EffectWhen>(); ++i)
    {
        const auto& effect_when = repository[ygg::Index<formalism::EffectWhen>(i)];
        std::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionAnd>>)
                {
                    checked_when = true;
                    EXPECT_EQ(repository[node].conditions.size(), std::size_t { 2 });
                }
            },
            repository[effect_when.condition].value);
    }
    EXPECT_TRUE(checked_when);
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(*action.effect, repository), std::size_t { 2 });
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_EQ(count_effect_nodes<formalism::EffectForall>(*action.effect, repository), std::size_t { 1 });
    EXPECT_EQ(count_effect_nodes<formalism::EffectWhen>(*action.effect, repository), std::size_t { 1 });
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

    auto domain_generated_names = std::unordered_set<std::string> {};
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
        if ((generated_index(name, "loki-universal-") || generated_index(name, "loki-goal-"))
            && !domain_generated_names.contains(name))
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
    const auto& repository = task.get_context();

    ASSERT_FALSE(task.get_axioms().empty());
    auto has_derived_requirement = false;
    for (auto requirement : task.get_requirements())
    {
        if (repository[requirement.get_index()].kind == formalism::RequirementKind::DerivedPredicates)
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
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_axioms().empty());
    auto has_derived_requirement = false;
    for (auto requirement : domain.get_requirements())
    {
        if (repository[requirement.get_index()].kind == formalism::RequirementKind::DerivedPredicates)
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
    const auto translation = loki::translate(parser.get_domain(), loki::TranslatorOptions { .remove_typing = false });
    const auto domain = translation.get_translated_domain();
    const auto& repository = domain.get_context();

    ASSERT_FALSE(domain.get_types().empty());

    auto has_typing_requirement = false;
    for (auto requirement : domain.get_requirements())
        has_typing_requirement |= repository[requirement.get_index()].kind == formalism::RequirementKind::Typing;
    EXPECT_TRUE(has_typing_requirement);

    for (auto predicate : domain.get_predicates())
    {
        if (std::string(predicate.get_name()) == "thing")
            continue;
        for (auto parameter : repository[predicate.get_index()].parameters)
            EXPECT_FALSE(repository[parameter].types.empty());
    }

    for (auto function : domain.get_functions())
    {
        for (auto parameter : repository[function.get_index()].parameters)
            EXPECT_FALSE(repository[parameter].types.empty());
    }

    for (auto action : domain.get_actions())
    {
        for (auto parameter : repository[action.get_index()].parameters)
            EXPECT_FALSE(repository[parameter].types.empty());
        ASSERT_TRUE(repository[action.get_index()].precondition.has_value());
        EXPECT_GT(count_condition_nodes<formalism::ConditionLiteral>(repository), std::size_t { 1 });
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
    const auto translation = loki::translate(parser.get_domain(), loki::TranslatorOptions { .remove_typing = true });
    const auto domain = translation.get_translated_domain();
    const auto& repository = domain.get_context();

    EXPECT_TRUE(domain.get_types().empty());

    for (auto predicate : domain.get_predicates())
    {
        for (auto parameter : repository[predicate.get_index()].parameters)
            EXPECT_TRUE(repository[parameter].types.empty());
    }

    for (auto function : domain.get_functions())
    {
        for (auto parameter : repository[function.get_index()].parameters)
            EXPECT_TRUE(repository[parameter].types.empty());
    }

    for (auto action : domain.get_actions())
    {
        for (auto parameter : repository[action.get_index()].parameters)
            EXPECT_TRUE(repository[parameter].types.empty());
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
    const auto& repository = domain.get_context();

    const auto variable_name = [&](ygg::Index<formalism::Parameter> parameter) {
        return std::string(repository[repository[parameter].variable].name);
    };

    ASSERT_FALSE(domain.get_actions().empty());
    const auto& action = repository[domain.get_actions().front().get_index()];
    ASSERT_EQ(action.parameters.size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(action.parameters.front()), "x_0");

    ASSERT_TRUE(action.precondition.has_value());
    const auto exists = std::get<ygg::Index<formalism::ConditionExists>>(repository[*action.precondition].value);
    ASSERT_EQ(repository[exists].parameters.size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(repository[exists].parameters.front()), "x_1");

    const auto conjunction = std::get<ygg::Index<formalism::ConditionAnd>>(repository[repository[exists].condition].value);
    auto checked_forall = false;
    for (auto child : repository[conjunction].conditions)
    {
        std::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionForall>>)
                {
                    checked_forall = true;
                    ASSERT_EQ(repository[node].parameters.size(), std::size_t { 1 });
                    EXPECT_EQ(variable_name(repository[node].parameters.front()), "x_2");
                }
            },
            repository[child].value);
    }
    EXPECT_TRUE(checked_forall);

    ASSERT_TRUE(action.effect.has_value());
    const auto effect_forall = std::get<ygg::Index<formalism::EffectForall>>(repository[*action.effect].value);
    ASSERT_EQ(repository[effect_forall].parameters.size(), std::size_t { 1 });
    EXPECT_EQ(variable_name(repository[effect_forall].parameters.front()), "x_3");
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
        auto names = std::unordered_set<std::string> {};
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

} // namespace loki::tests
