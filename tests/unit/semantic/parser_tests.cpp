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

#include <gtest/gtest.h>
#include <loki/loki.hpp>
#include <loki/semantic.hpp>
#include <set>
#include <string>

namespace loki::tests
{

TEST(LokiSemanticParser, AllowsDuplicateActionNamesAndInternsIdenticalBodies)
{
    auto parser = semantic::Parser(fixture_path("duplicate-action-name"));

    EXPECT_EQ(parser.get_domain().get_actions().size(), 2);
}

TEST(LokiSemanticParser, LowercasesSourceBeforeParsing)
{
    auto options = semantic::ParserOptions {};
    options.add_action_costs = false;
    auto parser = semantic::Parser(read_text(fixture_path("mixed-case")), options);

    EXPECT_EQ(parser.get_domain().get_name(), "mixed-domain");
    ASSERT_EQ(parser.get_domain().get_predicates().size(), 1);
    EXPECT_EQ(parser.get_domain().get_predicates()[0].get_name(), "ready");

    const auto task = parser.parse_task(read_text(fixture_path("mixed-case", "task.pddl")));
    EXPECT_EQ(task.get_name(), "mixed-task");
    ASSERT_EQ(task.get_objects().size(), 1);
    EXPECT_EQ(task.get_objects()[0].get_name(), "item");
}

TEST(LokiSemanticParser, ReturnedRequirementsAreDeclaredVerbatim)
{
    auto options = semantic::ParserOptions {};
    options.add_action_costs = false;
    auto parser = semantic::Parser(fixture_path("adl-requirements"), options);

    // Permissive mode neither validates nor rewrites requirements: the domain
    // declares :adl, so :adl is what comes back.
    auto kinds = std::set<formalism::RequirementKind> {};
    for (const auto requirement : parser.get_domain().get_requirements())
        kinds.insert(requirement.get_kind());

    const auto expected = std::set<formalism::RequirementKind> { formalism::RequirementKind::Adl };
    EXPECT_EQ(kinds, expected);
}

TEST(LokiSemanticParser, StrictModeAllowsForwardTypeReferences)
{
    auto options = semantic::ParserOptions {};
    options.strict = true;
    options.add_action_costs = false;
    auto parser = semantic::Parser(fixture_path("forward-types"), options);

    auto saw_child = false;
    auto saw_parent = false;
    for (const auto type : parser.get_domain().get_types())
    {
        if (type.get_name() == "child")
        {
            ASSERT_EQ(type.get_bases().size(), 1);
            EXPECT_EQ(type.get_bases()[0].get_name(), "parent");
            saw_child = true;
        }
        else if (type.get_name() == "parent")
        {
            ASSERT_EQ(type.get_bases().size(), 1);
            EXPECT_EQ(type.get_bases()[0].get_name(), "object");
            saw_parent = true;
        }
    }
    EXPECT_TRUE(saw_child);
    EXPECT_TRUE(saw_parent);
}

TEST(LokiSemanticParser, StrictModeChecksTaskQuantifierTypesAcrossRepositories)
{
    auto options = semantic::ParserOptions {};
    options.strict = true;
    options.add_action_costs = false;
    auto parser = semantic::Parser(fixture_path("quantified-types"), options);

    EXPECT_NO_THROW(parser.parse_task(fixture_path("quantified-types", "task.pddl")));
}

TEST(LokiSemanticParser, PreservesComplementaryLiteralsInConjunctions)
{
    auto options = semantic::ParserOptions {};
    options.add_action_costs = false;
    auto parser = semantic::Parser(fixture_path("complementary-literals"), options);
    const auto action = parser.get_domain().get_actions()[0];
    ASSERT_TRUE(action.get_precondition().has_value());
    auto conjuncts = std::size_t { 0 };
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionAndView>)
                conjuncts = node.get_conditions().size();
        },
        action.get_precondition().value().get_variant());
    // A complementary literal pair must not be canonicalized away.
    EXPECT_EQ(conjuncts, 3);
}

TEST(LokiSemanticParser, ParsesAndTranslatesDistinctTasksAfterOneDomain)
{
    semantic::Parser parser(fixture_path("many-tasks"));

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);

    const auto first_task = parser.parse_task(fixture_path("many-tasks", "task.pddl"));
    const auto first_translated_result = semantic::translate(first_task, translation);
    const auto first_translated = first_translated_result.get_translated_task();
    const auto second_task = parser.parse_task(fixture_path("many-tasks", "task2.pddl"));
    const auto second_translated_result = semantic::translate(second_task, translation);
    const auto second_translated = second_translated_result.get_translated_task();

    auto has_object = [](formalism::TaskView task, const std::string& name)
    {
        for (auto object : task.get_objects())
            if (std::string(object.get_name()) == name)
                return true;
        return false;
    };

    EXPECT_NE(first_translated.get_context().get_index(), translation.get_translated_domain().get_context().get_index());
    EXPECT_NE(second_translated.get_context().get_index(), translation.get_translated_domain().get_context().get_index());
    EXPECT_EQ(&first_translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
    EXPECT_EQ(&second_translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
    EXPECT_TRUE(has_object(first_translated, "a"));
    EXPECT_TRUE(has_object(second_translated, "b"));
    EXPECT_NE(&first_translated.get_context(), &second_translated.get_context());
    EXPECT_NE(std::string(first_translated.get_name()), std::string(second_translated.get_name()));
}

TEST(LokiSemanticParser, FailedTaskDoesNotLeakVariableScopes)
{
    auto options = semantic::ParserOptions {};
    options.add_action_costs = false;
    auto parser = semantic::Parser(fixture_path("task-isolation"), options);

    EXPECT_THROW(parser.parse_task(fixture_path("task-isolation", "bad-task.pddl")), semantic::UndefinedVariableError);

    EXPECT_THROW(parser.parse_task(fixture_path("task-isolation", "leaked-variable-task.pddl")), semantic::UndefinedVariableError);

    const auto task = parser.parse_task(fixture_path("task-isolation", "task.pddl"));
    EXPECT_EQ(task.get_name(), "good");
}

}  // namespace loki::tests
