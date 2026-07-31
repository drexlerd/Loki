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

#include <algorithm>
#include <gtest/gtest.h>
#include <loki/formalism/builder.hpp>
#include <loki/semantic/errors.hpp>
#include <loki/semantic/options.hpp>
#include <loki/semantic/parser.hpp>
#include <loki/semantic/translator.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>

namespace loki::tests
{
namespace
{

std::optional<std::string> conjunct_variable(formalism::ConditionView condition, std::string_view predicate_name, std::size_t term_index)
{
    if (!condition.get_variant().is<ygg::Index<formalism::ConditionAnd>>())
        return std::nullopt;

    for (auto child : condition.get_variant().get<ygg::Index<formalism::ConditionAnd>>().get_conditions())
    {
        auto result = std::optional<std::string> {};
        ygg::visit(
            [&](const auto& node)
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, formalism::ConditionLiteralView>)
                {
                    const auto atom = node.get_literal().get_atom();
                    if (atom.get_predicate().get_name() == predicate_name && atom.get_terms().size() > term_index)
                        result = variable_term_name(atom.get_terms()[term_index]);
                }
            },
            child.get_variant());
        if (result)
            return result;
    }
    return std::nullopt;
}

}  // namespace

TEST(LokiSemanticTranslator, RenamesQuantifiedVariablesDeterministically)
{
    semantic::Parser parser(fixture_path("rename-exists"));

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_parameters().size(), 2);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "?x");
    EXPECT_EQ(std::string(action.get_parameters()[1].get_variable().get_name()), "?x_0");
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_exists(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, SeparatesSiblingExistentialsOverSameVariable)
{
    semantic::Parser parser(fixture_path("sibling-exists"));
    const auto translation = semantic::translate(parser.get_domain());
    const auto action = translation.get_translated_domain().get_actions()[0];

    // Both existentials are lifted; the sibling scopes must stay independent bindings.
    ASSERT_EQ(action.get_parameters().size(), 2);
    const auto first = std::string(action.get_parameters()[0].get_variable().get_name());
    const auto second = std::string(action.get_parameters()[1].get_variable().get_name());
    EXPECT_NE(first, second);

    // Each conjunct must reference its own hoisted variable.
    ASSERT_TRUE(action.get_precondition().has_value());
    const auto first_reference = conjunct_variable(action.get_precondition().value(), "a", 0);
    const auto second_reference = conjunct_variable(action.get_precondition().value(), "b", 0);
    ASSERT_TRUE(first_reference.has_value());
    ASSERT_TRUE(second_reference.has_value());
    EXPECT_EQ(*first_reference, first);
    EXPECT_EQ(*second_reference, second);
}

TEST(LokiSemanticTranslator, LiftedParameterAvoidsFreeVariablesWhenRenaming)
{
    // The second sibling collides on ?p; its fresh name must avoid the action parameter ?p_0,
    // which occurs free in the sibling body.
    semantic::Parser parser(fixture_path("lift-capture"));
    const auto translation = semantic::translate(parser.get_domain());
    const auto action = translation.get_translated_domain().get_actions()[0];

    ASSERT_EQ(action.get_parameters().size(), 3);
    auto names = std::vector<std::string> {};
    for (auto parameter : action.get_parameters())
        names.push_back(std::string(parameter.get_variable().get_name()));
    // All parameters distinct, and the renamed binder did not capture the free ?p_0.
    EXPECT_EQ(names.size(), std::size_t { 3 });
    std::sort(names.begin(), names.end());
    EXPECT_EQ(std::unique(names.begin(), names.end()), names.end());
    ASSERT_TRUE(action.get_precondition().has_value());
    const auto free_reference = conjunct_variable(action.get_precondition().value(), "b", 1);
    ASSERT_TRUE(free_reference.has_value());
    EXPECT_EQ(*free_reference, "?p_0");
}

TEST(LokiSemanticTranslator, RenamesBeforeNegationNormalFormOnlyOnce)
{
    semantic::Parser parser(fixture_path("rename-once"));

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_axioms().size(), 1);
    const auto axiom = translated_domain.get_axioms()[0];
    ASSERT_EQ(axiom.get_parameters().size(), 1);
    EXPECT_EQ(std::string(axiom.get_parameters()[0].get_variable().get_name()), "?x_0");

    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_parameters().size(), 1);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "?x");
}

TEST(LokiSemanticTranslator, LowersNegatedExistsWithoutNegatingInnerCondition)
{
    semantic::Parser parser(fixture_path("negated-exists"));

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_axioms().size(), 1);
    const auto axiom = translated_domain.get_axioms()[0];
    EXPECT_TRUE(condition_mentions_predicate(axiom.get_condition(), "p"));
    EXPECT_TRUE(condition_mentions_predicate(axiom.get_condition(), "object"));
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_exists(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, GeneratesFreshAxiomsForIdenticalUniversalConditions)
{
    semantic::Parser parser(fixture_path("universal-cache"));

    const auto translation = semantic::translate(parser.get_domain(), semantic::TranslatorOptions { .compile_typing = false });
    const auto translated_domain = translation.get_translated_domain();

    auto generated_predicates = std::size_t {};
    auto generated_parameter_types = ygg::UnorderedSet<std::string> {};
    for (auto predicate : translated_domain.get_predicates())
    {
        if (!std::string(predicate.get_name()).starts_with("loki-universal-"))
            continue;
        ++generated_predicates;
        ASSERT_EQ(predicate.get_parameters().size(), std::size_t { 1 });
        const auto parameter = predicate.get_parameters().front();
        ASSERT_EQ(parameter.get_types().size(), std::size_t { 1 });
        generated_parameter_types.insert(std::string(parameter.get_types().front().get_name()));
    }

    EXPECT_EQ(generated_predicates, 2);
    EXPECT_EQ(generated_parameter_types.size(), std::size_t { 2 });
    EXPECT_TRUE(generated_parameter_types.contains("left"));
    EXPECT_TRUE(generated_parameter_types.contains("right"));
    EXPECT_EQ(translated_domain.get_axioms().size(), 2);
    ASSERT_EQ(translated_domain.get_actions().size(), 2);
    for (auto action : translated_domain.get_actions())
    {
        ASSERT_TRUE(action.get_precondition().has_value());
        EXPECT_FALSE(contains_forall(action.get_precondition().value()));
    }
}

TEST(LokiSemanticTranslator, MovesExistentialPreconditionVariablesToActionParameters)
{
    semantic::Parser parser(fixture_path("exists-precondition"));

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    EXPECT_EQ(action.get_arity(), 2);
    EXPECT_EQ(action.get_original_arity(), 1);
    EXPECT_EQ(action.get_parameters().size(), 2);
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_exists(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, CompilesTypingWhenEnabled)
{
    semantic::Parser parser(fixture_path("compile-typing"));

    const auto options = semantic::TranslatorOptions { .compile_typing = true };
    const auto translation = semantic::translate(parser.get_domain(), options);
    const auto translated_domain = translation.get_translated_domain();

    EXPECT_TRUE(has_predicate_named(translated_domain, "thing"));
    EXPECT_TRUE(has_predicate_named(translated_domain, "object"));
    EXPECT_FALSE(has_requirement_kind(translated_domain, formalism::RequirementKind::Typing));
    EXPECT_TRUE(translated_domain.get_types().empty());
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions().front();
    ASSERT_EQ(action.get_parameters().size(), 1);
    EXPECT_TRUE(action.get_parameters().front().get_types().empty());
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_TRUE(condition_mentions_predicate(action.get_precondition().value(), "thing"));

    const auto task = parser.parse_task(fixture_path("compile-typing", "task.pddl"));
    const auto translated_task_result = semantic::translate(task, translation, options);
    const auto translated_task = translated_task_result.get_translated_task();
    EXPECT_GE(count_initial_literals_for_predicate(translated_task.get_initial_literals(), "thing"), 2);
    EXPECT_GE(count_initial_literals_for_predicate(translated_task.get_initial_literals(), "object"), 2);

    const auto thing_predicate = predicate_named(translated_domain, "thing");
    ASSERT_TRUE(thing_predicate.has_value());
    EXPECT_TRUE(initial_literals_use_predicate(translated_task.get_initial_literals(), "thing", *thing_predicate));
}

TEST(LokiSemanticTranslator, PreservesTaskObjectTypesAfterDomainCanonicalization)
{
    semantic::Parser parser(fixture_path("task-object-types"));
    const auto domain_translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(fixture_path("task-object-types", "task.pddl")), domain_translation);
    const auto translated = translated_result.get_translated_task();

    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "bread-portion", "bread1"));
    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "content-portion", "content1"));
    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "place", "table1"));
    EXPECT_FALSE(has_initial_unary_literal(translated.get_initial_literals(), "place", "bread1"));
}

TEST(LokiSemanticTranslator, AddsEqualityPredicateWhenAdlDomainUsesEquality)
{
    semantic::Parser parser(fixture_path("adl-equality"));
    const auto options = semantic::TranslatorOptions { .materialize_equality = true };
    const auto translation = semantic::translate(parser.get_domain(), options);
    EXPECT_TRUE(has_equality_predicate(translation.get_translated_domain()));

    const auto translated_result = semantic::translate(parser.parse_task(fixture_path("adl-equality", "task.pddl")), translation, options);
    const auto translated = translated_result.get_translated_task();
    EXPECT_TRUE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), count_unique_object_names(translation.get_translated_domain(), translated));
}

TEST(LokiSemanticTranslator, InitializesEqualityForConstantsAndTaskObjects)
{
    semantic::Parser parser(fixture_path("equality-constants"));

    const auto options = semantic::TranslatorOptions { .materialize_equality = true };
    const auto translation = semantic::translate(parser.get_domain(), options);
    EXPECT_TRUE(has_equality_predicate(translation.get_translated_domain()));

    const auto task = parser.parse_task(fixture_path("equality-constants", "task.pddl"));
    const auto translated_result = semantic::translate(task, translation, options);
    const auto translated = translated_result.get_translated_task();

    EXPECT_TRUE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), count_unique_object_names(translation.get_translated_domain(), translated));
    EXPECT_EQ(&translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
}

TEST(LokiSemanticTranslator, SkipsEqualityInitializationWhenDisabled)
{
    semantic::Parser parser(fixture_path("equality-negated"));

    auto options = semantic::TranslatorOptions {};
    options.materialize_equality = false;
    const auto translation = semantic::translate(parser.get_domain(), options);
    EXPECT_FALSE(has_equality_predicate(translation.get_translated_domain()));

    const auto translated_result = semantic::translate(parser.parse_task(fixture_path("equality-negated", "task.pddl")), translation, options);
    const auto translated = translated_result.get_translated_task();
    EXPECT_FALSE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), 0);
}

TEST(LokiSemanticTranslator, RejectsTaskFromDifferentDomainTranslationRepository)
{
    auto first_parser = semantic::Parser(fixture_path("nnf"));
    auto second_parser = semantic::Parser(fixture_path("repo-mismatch"));

    const auto first_translation = semantic::translate(first_parser.get_domain());
    const auto second_task = second_parser.parse_task(fixture_path("repo-mismatch", "task.pddl"));

    try
    {
        static_cast<void>(semantic::translate(second_task, first_translation));
        FAIL() << "Expected mismatched domain diagnostic";
    }
    catch (const semantic::MismatchedDomainError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("nnf"), std::string::npos);
        EXPECT_NE(message.find("second-translation-domain"), std::string::npos);
    }
}

TEST(LokiSemanticTranslator, TaskEqualityRequiresTranslatedDomainEqualityPredicate)
{
    semantic::Parser parser(fixture_path("task-only-equality"));
    const auto options = semantic::TranslatorOptions { .materialize_equality = true };
    const auto translation = semantic::translate(parser.get_domain(), options);
    ASSERT_FALSE(has_equality_predicate(translation.get_translated_domain()));

    const auto task = parser.parse_task(fixture_path("task-only-equality", "task.pddl"));
    EXPECT_THROW(static_cast<void>(semantic::translate(task, translation, options)), semantic::InvalidEqualityError);
}

TEST(LokiSemanticTranslator, RenamesTaskGoalVariablesBeforeGoalSimplificationOnlyOnce)
{
    semantic::Parser parser(fixture_path("task-rename-once"));
    const auto domain_translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(fixture_path("task-rename-once", "task.pddl")), domain_translation);
    const auto translated = translated_result.get_translated_task();
    ASSERT_EQ(translated.get_axioms().size(), 1);
    const auto axiom = translated.get_axioms().front();
    ASSERT_EQ(axiom.get_parameters().size(), 1);
    EXPECT_EQ(std::string(axiom.get_parameters().front().get_variable().get_name()), "?x");
}

TEST(LokiCanonicalization, SortsSemanticFreeListsLexicographicallyBeforeInterning)
{
    auto repository = formalism::Repository(0);
    const auto p_predicate = formalism::get_or_create<formalism::Predicate>(repository, cista::offset::string("p"), ygg::IndexList<formalism::Parameter> {});
    const auto q_predicate = formalism::get_or_create<formalism::Predicate>(repository, cista::offset::string("q"), ygg::IndexList<formalism::Parameter> {});
    const auto p = p_predicate.get_index();
    const auto q = q_predicate.get_index();
    const auto p_atom = formalism::get_or_create<formalism::Atom>(repository, p, ygg::IndexList<formalism::Term> {}).get_index();
    const auto q_atom = formalism::get_or_create<formalism::Atom>(repository, q, ygg::IndexList<formalism::Term> {}).get_index();
    const auto p_literal = formalism::get_or_create<formalism::Literal>(repository, p_atom, true).get_index();
    const auto q_literal = formalism::get_or_create<formalism::Literal>(repository, q_atom, true).get_index();
    const auto p_condition_view = formalism::get_or_create<formalism::Condition>(
        repository,
        ygg::Data<formalism::Condition>::Variant(formalism::get_or_create<formalism::ConditionLiteral>(repository, p_literal).get_index()));
    const auto q_condition_view = formalism::get_or_create<formalism::Condition>(
        repository,
        ygg::Data<formalism::Condition>::Variant(formalism::get_or_create<formalism::ConditionLiteral>(repository, q_literal).get_index()));
    const auto p_condition = p_condition_view.get_index();
    const auto q_condition = q_condition_view.get_index();

    auto first_conditions = ygg::IndexList<formalism::Condition> {};
    first_conditions.push_back(q_condition);
    first_conditions.push_back(p_condition);
    const auto first = formalism::get_or_create<formalism::ConditionAnd>(repository, std::move(first_conditions));

    auto second_conditions = ygg::IndexList<formalism::Condition> {};
    second_conditions.push_back(p_condition);
    second_conditions.push_back(q_condition);
    const auto second = formalism::get_or_create<formalism::ConditionAnd>(repository, std::move(second_conditions));

    EXPECT_EQ(first, second);
    ASSERT_EQ(first.get_conditions().size(), 2);
    EXPECT_EQ(first.get_conditions()[0], p_condition_view);
    EXPECT_EQ(first.get_conditions()[1], q_condition_view);

    auto first_predicates = ygg::IndexList<formalism::Predicate> {};
    first_predicates.push_back(q);
    first_predicates.push_back(p);
    const auto first_domain = formalism::get_or_create<formalism::Domain>(repository,
                                                                          ygg::Data<formalism::Domain>(cista::offset::string("d"),
                                                                                                       ygg::IndexList<formalism::Requirement> {},
                                                                                                       ygg::IndexList<formalism::Type> {},
                                                                                                       ygg::IndexList<formalism::Object> {},
                                                                                                       std::move(first_predicates),
                                                                                                       ygg::IndexList<formalism::FunctionSkeleton> {},
                                                                                                       ygg::IndexList<formalism::Action> {},
                                                                                                       ygg::IndexList<formalism::Axiom> {}));

    auto second_predicates = ygg::IndexList<formalism::Predicate> {};
    second_predicates.push_back(p);
    second_predicates.push_back(q);
    const auto second_domain = formalism::get_or_create<formalism::Domain>(repository,
                                                                           ygg::Data<formalism::Domain>(cista::offset::string("d"),
                                                                                                        ygg::IndexList<formalism::Requirement> {},
                                                                                                        ygg::IndexList<formalism::Type> {},
                                                                                                        ygg::IndexList<formalism::Object> {},
                                                                                                        std::move(second_predicates),
                                                                                                        ygg::IndexList<formalism::FunctionSkeleton> {},
                                                                                                        ygg::IndexList<formalism::Action> {},
                                                                                                        ygg::IndexList<formalism::Axiom> {}));

    EXPECT_EQ(first_domain, second_domain);
    ASSERT_EQ(first_domain.get_predicates().size(), 2);
    EXPECT_EQ(first_domain.get_predicates()[0], p_predicate);
    EXPECT_EQ(first_domain.get_predicates()[1], q_predicate);
}

}  // namespace loki::tests
