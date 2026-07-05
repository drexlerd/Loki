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
#include <fstream>
#include <gtest/gtest.h>
#include <loki/loki.hpp>
#include <loki/semantic.hpp>
#include <optional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <yggdrasil/semantics/equal_to.hpp>
#include <yggdrasil/serialization/json_suite.hpp>

namespace loki::tests
{
namespace fs = std::filesystem;

namespace semantic_suite
{
struct ParserSuiteCase
{
    std::string name;
    fs::path domain_file;
    fs::path task_file;
};

ParserSuiteCase parse_case(const boost::json::object& suite, const boost::json::object& object)
{
    return ParserSuiteCase { ygg::common::as_string(object, "name", "case"),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "domain_file", "case")),
                             ygg::common::suite_path(suite, ygg::common::as_string(object, "task_file", "case")) };
}

std::vector<ParserSuiteCase> load_cases()
{
    const auto suite_value = ygg::common::load_json_file(ygg::common::root_path() / "tests/unit/parser/suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<ParserSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_case(suite, ygg::common::as_object(case_value, "case")));
    return result;
}

}  // namespace semantic_suite

template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template<typename Views>
void expect_contiguous_indices(Views views, const std::string& label, bool require_order = true)
{
    SCOPED_TRACE(label);

    using View = std::remove_cvref_t<decltype(*views.begin())>;
    auto local = std::vector<View> {};
    local.reserve(views.size());
    for (auto view : views)
        if (&views.get_context().get_canonical_context(view.get_index()) == &views.get_context())
            local.push_back(view);
    if (local.empty())
        return;

    auto follows = [](auto previous, auto current)
    {
        using Index = decltype(previous.get_index());
        return current.get_index() == Index(previous.get_index().get_value() + 1);
    };

    auto sorted = local;
    std::sort(sorted.begin(), sorted.end(), [](auto lhs, auto rhs) { return lhs.get_index() < rhs.get_index(); });
    for (size_t i = 1; i < sorted.size(); ++i)
    {
        if (!follows(sorted[i - 1], sorted[i]))
        {
            ADD_FAILURE() << label << " has a local index gap between " << sorted[i - 1].get_index().get_value() << " and "
                          << sorted[i].get_index().get_value();
            break;
        }
    }

    if (!require_order)
        return;

    for (size_t i = 1; i < local.size(); ++i)
    {
        if (!follows(local[i - 1], local[i]))
        {
            ADD_FAILURE() << label << " is not stored in contiguous local canonical index order at offset " << i
                          << ": previous=" << local[i - 1].get_index().get_value() << ", current=" << local[i].get_index().get_value();
            break;
        }
    }
}

void expect_contiguous_domain_indices(formalism::DomainView domain)
{
    expect_contiguous_indices(domain.get_requirements(), "domain.requirements");
    expect_contiguous_indices(domain.get_types(), "domain.types", false);
    expect_contiguous_indices(domain.get_constants(), "domain.constants");
    expect_contiguous_indices(domain.get_predicates(), "domain.predicates");
    expect_contiguous_indices(domain.get_functions(), "domain.functions");
    expect_contiguous_indices(domain.get_actions(), "domain.actions");
    expect_contiguous_indices(domain.get_axioms(), "domain.axioms");
}

void expect_contiguous_task_indices(formalism::TaskView task)
{
    expect_contiguous_indices(task.get_requirements(), "task.requirements");
    expect_contiguous_indices(task.get_objects(), "task.objects");
    expect_contiguous_indices(task.get_initial_literals(), "task.initial_literals");
    expect_contiguous_indices(task.get_initial_function_values(), "task.initial_function_values");
    expect_contiguous_indices(task.get_axioms(), "task.axioms");
}

bool contains_not_or_imply(formalism::ConditionView condition)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::ConditionLiteralView) { return false; },
            [&](formalism::ConditionNumericConstraintView) { return false; },
            [&](formalism::ConditionNotView) { return true; },
            [&](formalism::ConditionImplyView) { return true; },
            [&](formalism::ConditionAndView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_not_or_imply(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionOrView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_not_or_imply(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionExistsView node) { return contains_not_or_imply(node.get_condition()); },
            [&](formalism::ConditionForallView node) { return contains_not_or_imply(node.get_condition()); },
        },
        condition.get_variant());
}

bool contains_forall(formalism::ConditionView condition)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::ConditionLiteralView) { return false; },
            [&](formalism::ConditionNumericConstraintView) { return false; },
            [&](formalism::ConditionForallView) { return true; },
            [&](formalism::ConditionNotView node) { return contains_forall(node.get_condition()); },
            [&](formalism::ConditionImplyView node) { return contains_forall(node.get_left()) || contains_forall(node.get_right()); },
            [&](formalism::ConditionAndView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_forall(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionOrView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_forall(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionExistsView node) { return contains_forall(node.get_condition()); },
        },
        condition.get_variant());
}

bool contains_exists(formalism::ConditionView condition)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::ConditionLiteralView) { return false; },
            [&](formalism::ConditionNumericConstraintView) { return false; },
            [&](formalism::ConditionExistsView) { return true; },
            [&](formalism::ConditionNotView node) { return contains_exists(node.get_condition()); },
            [&](formalism::ConditionImplyView node) { return contains_exists(node.get_left()) || contains_exists(node.get_right()); },
            [&](formalism::ConditionAndView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_exists(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionOrView node)
            {
                for (auto child : node.get_conditions())
                    if (contains_exists(child))
                        return true;
                return false;
            },
            [&](formalism::ConditionForallView node) { return contains_exists(node.get_condition()); },
        },
        condition.get_variant());
}

std::size_t count_effect_when(formalism::EffectView effect)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::EffectLiteralView) -> std::size_t { return 0; },
            [&](formalism::EffectNumericView) -> std::size_t { return 0; },
            [&](formalism::EffectOneOfView node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : node.get_effects())
                    result += count_effect_when(child);
                return result;
            },
            [&](formalism::EffectProbabilisticView node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto alternative : node.get_alternatives())
                    result += count_effect_when(alternative.get_effect());
                return result;
            },
            [&](formalism::EffectAndView node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : node.get_effects())
                    result += count_effect_when(child);
                return result;
            },
            [&](formalism::EffectForallView node) { return count_effect_when(node.get_effect()); },
            [&](formalism::EffectWhenView node) { return std::size_t { 1 } + count_effect_when(node.get_effect()); },
        },
        effect.get_variant());
}

bool is_effect_and(formalism::EffectView effect) { return effect.get_variant().is<ygg::Index<formalism::EffectAnd>>(); }

std::optional<ygg::Index<formalism::Object>> object_term(formalism::TermView term)
{
    return ygg::visit(
        Overloaded {
            [](formalism::ObjectView object) -> std::optional<ygg::Index<formalism::Object>> { return object.get_index(); },
            [](auto) -> std::optional<ygg::Index<formalism::Object>> { return std::nullopt; },
        },
        term.get_variant());
}

template<typename LiteralRange>
std::size_t count_equality_literals(const LiteralRange& literals)
{
    auto result = std::size_t { 0 };
    for (auto literal : literals)
    {
        if (!literal.get_polarity())
            continue;
        const auto atom = literal.get_atom();
        if (std::string(atom.get_predicate().get_name()) != "=" || atom.get_terms().size() != 2)
            continue;
        const auto left = object_term(atom.get_terms()[0]);
        const auto right = object_term(atom.get_terms()[1]);
        if (left && right && left->get_value() == right->get_value())
            ++result;
    }
    return result;
}

bool has_equality_predicate(formalism::DomainView domain)
{
    for (auto predicate : domain.get_predicates())
        if (std::string(predicate.get_name()) == "=")
            return true;
    return false;
}

bool has_requirement_kind(formalism::DomainView domain, formalism::RequirementKind kind)
{
    for (auto requirement : domain.get_requirements())
        if (requirement.get_kind() == kind)
            return true;
    return false;
}

bool has_predicate_named(formalism::DomainView domain, const std::string& name)
{
    for (auto predicate : domain.get_predicates())
        if (std::string(predicate.get_name()) == name)
            return true;
    return false;
}

std::optional<ygg::Index<formalism::Predicate>> predicate_named(formalism::DomainView domain, const std::string& name)
{
    for (auto predicate : domain.get_predicates())
        if (std::string(predicate.get_name()) == name)
            return predicate.get_index();
    return std::nullopt;
}

template<typename LiteralRange>
bool initial_literals_use_predicate(const LiteralRange& literals, const std::string& name, ygg::Index<formalism::Predicate> expected)
{
    auto found = false;
    for (auto literal : literals)
    {
        const auto predicate = literal.get_atom().get_predicate();
        if (std::string(predicate.get_name()) != name)
            continue;
        found = true;
        if (predicate.get_index() != expected)
            return false;
    }
    return found;
}

std::optional<std::string> object_term_name(formalism::TermView term)
{
    return ygg::visit(
        Overloaded {
            [](formalism::ObjectView object) -> std::optional<std::string> { return std::string(object.get_name()); },
            [](auto) -> std::optional<std::string> { return std::nullopt; },
        },
        term.get_variant());
}

template<typename LiteralRange>
bool has_initial_unary_literal(const LiteralRange& literals, const std::string& predicate_name, const std::string& object_name)
{
    for (auto literal : literals)
    {
        if (!literal.get_polarity())
            continue;
        const auto atom = literal.get_atom();
        if (atom.get_terms().size() != 1 || std::string(atom.get_predicate().get_name()) != predicate_name)
            continue;
        const auto object = object_term_name(atom.get_terms()[0]);
        if (object && *object == object_name)
            return true;
    }
    return false;
}

bool condition_mentions_predicate(formalism::ConditionView condition, const std::string& name)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::ConditionLiteralView node) { return std::string(node.get_literal().get_atom().get_predicate().get_name()) == name; },
            [&](formalism::ConditionNumericConstraintView) { return false; },
            [&](formalism::ConditionNotView node) { return condition_mentions_predicate(node.get_condition(), name); },
            [&](formalism::ConditionImplyView node)
            { return condition_mentions_predicate(node.get_left(), name) || condition_mentions_predicate(node.get_right(), name); },
            [&](formalism::ConditionAndView node)
            {
                for (auto child : node.get_conditions())
                    if (condition_mentions_predicate(child, name))
                        return true;
                return false;
            },
            [&](formalism::ConditionOrView node)
            {
                for (auto child : node.get_conditions())
                    if (condition_mentions_predicate(child, name))
                        return true;
                return false;
            },
            [&](formalism::ConditionExistsView node) { return condition_mentions_predicate(node.get_condition(), name); },
            [&](formalism::ConditionForallView node) { return condition_mentions_predicate(node.get_condition(), name); },
        },
        condition.get_variant());
}

std::optional<std::string> variable_term_name(formalism::TermView term)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::VariableView variable) -> std::optional<std::string> { return std::string(variable.get_name()); },
            [&](auto) -> std::optional<std::string> { return std::nullopt; },
        },
        term.get_variant());
}

bool has_top_level_effect_literal_with_terms(formalism::EffectView effect,
                                             const std::string& predicate_name,
                                             const std::string& first_variable,
                                             const std::string& second_variable)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::EffectLiteralView node)
            {
                const auto atom = node.get_literal().get_atom();
                if (std::string(atom.get_predicate().get_name()) != predicate_name || atom.get_terms().size() != 2)
                    return false;
                const auto first = variable_term_name(atom.get_terms()[0]);
                const auto second = variable_term_name(atom.get_terms()[1]);
                return first && second && *first == first_variable && *second == second_variable;
            },
            [&](formalism::EffectAndView node)
            {
                for (auto child : node.get_effects())
                {
                    if (has_top_level_effect_literal_with_terms(child, predicate_name, first_variable, second_variable))
                        return true;
                }
                return false;
            },
            [&](auto) { return false; },
        },
        effect.get_variant());
}

template<typename LiteralRange>
std::size_t count_initial_literals_for_predicate(const LiteralRange& literals, const std::string& name)
{
    auto result = std::size_t { 0 };
    for (auto literal : literals)
    {
        if (std::string(literal.get_atom().get_predicate().get_name()) == name)
            ++result;
    }
    return result;
}

std::size_t count_unique_object_names(formalism::DomainView domain, formalism::TaskView task)
{
    auto names = ygg::UnorderedSet<std::string> {};
    for (auto object : domain.get_constants())
        names.insert(std::string(object.get_name()));
    for (auto object : task.get_objects())
        names.insert(std::string(object.get_name()));
    return names.size();
}

std::optional<std::string> first_exists_parameter_name(formalism::ConditionView condition)
{
    return ygg::visit(
        Overloaded {
            [&](formalism::ConditionLiteralView) -> std::optional<std::string> { return std::nullopt; },
            [&](formalism::ConditionNumericConstraintView) -> std::optional<std::string> { return std::nullopt; },
            [&](formalism::ConditionNotView node) { return first_exists_parameter_name(node.get_condition()); },
            [&](formalism::ConditionImplyView node) -> std::optional<std::string>
            {
                if (auto result = first_exists_parameter_name(node.get_left()))
                    return result;
                return first_exists_parameter_name(node.get_right());
            },
            [&](formalism::ConditionAndView node) -> std::optional<std::string>
            {
                for (auto child : node.get_conditions())
                    if (auto result = first_exists_parameter_name(child))
                        return result;
                return std::nullopt;
            },
            [&](formalism::ConditionOrView node) -> std::optional<std::string>
            {
                for (auto child : node.get_conditions())
                    if (auto result = first_exists_parameter_name(child))
                        return result;
                return std::nullopt;
            },
            [&](formalism::ConditionExistsView node) -> std::optional<std::string>
            {
                if (node.get_parameters().empty())
                    return std::nullopt;
                return std::string(node.get_parameters().front().get_variable().get_name());
            },
            [&](formalism::ConditionForallView node) { return first_exists_parameter_name(node.get_condition()); },
        },
        condition.get_variant());
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

    EXPECT_TRUE(ygg::EqualTo<formalism::ConditionAndView> {}(first, second));
    ASSERT_EQ(first.get_conditions().size(), 2);
    EXPECT_TRUE(ygg::EqualTo<formalism::ConditionView> {}(first.get_conditions()[0], p_condition_view));
    EXPECT_TRUE(ygg::EqualTo<formalism::ConditionView> {}(first.get_conditions()[1], q_condition_view));

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

    EXPECT_TRUE(ygg::EqualTo<formalism::DomainView> {}(first_domain, second_domain));
    ASSERT_EQ(first_domain.get_predicates().size(), 2);
    EXPECT_TRUE(ygg::EqualTo<formalism::PredicateView> {}(first_domain.get_predicates()[0], p_predicate));
    EXPECT_TRUE(ygg::EqualTo<formalism::PredicateView> {}(first_domain.get_predicates()[1], q_predicate));
}

TEST(LokiSemanticTranslator, RewritesConditionsToNegationNormalForm)
{
    const auto domain_source = std::string { "(define (domain nnf)"
                                             "(:predicates (p) (q) (r))"
                                             "(:action a :parameters () "
                                             ":precondition (imply (not (and (p) (q))) (r)) "
                                             ":effect (and (p)))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_GT(translated_domain.get_actions().size(), 0);
    for (auto action : translated_domain.get_actions())
    {
        ASSERT_TRUE(action.get_precondition().has_value());
        EXPECT_FALSE(contains_not_or_imply(action.get_precondition().value()));
    }
}

TEST(LokiSemanticTranslator, RenamesQuantifiedVariablesDeterministically)
{
    const auto domain_source = std::string { "(define (domain rename)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (exists (?x - object) (p ?x)) "
                                             ":effect (and (p ?x)))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_parameters().size(), 2);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "?x_0");
    EXPECT_EQ(std::string(action.get_parameters()[1].get_variable().get_name()), "?x_1");
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_exists(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, RenamesBeforeNegationNormalFormOnlyOnce)
{
    const auto domain_source = std::string { "(define (domain rename-once)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (not (exists (?x - object) (p ?x ?x))) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_axioms().size(), 1);
    const auto axiom = translated_domain.get_axioms()[0];
    ASSERT_EQ(axiom.get_parameters().size(), 1);
    EXPECT_EQ(std::string(axiom.get_parameters()[0].get_variable().get_name()), "?x_1");

    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_parameters().size(), 1);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "?x_0");
}

TEST(LokiSemanticTranslator, RemovesUniversalQuantifiersWithDerivedAxioms)
{
    const auto domain_source = std::string { "(define (domain universal)"
                                             "(:requirements :typing :universal-preconditions)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (forall (?y - object) (p ?x ?y)) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto original_axioms = domain.get_axioms().size();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();

    EXPECT_GT(translated_domain.get_axioms().size(), original_axioms);
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_forall(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, LowersNegatedExistsWithoutNegatingInnerCondition)
{
    const auto domain_source = std::string { "(define (domain negated-exists)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (not (exists (?y - object) (p ?x ?y))) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

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

TEST(LokiSemanticTranslator, GeneratedAxiomParametersMatchHeadPredicateArity)
{
    const auto domain_source = std::string { "(define (domain generated-axiom-arity)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (not (exists (?y - object) (p ?x ?y))) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_axioms().size(), 1);
    const auto axiom = translated_domain.get_axioms().front();
    const auto literal = axiom.get_head();
    const auto atom = literal.get_atom();
    const auto predicate = atom.get_predicate();

    EXPECT_EQ(predicate.get_parameters().size(), atom.get_terms().size());
    EXPECT_GE(axiom.get_parameters().size(), atom.get_terms().size());
    EXPECT_FALSE(contains_exists(axiom.get_condition()));
}

TEST(LokiSemanticTranslator, GeneratesFreshAxiomsForIdenticalUniversalConditions)
{
    const auto domain_source = std::string { "(define (domain universal-cache)"
                                             "(:requirements :typing :universal-preconditions)"
                                             "(:types left right)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             "(:action a :parameters (?x - left) "
                                             ":precondition (forall (?y - object) (p ?x ?y)) "
                                             ":effect (and))"
                                             "(:action b :parameters (?x - right) "
                                             ":precondition (forall (?y - object) (p ?x ?y)) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain(), semantic::TranslatorOptions { .remove_typing = false });
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

TEST(LokiSemanticTranslator, SplitsDisjunctiveActionPreconditionsAfterDnf)
{
    const auto domain_source = std::string { "(define (domain split)"
                                             "(:requirements :disjunctive-preconditions)"
                                             "(:predicates (p) (q) (r))"
                                             "(:action a :parameters () "
                                             ":precondition (and (p) (or (q) (r))) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();

    ASSERT_EQ(translated_domain.get_actions().size(), 2);
    for (auto action : translated_domain.get_actions())
    {
        ASSERT_TRUE(action.get_precondition().has_value());
        EXPECT_FALSE(ygg::visit(
            Overloaded {
                [&](formalism::ConditionOrView) { return true; },
                [&](auto) { return false; },
            },
            action.get_precondition().value().get_variant()));
    }
}

TEST(LokiSemanticTranslator, MovesExistentialPreconditionVariablesToActionParameters)
{
    const auto domain_source = std::string { "(define (domain exists)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object ?y - object) (q ?x - object))"
                                             "(:action a :parameters (?x - object) "
                                             ":precondition (and (q ?x) (exists (?y - object) (p ?x ?y))) "
                                             ":effect (and))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions()[0];
    EXPECT_EQ(action.get_arity(), 2);
    EXPECT_EQ(action.get_original_arity(), 1);
    EXPECT_EQ(action.get_parameters().size(), 2);
    ASSERT_TRUE(action.get_precondition().has_value());
    EXPECT_FALSE(contains_exists(action.get_precondition().value()));
}

TEST(LokiSemanticTranslator, SplitsDisjunctiveWhenEffectsAndFlattensConjunctions)
{
    const auto domain_source = std::string { "(define (domain effects)"
                                             "(:requirements :conditional-effects :disjunctive-preconditions)"
                                             "(:predicates (p) (q) (r) (s))"
                                             "(:action a :parameters () "
                                             ":precondition () "
                                             ":effect (when (or (p) (q)) (and (r) (s))))"
                                             ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();

    ASSERT_EQ(translated_domain.get_actions().size(), 1);
    const auto action = translated_domain.get_actions().front();
    ASSERT_TRUE(action.get_effect().has_value());
    EXPECT_TRUE(is_effect_and(action.get_effect().value()));
    EXPECT_EQ(count_effect_when(action.get_effect().value()), 4);
}

TEST(LokiSemanticTranslator, KeepsActionScopedEffectVariablesAfterQuantifierRenaming)
{
    const auto domain_path = fs::path(std::string(DATA_DIR)) / "planning-benchmarks/tests/classical/airport/domain.pddl";
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_path);

    semantic::Parser parser(domain_path);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();

    auto found_takeoff = false;
    for (auto action : translated_domain.get_actions())
    {
        if (std::string(action.get_name()) != "takeoff")
            continue;

        found_takeoff = true;
        ASSERT_TRUE(action.get_effect().has_value());
        EXPECT_TRUE(has_top_level_effect_literal_with_terms(action.get_effect().value(), "blocked", "?s_0", "?a_0"));
        EXPECT_FALSE(has_top_level_effect_literal_with_terms(action.get_effect().value(), "blocked", "?s_2", "?a_0"));
    }
    EXPECT_TRUE(found_takeoff);
}

TEST(LokiSemanticTranslator, AddsTypePredicatesAndRemovesTypingByDefault)
{
    const auto domain_source = std::string { "(define (domain typing)"
                                             "(:requirements :typing)"
                                             "(:types thing)"
                                             "(:constants c - thing)"
                                             "(:predicates (p ?x - thing))"
                                             "(:action a :parameters (?x - thing) "
                                             ":precondition (p ?x) "
                                             ":effect (and (p ?x)))"
                                             ")" };
    const auto task_source = std::string { "(define (problem typing-problem) (:domain typing)"
                                           "(:objects o - thing)"
                                           "(:init)"
                                           "(:goal (p o))"
                                           ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
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

    const auto task = parser.parse_task(task_source);
    const auto translated_task_result = semantic::translate(task, translation);
    const auto translated_task = translated_task_result.get_translated_task();
    EXPECT_GE(count_initial_literals_for_predicate(translated_task.get_initial_literals(), "thing"), 2);
    EXPECT_GE(count_initial_literals_for_predicate(translated_task.get_initial_literals(), "object"), 2);

    const auto thing_predicate = predicate_named(translated_domain, "thing");
    ASSERT_TRUE(thing_predicate.has_value());
    EXPECT_TRUE(initial_literals_use_predicate(translated_task.get_initial_literals(), "thing", *thing_predicate));
}

TEST(LokiSemanticTranslator, PreservesTaskObjectTypesAfterDomainCanonicalization)
{
    const auto domain_path = fs::path(std::string(DATA_DIR)) / "planning-benchmarks/tests/classical/childsnack/domain.pddl";
    const auto task_path = fs::path(std::string(DATA_DIR)) / "planning-benchmarks/tests/classical/childsnack/test-1.pddl";
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_path);
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(task_path);

    semantic::Parser parser(domain_path);
    const auto domain_translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(task_path), domain_translation);
    const auto translated = translated_result.get_translated_task();

    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "bread-portion", "bread1"));
    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "content-portion", "content1"));
    EXPECT_TRUE(has_initial_unary_literal(translated.get_initial_literals(), "place", "table1"));
    EXPECT_FALSE(has_initial_unary_literal(translated.get_initial_literals(), "place", "bread1"));
}

TEST(LokiSemanticTranslator, AddsEqualityPredicateWhenAdlDomainUsesEquality)
{
    const auto domain_source = std::string { "(define (domain adl-equality)"
                                             "(:requirements :adl)"
                                             "(:predicates (p ?x))"
                                             "(:action a :parameters (?x) "
                                             ":precondition (= ?x ?x) "
                                             ":effect (p ?x))"
                                             ")" };
    const auto task_source = std::string { "(define (problem adl-equality-problem) (:domain adl-equality)"
                                           "(:objects o)"
                                           "(:init)"
                                           "(:goal (p o))"
                                           ")" };

    semantic::Parser parser(domain_source);
    const auto translation = semantic::translate(parser.get_domain());
    EXPECT_TRUE(has_equality_predicate(translation.get_translated_domain()));

    const auto translated_result = semantic::translate(parser.parse_task(task_source), translation);
    const auto translated = translated_result.get_translated_task();
    EXPECT_TRUE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), count_unique_object_names(translation.get_translated_domain(), translated));
}

TEST(LokiSemanticTranslator, InitializesEqualityForConstantsAndTaskObjects)
{
    const auto domain_source = std::string { "(define (domain equality)"
                                             "(:requirements :typing :equality)"
                                             "(:types thing)"
                                             "(:constants c - thing)"
                                             "(:predicates (p))"
                                             ")" };
    const auto task_source = std::string { "(define (problem equality-problem) (:domain equality)"
                                           "(:objects o - thing)"
                                           "(:init)"
                                           "(:goal (p))"
                                           ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    EXPECT_TRUE(has_equality_predicate(translation.get_translated_domain()));

    const auto task = parser.parse_task(task_source);
    const auto translated_result = semantic::translate(task, translation);
    const auto translated = translated_result.get_translated_task();

    EXPECT_TRUE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), count_unique_object_names(translation.get_translated_domain(), translated));
    EXPECT_EQ(&translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
}

TEST(LokiSemanticTranslator, SkipsEqualityInitializationWhenDisabled)
{
    const auto domain_source = std::string { "(define (domain equality)"
                                             "(:requirements :typing :equality)"
                                             "(:types thing)"
                                             "(:constants c - thing)"
                                             "(:predicates (p))"
                                             "(:action a :parameters (?x - thing ?y - thing) "
                                             ":precondition (not (= ?x ?y)) "
                                             ":effect (p))"
                                             ")" };
    const auto task_source = std::string { "(define (problem equality-problem) (:domain equality)"
                                           "(:objects o - thing)"
                                           "(:init)"
                                           "(:goal (p))"
                                           ")" };

    semantic::Parser parser(domain_source);

    auto options = semantic::TranslatorOptions {};
    options.initialize_equality = false;
    const auto translation = semantic::translate(parser.get_domain(), options);
    EXPECT_FALSE(has_equality_predicate(translation.get_translated_domain()));

    const auto translated_result = semantic::translate(parser.parse_task(task_source), translation, options);
    const auto translated = translated_result.get_translated_task();
    EXPECT_FALSE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_initial_literals()), 0);
}

TEST(LokiSemanticTranslator, GeneratedGoalPredicateAvoidsExistingNames)
{
    const auto domain_source = std::string { "(define (domain goal-name-collision)"
                                             "(:requirements :disjunctive-preconditions)"
                                             "(:predicates (loki-goal-0) (p) (q))"
                                             ")" };
    const auto task_source = std::string { "(define (problem goal-name-collision-problem) (:domain goal-name-collision)"
                                           "(:init)"
                                           "(:goal (or (p) (q)))"
                                           ")" };

    semantic::Parser parser(domain_source);
    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(task_source), translation);
    const auto translated = translated_result.get_translated_task();
    EXPECT_TRUE(has_predicate_named(translation.get_translated_domain(), "loki-goal-0"));

    auto found_generated = false;
    for (auto predicate : translated.get_predicates())
        found_generated |= std::string(predicate.get_name()) == "loki-goal-1";

    EXPECT_TRUE(found_generated);
}

TEST(LokiSemanticTranslator, RejectsTaskFromDifferentDomainTranslationRepository)
{
    auto first_parser = semantic::Parser(std::string { R"(
(define (domain first-translation-domain)
  (:predicates (p))
)
)" });
    auto second_parser = semantic::Parser(std::string { R"(
(define (domain second-translation-domain)
  (:predicates (q))
)
)" });

    const auto first_translation = semantic::translate(first_parser.get_domain());
    const auto second_task = second_parser.parse_task(std::string { R"(
(define (problem second-translation-task)
  (:domain second-translation-domain)
  (:init)
  (:goal (q))
)
)" });

    try
    {
        static_cast<void>(semantic::translate(second_task, first_translation));
        FAIL() << "Expected mismatched domain diagnostic";
    }
    catch (const semantic::MismatchedDomainError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("first-translation-domain"), std::string::npos);
        EXPECT_NE(message.find("second-translation-domain"), std::string::npos);
    }
}

TEST(LokiSemanticTranslator, TaskEqualityRequiresTranslatedDomainEqualityPredicate)
{
    const auto domain_source = std::string { "(define (domain task-only-equality)"
                                             "(:requirements :strips)"
                                             "(:predicates (p))"
                                             ")" };
    const auto task_source = std::string { "(define (problem task-only-equality-problem) (:domain task-only-equality)"
                                           "(:requirements :equality)"
                                           "(:objects o)"
                                           "(:init)"
                                           "(:goal (p))"
                                           ")" };

    semantic::Parser parser(domain_source);
    const auto translation = semantic::translate(parser.get_domain());
    ASSERT_FALSE(has_equality_predicate(translation.get_translated_domain()));

    const auto task = parser.parse_task(task_source);
    EXPECT_THROW(static_cast<void>(semantic::translate(task, translation)), semantic::InvalidEqualityError);
}

TEST(LokiSemanticTranslator, SimplifiesComplexTaskGoalsWithTaskAxioms)
{
    const auto domain_source = std::string { "(define (domain goal)"
                                             "(:requirements :disjunctive-preconditions)"
                                             "(:predicates (p) (q))"
                                             ")" };
    const auto task_source = std::string { "(define (problem goal-problem) (:domain goal)"
                                           "(:init)"
                                           "(:goal (or (p) (q)))"
                                           ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto domain_translation = semantic::translate(domain);
    const auto original_translated_domain_axioms = domain_translation.get_translated_domain().get_axioms().size();
    const auto task = parser.parse_task(task_source);
    const auto translated_result = semantic::translate(task, domain_translation);
    const auto translated = translated_result.get_translated_task();

    ASSERT_TRUE(translated.get_goal().has_value());
    EXPECT_FALSE(contains_not_or_imply(translated.get_goal().value()));
    EXPECT_EQ(translated.get_axioms().size(), 2);
    for (auto axiom : translated.get_axioms())
    {
        const auto is_disjunction = ygg::visit(
            Overloaded {
                [](formalism::ConditionOrView) { return true; },
                [](auto) { return false; },
            },
            axiom.get_condition().get_variant());
        EXPECT_FALSE(is_disjunction);
    }
    EXPECT_EQ(domain_translation.get_translated_domain().get_axioms().size(), original_translated_domain_axioms);
    EXPECT_GT(translated.get_predicates().size(), 0);
    EXPECT_EQ(translated.get_domain().get_predicates().size(), domain_translation.get_translated_domain().get_predicates().size());
}

TEST(LokiSemanticTranslator, RenamesTaskGoalVariablesBeforeGoalSimplificationOnlyOnce)
{
    const auto domain_source = std::string { "(define (domain task-rename-once)"
                                             "(:requirements :typing :existential-preconditions)"
                                             "(:predicates (p ?x - object ?y - object))"
                                             ")" };
    const auto task_source = std::string { "(define (problem task-rename-once-problem) (:domain task-rename-once)"
                                           "(:objects o - object)"
                                           "(:init)"
                                           "(:goal (not (exists (?x - object) (p ?x ?x))))"
                                           ")" };

    semantic::Parser parser(domain_source);
    const auto domain_translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(task_source), domain_translation);
    const auto translated = translated_result.get_translated_task();
    ASSERT_EQ(translated.get_axioms().size(), 1);
    const auto axiom = translated.get_axioms().front();
    ASSERT_EQ(axiom.get_parameters().size(), 1);
    EXPECT_EQ(std::string(axiom.get_parameters().front().get_variable().get_name()), "?x_0");
}

TEST(LokiSemanticParser, ReportsSyntaxFailureMessage)
{
    const auto domain = std::string { R"(
(define (domain bad-syntax)
  (:predicates (p)
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected parse error";
    }
    catch (const semantic::ParseError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("Could not parse PDDL domain"), std::string::npos);
        EXPECT_NE(message.find("In line 2:"), std::string::npos);
        EXPECT_NE(message.find("^_"), std::string::npos);
    }
}

TEST(LokiSemanticParser, IncludesFileNameForPathDiagnostics)
{
    const auto domain_path = fs::path(::testing::TempDir()) / "loki_bad_domain.pddl";
    {
        auto out = std::ofstream(domain_path);
        out << "(define (domain bad-syntax) (:predicates (p))";
    }

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

    const auto task_path = fs::path(::testing::TempDir()) / "loki_bad_task.pddl";
    {
        auto out = std::ofstream(task_path);
        out << "(define (problem p)\n";
        out << "  (:domain other)\n";
        out << "  (:init)\n";
        out << ")\n";
    }

    try
    {
        auto parser = semantic::Parser(std::string { "(define (domain d) (:predicates (p)))" });
        parser.parse_task(task_path);
        FAIL() << "Expected mismatched domain error";
    }
    catch (const semantic::MismatchedDomainError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("Task references domain 'other'"), std::string::npos);
        EXPECT_NE(message.find("In file " + task_path.string() + ", line 2:"), std::string::npos);
    }
}

TEST(LokiSemanticParser, StrictModeRejectsUndefinedPredicates)
{
    const auto domain = std::string { R"(
(define (domain strict-undefined)
  (:predicates)
  (:action a
    :parameters ()
    :precondition (missing)
    :effect (and))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = semantic::Parser(domain, options);
        FAIL() << "Expected UndefinedPredicateError";
    }
    catch (const semantic::UndefinedPredicateError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsDuplicatePredicateDefinitions)
{
    const auto domain = std::string { R"(
(define (domain duplicate-predicate)
  (:predicates (p) (p))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected duplicate predicate error";
    }
    catch (const semantic::DuplicatePredicateError&)
    {
    }
}

TEST(LokiSemanticParser, AllowsDuplicateActionNamesAndInternsIdenticalBodies)
{
    const auto domain = std::string { R"(
(define (domain duplicate-action-name)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :effect (p))
  (:action a
    :parameters ()
    :effect (q))
  (:action a
    :parameters ()
    :effect (p))
)
)" };

    auto parser = semantic::Parser(domain);

    EXPECT_EQ(parser.get_domain().get_actions().size(), 2);
}

TEST(LokiSemanticParser, StrictModeRejectsMissingRequirements)
{
    const auto domain = std::string { R"(
(define (domain missing-requirement)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = semantic::Parser(domain, options);
        FAIL() << "Expected missing requirement error";
    }
    catch (const semantic::MissingRequirementError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find(":disjunctive-preconditions"), std::string::npos);
        EXPECT_NE(message.find("In line 6:"), std::string::npos);
        EXPECT_NE(message.find("^_"), std::string::npos);
    }
}

TEST(LokiSemanticParser, PermissiveModeAllowsMissingRequirements)
{
    const auto domain = std::string { R"(
(define (domain permissive-requirements)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
)" };

    EXPECT_NO_THROW({
        auto parser = semantic::Parser(domain);
        (void) parser;
    });
}

TEST(LokiSemanticParser, StrictModeExpandsAdlRequirement)
{
    const auto domain = std::string { R"(
(define (domain adl-requirements)
  (:requirements :adl)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :precondition (or (p) (q))
    :effect (and))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;
    EXPECT_NO_THROW(semantic::Parser(domain, options));
}

TEST(LokiSemanticParser, StrictModeRejectsMissingEqualityRequirement)
{
    const auto domain = std::string { R"(
(define (domain missing-equality-requirement)
  (:predicates)
  (:action a
    :parameters (?x ?y)
    :precondition (= ?x ?y)
    :effect (and))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = semantic::Parser(domain, options);
        FAIL() << "Expected missing equality requirement";
    }
    catch (const semantic::MissingRequirementError& error)
    {
        EXPECT_NE(std::string(error.what()).find(":equality"), std::string::npos);
    }
}

TEST(LokiSemanticParser, ReportsInvalidEqualityArity)
{
    const auto domain = std::string { R"(
(define (domain invalid-equality)
  (:requirements :equality)
  (:predicates)
  (:action a
    :parameters (?x ?y ?z)
    :precondition (= ?x ?y ?z)
    :effect (and))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected invalid equality";
    }
    catch (const semantic::InvalidEqualityError&)
    {
    }
}

TEST(LokiSemanticParser, StrictModeRejectsPredicateArgumentTypeMismatch)
{
    const auto domain = std::string { R"(
(define (domain predicate-type-mismatch)
  (:requirements :typing)
  (:types room ball)
  (:constants b - ball)
  (:predicates (at ?r - room))
  (:action a
    :parameters ()
    :precondition (at b)
    :effect (and))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = semantic::Parser(domain, options);
        FAIL() << "Expected type mismatch";
    }
    catch (const semantic::TypeMismatchError&)
    {
    }
}

TEST(LokiSemanticParser, PermissiveModeAllowsPredicateArgumentTypeMismatch)
{
    const auto domain = std::string { R"(
(define (domain permissive-predicate-type-mismatch)
  (:requirements :typing)
  (:types room ball)
  (:constants b - ball)
  (:predicates (at ?r - room))
  (:action a
    :parameters ()
    :precondition (at b)
    :effect (and))
)
)" };

    EXPECT_NO_THROW({
        auto parser = semantic::Parser(domain);
        (void) parser;
    });
}

TEST(LokiSemanticParser, StrictModeRejectsFunctionArgumentTypeMismatch)
{
    const auto domain = std::string { R"(
(define (domain function-type-mismatch)
  (:requirements :typing :numeric-fluents)
  (:types room ball)
  (:constants b - ball)
  (:predicates)
  (:functions (distance ?r - room))
  (:action a
    :parameters ()
    :effect (assign (distance b) 0))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;

    try
    {
        auto parser = semantic::Parser(domain, options);
        FAIL() << "Expected type mismatch";
    }
    catch (const semantic::TypeMismatchError&)
    {
    }
}

TEST(LokiSemanticParser, AddsActionCostsDefaultsInPermissiveMode)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates)
  (:functions (total-cost))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };
    const auto task = std::string { R"(
(define (problem action-cost-task)
  (:domain action-cost-domain)
  (:init)
)
)" };

    auto parser = semantic::Parser(domain);
    const auto parsed_task = parser.parse_task(task);

    ASSERT_TRUE(parsed_task.get_metric().has_value());
    EXPECT_TRUE(parsed_task.get_metric().value().is_minimize());
    ygg::visit(
        [](const auto expression)
        {
            using Expression = std::decay_t<decltype(expression)>;
            EXPECT_TRUE((std::is_same_v<Expression, formalism::FunctionTermView>) );
            if constexpr (std::is_same_v<Expression, formalism::FunctionTermView>)
            {
                EXPECT_EQ(expression.get_function().get_name(), "total-cost");
                EXPECT_EQ(expression.get_terms().size(), 0);
            }
        },
        parsed_task.get_metric().value().get_expression().get_variant());

    ASSERT_EQ(parsed_task.get_initial_function_values().size(), 1);
    const auto initial_value = parsed_task.get_initial_function_values()[0];
    EXPECT_EQ(initial_value.get_function().get_function().get_name(), "total-cost");
    EXPECT_EQ(initial_value.get_function().get_terms().size(), 0);
    ygg::visit(
        [](const auto expression)
        {
            using Expression = std::decay_t<decltype(expression)>;
            EXPECT_TRUE((std::is_same_v<Expression, formalism::FunctionExpressionNumberView>) );
            if constexpr (std::is_same_v<Expression, formalism::FunctionExpressionNumberView>)
            {
                EXPECT_EQ(expression.get_value(), 0.0);
            }
        },
        initial_value.get_value().get_variant());
}

TEST(LokiSemanticParser, StrictActionCostsRequiresMetric)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates)
  (:functions (total-cost))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };
    const auto task = std::string { R"(
(define (problem action-cost-task)
  (:domain action-cost-domain)
  (:init (= (total-cost) 0))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;
    auto parser = semantic::Parser(domain, options);
    EXPECT_THROW(parser.parse_task(task), semantic::SemanticError);
}

TEST(LokiSemanticParser, StrictActionCostsRequiresInitialValue)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates)
  (:functions (total-cost))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };
    const auto task = std::string { R"(
(define (problem action-cost-task)
  (:domain action-cost-domain)
  (:init)
  (:metric minimize (total-cost))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;
    auto parser = semantic::Parser(domain, options);
    EXPECT_THROW(parser.parse_task(task), semantic::SemanticError);
}

TEST(LokiSemanticParser, StrictActionCostsRequiresTotalCostFunction)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;
    EXPECT_THROW(semantic::Parser(domain, options), semantic::SemanticError);
}

TEST(LokiSemanticParser, AddsTotalCostFunctionInPermissiveMode)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };

    auto parser = semantic::Parser(domain);
    auto found = false;
    for (auto function : parser.get_domain().get_functions())
        found = found || function.get_name() == "total-cost";
    EXPECT_TRUE(found);
}

TEST(LokiSemanticParser, AddActionCostsOptionInjectsUnitCosts)
{
    const auto domain = std::string { R"(
(define (domain plain-domain)
  (:requirements :strips)
  (:predicates (p))
  (:action flip
    :parameters ()
    :effect (p))
)
)" };
    const auto task = std::string { R"(
(define (problem plain-task)
  (:domain plain-domain)
  (:init)
  (:goal (p))
)
)" };

    auto options = parser::ParserOptions {};
    options.add_action_costs = true;
    auto parser = semantic::Parser(domain, options);

    const auto domain_text = loki::format_domain(parser.get_domain());
    EXPECT_NE(domain_text.find(":numeric-fluents"), std::string::npos);
    EXPECT_NE(domain_text.find("(total-cost)"), std::string::npos);
    EXPECT_NE(domain_text.find("(increase (total-cost) 1)"), std::string::npos);

    const auto parsed_task = parser.parse_task(task);
    ASSERT_TRUE(parsed_task.get_metric().has_value());
    EXPECT_TRUE(parsed_task.get_metric().value().is_minimize());
    ASSERT_EQ(parsed_task.get_initial_function_values().size(), 1);
    EXPECT_EQ(parsed_task.get_initial_function_values()[0].get_function().get_function().get_name(), "total-cost");
}

TEST(LokiSemanticParser, AddActionCostsOptionSkipsActionsAlreadyWritingTotalCost)
{
    const auto domain = std::string { R"(
(define (domain plain-domain)
  (:requirements :strips)
  (:predicates (p))
  (:action expensive
    :parameters ()
    :effect (and (p) (increase (total-cost) 5)))
  (:action cheap
    :parameters ()
    :effect (p))
)
)" };

    auto options = parser::ParserOptions {};
    options.add_action_costs = true;
    auto parser = semantic::Parser(domain, options);

    const auto domain_text = loki::format_domain(parser.get_domain());
    const auto count = [&](const std::string& needle)
    {
        auto occurrences = 0;
        for (auto pos = domain_text.find(needle); pos != std::string::npos; pos = domain_text.find(needle, pos + 1))
            ++occurrences;
        return occurrences;
    };
    EXPECT_EQ(count("(increase (total-cost) 5)"), 1);
    // Only the cheap action gets a unit cost; the expensive one already writes total-cost.
    EXPECT_EQ(count("(increase (total-cost) 1)"), 1);
}

TEST(LokiSemanticParser, AddActionCostsOptionCompletesInsteadOfStrictErrors)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:action pay
    :parameters ()
    :effect (increase (total-cost) 1))
)
)" };
    const auto task = std::string { R"(
(define (problem action-cost-task)
  (:domain action-cost-domain)
  (:init)
  (:goal (p))
)
)" };

    auto options = parser::ParserOptions {};
    options.strict = true;
    options.add_action_costs = true;
    auto parser = semantic::Parser(domain, options);
    const auto parsed_task = parser.parse_task(task);

    ASSERT_TRUE(parsed_task.get_metric().has_value());
    ASSERT_EQ(parsed_task.get_initial_function_values().size(), 1);
}

TEST(LokiSemanticParser, ActionCostsForbidsGeneralNumericWritesInPermissiveMode)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:functions (total-cost) (fuel))
  (:action burn
    :parameters ()
    :effect (assign (fuel) 5))
)
)" };

    EXPECT_THROW((semantic::Parser { domain }), semantic::MissingRequirementError);
}

TEST(LokiSemanticParser, ActionCostsForbidsIncreaseOfOtherFunctions)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:functions (total-cost) (fuel))
  (:action burn
    :parameters ()
    :effect (increase (fuel) 1))
)
)" };

    EXPECT_THROW((semantic::Parser { domain }), semantic::MissingRequirementError);
}

TEST(LokiSemanticParser, ActionCostsAllowsFunctionReadsInIncreaseAmounts)
{
    const auto domain = std::string { R"(
(define (domain action-cost-domain)
  (:requirements :action-costs)
  (:predicates (p))
  (:functions (total-cost) (road-length))
  (:action drive
    :parameters ()
    :effect (increase (total-cost) (road-length)))
)
)" };

    EXPECT_NO_THROW((semantic::Parser { domain }));
}

TEST(LokiSemanticParser, NumericUseWithoutRequirementErrorsInPermissiveMode)
{
    const auto domain = std::string { R"(
(define (domain no-requirement-domain)
  (:requirements :strips)
  (:predicates (p))
  (:functions (fuel))
)
)" };

    EXPECT_THROW((semantic::Parser { domain }), semantic::MissingRequirementError);
}

TEST(LokiSemanticParser, NumericFluentsTaskWithoutMetricKeepsMetricAbsent)
{
    const auto domain = std::string { R"(
(define (domain numeric-domain)
  (:requirements :numeric-fluents)
  (:predicates (p))
  (:functions (fuel))
  (:action burn
    :parameters ()
    :effect (decrease (fuel) 1))
)
)" };
    const auto task = std::string { R"(
(define (problem numeric-task)
  (:domain numeric-domain)
  (:init (= (fuel) 3))
  (:goal (p))
)
)" };

    for (const auto strict : { false, true })
    {
        auto options = parser::ParserOptions {};
        options.strict = strict;
        auto parser = semantic::Parser(domain, options);
        const auto parsed_task = parser.parse_task(task);
        EXPECT_FALSE(parsed_task.get_metric().has_value());
        ASSERT_EQ(parsed_task.get_initial_function_values().size(), 1);
        EXPECT_EQ(parsed_task.get_initial_function_values()[0].get_function().get_function().get_name(), "fuel");
    }
}

TEST(LokiSemanticParser, ReportsInvalidMetricOptimization)
{
    const auto domain = std::string { R"(
(define (domain metric-domain)
  (:requirements :numeric-fluents)
  (:predicates)
  (:functions (total-cost))
)
)" };
    const auto task = std::string { R"(
(define (problem metric-task)
  (:domain metric-domain)
  (:init)
  (:metric smallest (total-cost))
)
)" };

    auto parser = semantic::Parser(domain);
    try
    {
        parser.parse_task(task);
        FAIL() << "Expected invalid metric";
    }
    catch (const semantic::InvalidMetricError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsInvalidNumericEffectOperator)
{
    const auto domain = std::string { R"(
(define (domain invalid-numeric-effect)
  (:requirements :numeric-fluents)
  (:predicates)
  (:functions (total-cost))
  (:action a
    :parameters ()
    :effect (= (total-cost) 1))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected invalid numeric effect";
    }
    catch (const semantic::InvalidNumericEffectError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsInvalidNumericConstraintComparator)
{
    const auto domain = std::string { R"(
(define (domain invalid-numeric-constraint)
  (:requirements :numeric-fluents)
  (:predicates)
  (:functions (total-cost))
  (:action a
    :parameters ()
    :precondition (assign (total-cost) 1)
    :effect (and))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected invalid numeric constraint";
    }
    catch (const semantic::InvalidNumericConstraintError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsOutOfRangeProbabilisticEffectProbability)
{
    const auto domain = std::string { R"(
(define (domain invalid-probability)
  (:requirements :probabilistic-effects)
  (:predicates (p))
  (:action a
    :parameters ()
    :effect (probabilistic 1.2 (p)))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected invalid probabilistic effect";
    }
    catch (const semantic::InvalidProbabilisticEffectError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsProbabilisticEffectTotalAboveOne)
{
    const auto domain = std::string { R"(
(define (domain invalid-probability-total)
  (:requirements :probabilistic-effects)
  (:predicates (p) (q))
  (:action a
    :parameters ()
    :effect (probabilistic 0.7 (p) 0.6 (q)))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected invalid probabilistic effect";
    }
    catch (const semantic::InvalidProbabilisticEffectError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsDeclaredPredicateArityMismatch)
{
    const auto domain = std::string { R"(
(define (domain bad-arity)
  (:predicates (p ?x))
  (:action a
    :parameters ()
    :precondition (p)
    :effect (and))
)
)" };

    try
    {
        auto parser = semantic::Parser(domain);
        FAIL() << "Expected arity mismatch";
    }
    catch (const semantic::ArityMismatchError&)
    {
    }
}

TEST(LokiSemanticParser, ReportsMismatchedTaskDomain)
{
    const auto domain = std::string { R"(
(define (domain expected-domain)
  (:predicates (p))
)
)" };
    const auto task = std::string { R"(
(define (problem mismatched-domain-task)
  (:domain actual-domain)
  (:init)
  (:goal (and))
)
)" };

    auto parser = semantic::Parser(domain);
    try
    {
        parser.parse_task(task);
        FAIL() << "Expected mismatched domain diagnostic";
    }
    catch (const semantic::MismatchedDomainError& error)
    {
        const auto message = std::string(error.what());
        EXPECT_NE(message.find("actual-domain"), std::string::npos);
        EXPECT_NE(message.find("expected-domain"), std::string::npos);
    }
}

TEST(LokiSemanticParser, PermissiveModeKeepsImplicitPredicateCompatibility)
{
    const auto domain = std::string { R"(
(define (domain permissive-implicit)
  (:predicates)
  (:action a
    :parameters ()
    :precondition (missing)
    :effect (and))
)
)" };

    EXPECT_NO_THROW({
        auto parser = semantic::Parser(domain);
        (void) parser;
    });
}

TEST(LokiSemanticParser, ParsesAndTranslatesAllSuiteCases)
{
    const auto cases = semantic_suite::load_cases();
    ASSERT_FALSE(cases.empty());
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);
        try
        {
            semantic::Parser parser(item.domain_file);

            const auto domain = parser.get_domain();
            const auto translation = semantic::translate(domain);
            const auto task = parser.parse_task(item.task_file);
            const auto translated_result = semantic::translate(task, translation);
            const auto translated = translated_result.get_translated_task();
            EXPECT_EQ(&translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
        }
        catch (const std::exception& e)
        {
            ADD_FAILURE() << item.domain_file << " / " << item.task_file << ": " << e.what();
        }
    }
}

TEST(LokiSemanticParser, ParsesAllSuiteCasesWithContiguousTopLevelIndices)
{
    const auto cases = semantic_suite::load_cases();
    ASSERT_FALSE(cases.empty());
    if (!benchmark_suite_available(cases))
        GTEST_SKIP() << "Benchmark data unavailable: " << cases.front().domain_file;

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.domain_file);
        LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(item.task_file);
        try
        {
            semantic::Parser parser(item.domain_file);

            const auto domain = parser.get_domain();
            const auto task = parser.parse_task(item.task_file);
            EXPECT_TRUE(ygg::EqualTo<formalism::DomainView> {}(task.get_domain(), domain));
            expect_contiguous_domain_indices(domain);
            expect_contiguous_task_indices(task);
        }
        catch (const std::exception& e)
        {
            ADD_FAILURE() << item.domain_file << " / " << item.task_file << ": " << e.what();
        }
    }
}

TEST(LokiSemanticParser, ParsesDomainAndManyTasks)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests" / "classical" / "gripper";
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(root / "domain.pddl");
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(root / "test-1.pddl");

    semantic::Parser parser(root / "domain.pddl");

    const auto domain = parser.get_domain();
    EXPECT_EQ(std::string(domain.get_name()), "gripper-strips");
    EXPECT_GT(domain.get_actions().size(), 0);

    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    EXPECT_EQ(std::string(translated_domain.get_name()), "gripper-strips");
    EXPECT_NE(translated_domain.get_context().get_index(), domain.get_context().get_index());
    EXPECT_EQ(semantic::domain_translation_steps().size(), 9);
    EXPECT_EQ(semantic::task_translation_steps().size(), 10);
    const auto task1 = parser.parse_task(root / "test-1.pddl");
    const auto translated1_result = semantic::translate(task1, translation);
    const auto translated1 = translated1_result.get_translated_task();
    EXPECT_EQ(std::string(translated1.get_domain().get_name()), "gripper-strips");
    EXPECT_NE(translated1.get_context().get_index(), translated_domain.get_context().get_index());
    EXPECT_EQ(&translated1.get_domain().get_context().get_root(), &translated_domain.get_context().get_root());
    EXPECT_GT(translated1.get_initial_literals().size(), 0);

    const auto task2 = parser.parse_task(root / "test-1.pddl");
    const auto translated2_result = semantic::translate(task2, translation);
    const auto translated2 = translated2_result.get_translated_task();
    EXPECT_EQ(std::string(translated2.get_domain().get_name()), "gripper-strips");
    EXPECT_NE(translated2.get_context().get_index(), translated_domain.get_context().get_index());
    EXPECT_EQ(&translated2.get_domain().get_context().get_root(), &translated_domain.get_context().get_root());
    EXPECT_GT(translated2.get_initial_literals().size(), 0);
}

TEST(LokiSemanticParser, ParsesAndTranslatesDistinctTasksAfterOneDomain)
{
    const auto domain_source = std::string { "(define (domain many)"
                                             "(:predicates (p ?x))"
                                             ")" };
    const auto first_task_source = std::string { "(define (problem first) (:domain many)"
                                                 "(:objects a)"
                                                 "(:init (p a))"
                                                 "(:goal (p a))"
                                                 ")" };
    const auto second_task_source = std::string { "(define (problem second) (:domain many)"
                                                  "(:objects b)"
                                                  "(:init (p b))"
                                                  "(:goal (p b))"
                                                  ")" };

    semantic::Parser parser(domain_source);

    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);

    const auto first_task = parser.parse_task(first_task_source);
    const auto first_translated_result = semantic::translate(first_task, translation);
    const auto first_translated = first_translated_result.get_translated_task();
    const auto second_task = parser.parse_task(second_task_source);
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

}
