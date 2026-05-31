/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include <loki2/semantic.hpp>

#include <yggdrasil/serialization/json.hpp>

#include <algorithm>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace loki2::tests
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

fs::path suite_root_path()
{
    return fs::path(std::string(DATA_DIR)) / "..";
}

fs::path suite_path(const boost::json::object& suite, const std::string& path)
{
    const auto prefix = ygg::common::find_string(suite, "prefix", "suite").value_or("");
    return suite_root_path() / prefix / path;
}

ParserSuiteCase parse_case(const boost::json::object& suite, const boost::json::object& object)
{
    return ParserSuiteCase { ygg::common::as_string(object, "name", "case"),
                             suite_path(suite, ygg::common::as_string(object, "domain_file", "case")),
                             suite_path(suite, ygg::common::as_string(object, "task_file", "case")) };
}

std::vector<ParserSuiteCase> load_cases()
{
    const auto suite_value = ygg::common::load_json_file(suite_root_path() / "tests/unit/loki2/parser_suite.json");
    const auto& suite = ygg::common::as_object(suite_value, "suite");
    auto result = std::vector<ParserSuiteCase> {};
    for (const auto& case_value : ygg::common::as_array(suite, "cases", "suite"))
        result.push_back(parse_case(suite, ygg::common::as_object(case_value, "case")));
    return result;
}

} // namespace semantic_suite


template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template<typename T>
void expect_contiguous_indices(const ygg::IndexList<T>& indices, const std::string& label, const pddl::Repository& repository, bool require_order = true)
{
    SCOPED_TRACE(label);

    auto values = std::vector<ygg::uint_t> {};
    values.reserve(indices.size());
    for (auto index : indices)
    {
        if (&repository.get_canonical_context(index) == &repository)
            values.push_back(index.get_value());
    }
    if (values.empty())
        return;

    auto sorted = values;
    std::sort(sorted.begin(), sorted.end());
    for (size_t i = 1; i < sorted.size(); ++i)
    {
        if (sorted[i] != sorted[i - 1] + 1)
        {
            ADD_FAILURE() << label << " has a local index gap between " << sorted[i - 1] << " and " << sorted[i];
            break;
        }
    }

    if (!require_order)
        return;

    for (size_t i = 1; i < values.size(); ++i)
    {
        if (values[i] != values[i - 1] + 1)
        {
            ADD_FAILURE() << label << " is not stored in contiguous local canonical index order at offset " << i
                          << ": previous=" << values[i - 1] << ", current=" << values[i];
            break;
        }
    }
}

void expect_contiguous_domain_indices(pddl::DomainView domain)
{
    const auto& data = domain.get_data();
    expect_contiguous_indices(data.requirements, "domain.requirements", domain.get_context());
    expect_contiguous_indices(data.types, "domain.types", domain.get_context(), false);
    expect_contiguous_indices(data.constants, "domain.constants", domain.get_context());
    expect_contiguous_indices(data.predicates, "domain.predicates", domain.get_context());
    expect_contiguous_indices(data.functions, "domain.functions", domain.get_context());
    expect_contiguous_indices(data.actions, "domain.actions", domain.get_context());
    expect_contiguous_indices(data.axioms, "domain.axioms", domain.get_context());
}

void expect_contiguous_task_indices(pddl::TaskView task)
{
    const auto& data = task.get_data();
    expect_contiguous_indices(data.requirements, "task.requirements", task.get_context());
    expect_contiguous_indices(data.objects, "task.objects", task.get_context());
    expect_contiguous_indices(data.initial_literals, "task.initial_literals", task.get_context());
    expect_contiguous_indices(data.initial_function_values, "task.initial_function_values", task.get_context());
    expect_contiguous_indices(data.axioms, "task.axioms", task.get_context());
}

bool contains_not_or_imply(ygg::Index<pddl::Condition> condition, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::ConditionLiteral>) { return false; },
            [&](ygg::Index<pddl::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<pddl::ConditionNot>) { return true; },
            [&](ygg::Index<pddl::ConditionImply>) { return true; },
            [&](ygg::Index<pddl::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_not_or_imply(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_not_or_imply(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionExists> node) { return contains_not_or_imply(repository[node].condition, repository); },
            [&](ygg::Index<pddl::ConditionForall> node) { return contains_not_or_imply(repository[node].condition, repository); },
        },
        repository[condition].value);
}

bool contains_forall(ygg::Index<pddl::Condition> condition, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::ConditionLiteral>) { return false; },
            [&](ygg::Index<pddl::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<pddl::ConditionForall>) { return true; },
            [&](ygg::Index<pddl::ConditionNot> node) { return contains_forall(repository[node].condition, repository); },
            [&](ygg::Index<pddl::ConditionImply> node) { return contains_forall(repository[node].left, repository) || contains_forall(repository[node].right, repository); },
            [&](ygg::Index<pddl::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_forall(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_forall(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionExists> node) { return contains_forall(repository[node].condition, repository); },
        },
        repository[condition].value);
}

bool contains_exists(ygg::Index<pddl::Condition> condition, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::ConditionLiteral>) { return false; },
            [&](ygg::Index<pddl::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<pddl::ConditionExists>) { return true; },
            [&](ygg::Index<pddl::ConditionNot> node) { return contains_exists(repository[node].condition, repository); },
            [&](ygg::Index<pddl::ConditionImply> node) { return contains_exists(repository[node].left, repository) || contains_exists(repository[node].right, repository); },
            [&](ygg::Index<pddl::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_exists(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_exists(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionForall> node) { return contains_exists(repository[node].condition, repository); },
        },
        repository[condition].value);
}




std::size_t count_effect_when(ygg::Index<pddl::Effect> effect, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::EffectLiteral>) -> std::size_t { return 0; },
            [&](ygg::Index<pddl::EffectNumeric>) -> std::size_t { return 0; },
            [&](ygg::Index<pddl::EffectOneOf> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : repository[node].effects) result += count_effect_when(child, repository);
                return result;
            },
            [&](ygg::Index<pddl::EffectProbabilistic> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto alternative : repository[node].alternatives) result += count_effect_when(repository[alternative].effect, repository);
                return result;
            },
            [&](ygg::Index<pddl::EffectAnd> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : repository[node].effects) result += count_effect_when(child, repository);
                return result;
            },
            [&](ygg::Index<pddl::EffectForall> node) { return count_effect_when(repository[node].effect, repository); },
            [&](ygg::Index<pddl::EffectWhen> node) { return std::size_t { 1 } + count_effect_when(repository[node].effect, repository); },
        },
        repository[effect].value);
}

bool is_effect_and(ygg::Index<pddl::Effect> effect, const pddl::Repository& repository)
{
    return std::visit(Overloaded { [&](ygg::Index<pddl::EffectAnd>) { return true; }, [&](auto) { return false; } }, repository[effect].value);
}

std::optional<ygg::Index<pddl::Object>> object_term(ygg::Index<pddl::Term> term, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::Object> object) -> std::optional<ygg::Index<pddl::Object>> { return object; },
            [&](auto) -> std::optional<ygg::Index<pddl::Object>> { return std::nullopt; },
        },
        repository[term].value);
}

std::size_t count_equality_literals(const ygg::IndexList<pddl::Literal>& literals, const pddl::Repository& repository)
{
    auto result = std::size_t { 0 };
    for (auto literal : literals)
    {
        const auto& literal_data = repository[literal];
        if (!literal_data.positive)
            continue;
        const auto& atom = repository[literal_data.atom];
        const auto& predicate = repository[atom.predicate];
        if (std::string(predicate.name) != "=" || atom.terms.size() != 2)
            continue;
        const auto left = object_term(atom.terms[0], repository);
        const auto right = object_term(atom.terms[1], repository);
        if (left && right && left->get_value() == right->get_value())
            ++result;
    }
    return result;
}

bool has_equality_predicate(pddl::DomainView domain)
{
    const auto& repository = domain.get_context();
    for (auto predicate : domain.get_data().predicates)
        if (std::string(repository[predicate].name) == "=")
            return true;
    return false;
}

bool has_requirement_kind(const ygg::IndexList<pddl::Requirement>& requirements, pddl::RequirementKind kind, const pddl::Repository& repository)
{
    for (auto requirement : requirements)
        if (repository[requirement].kind == kind)
            return true;
    return false;
}

bool has_predicate_named(pddl::DomainView domain, const std::string& name)
{
    const auto& repository = domain.get_context();
    for (auto predicate : domain.get_data().predicates)
        if (std::string(repository[predicate].name) == name)
            return true;
    return false;
}

bool condition_mentions_predicate(ygg::Index<pddl::Condition> condition, const pddl::Repository& repository, const std::string& name)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::ConditionLiteral> node)
            {
                const auto literal = repository[node].literal;
                const auto atom = repository[literal].atom;
                const auto predicate = repository[atom].predicate;
                return std::string(repository[predicate].name) == name;
            },
            [&](ygg::Index<pddl::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<pddl::ConditionNot> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
            [&](ygg::Index<pddl::ConditionImply> node) { return condition_mentions_predicate(repository[node].left, repository, name) || condition_mentions_predicate(repository[node].right, repository, name); },
            [&](ygg::Index<pddl::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (condition_mentions_predicate(child, repository, name))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (condition_mentions_predicate(child, repository, name))
                        return true;
                return false;
            },
            [&](ygg::Index<pddl::ConditionExists> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
            [&](ygg::Index<pddl::ConditionForall> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
        },
        repository[condition].value);
}

std::size_t count_initial_literals_for_predicate(const ygg::IndexList<pddl::Literal>& literals, const pddl::Repository& repository, const std::string& name)
{
    auto result = std::size_t { 0 };
    for (auto literal : literals)
    {
        const auto atom = repository[literal].atom;
        const auto predicate = repository[atom].predicate;
        if (std::string(repository[predicate].name) == name)
            ++result;
    }
    return result;
}

std::size_t count_unique_object_names(pddl::DomainView domain, pddl::TaskView task)
{
    auto names = std::unordered_set<std::string> {};
    const auto& repository = task.get_context();
    for (auto object : domain.get_data().constants)
        names.insert(std::string(repository[object].name));
    for (auto object : task.get_data().objects)
        names.insert(std::string(repository[object].name));
    return names.size();
}

std::optional<std::string> first_exists_parameter_name(ygg::Index<pddl::Condition> condition, const pddl::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<pddl::ConditionLiteral>) -> std::optional<std::string> { return std::nullopt; },
            [&](ygg::Index<pddl::ConditionNumericConstraint>) -> std::optional<std::string> { return std::nullopt; },
            [&](ygg::Index<pddl::ConditionNot> node) { return first_exists_parameter_name(repository[node].condition, repository); },
            [&](ygg::Index<pddl::ConditionImply> node) -> std::optional<std::string>
            {
                if (auto result = first_exists_parameter_name(repository[node].left, repository))
                    return result;
                return first_exists_parameter_name(repository[node].right, repository);
            },
            [&](ygg::Index<pddl::ConditionAnd> node) -> std::optional<std::string>
            {
                for (auto child : repository[node].conditions)
                    if (auto result = first_exists_parameter_name(child, repository))
                        return result;
                return std::nullopt;
            },
            [&](ygg::Index<pddl::ConditionOr> node) -> std::optional<std::string>
            {
                for (auto child : repository[node].conditions)
                    if (auto result = first_exists_parameter_name(child, repository))
                        return result;
                return std::nullopt;
            },
            [&](ygg::Index<pddl::ConditionExists> node) -> std::optional<std::string>
            {
                const auto& exists = repository[node];
                if (exists.parameters.empty())
                    return std::nullopt;
                const auto variable = repository[exists.parameters.front()].variable;
                return std::string(repository[variable].name);
            },
            [&](ygg::Index<pddl::ConditionForall> node) { return first_exists_parameter_name(repository[node].condition, repository); },
        },
        repository[condition].value);
}

TEST(Loki2Canonicalization, SortsSemanticFreeListsLexicographicallyBeforeInterning)
{
    auto repository = pddl::Repository(0);
    auto builder = pddl::Builder {};

    const auto p = builder.predicate(repository, cista::offset::string("p"), {}).get_index();
    const auto q = builder.predicate(repository, cista::offset::string("q"), {}).get_index();
    const auto p_atom = builder.atom(repository, p, {}).get_index();
    const auto q_atom = builder.atom(repository, q, {}).get_index();
    const auto p_literal = builder.literal(repository, true, p_atom).get_index();
    const auto q_literal = builder.literal(repository, true, q_atom).get_index();
    const auto p_condition = builder.condition(repository, ygg::Data<pddl::Condition>::Variant(builder.condition_literal(repository, p_literal).get_index())).get_index();
    const auto q_condition = builder.condition(repository, ygg::Data<pddl::Condition>::Variant(builder.condition_literal(repository, q_literal).get_index())).get_index();

    auto first_conditions = ygg::IndexList<pddl::Condition> {};
    first_conditions.push_back(q_condition);
    first_conditions.push_back(p_condition);
    const auto first = builder.condition_and(repository, std::move(first_conditions)).get_index();

    auto second_conditions = ygg::IndexList<pddl::Condition> {};
    second_conditions.push_back(p_condition);
    second_conditions.push_back(q_condition);
    const auto second = builder.condition_and(repository, std::move(second_conditions)).get_index();

    EXPECT_EQ(first, second);
    ASSERT_EQ(repository[first].conditions.size(), 2);
    EXPECT_EQ(repository[first].conditions[0], p_condition);
    EXPECT_EQ(repository[first].conditions[1], q_condition);

    auto first_predicates = ygg::IndexList<pddl::Predicate> {};
    first_predicates.push_back(q);
    first_predicates.push_back(p);
    const auto first_domain = builder.domain(repository, ygg::Data<pddl::Domain>(cista::offset::string("d"), {}, {}, {}, std::move(first_predicates), {}, {}, {})).get_index();

    auto second_predicates = ygg::IndexList<pddl::Predicate> {};
    second_predicates.push_back(p);
    second_predicates.push_back(q);
    const auto second_domain = builder.domain(repository, ygg::Data<pddl::Domain>(cista::offset::string("d"), {}, {}, {}, std::move(second_predicates), {}, {}, {})).get_index();

    EXPECT_EQ(first_domain, second_domain);
    ASSERT_EQ(repository[first_domain].predicates.size(), 2);
    EXPECT_EQ(repository[first_domain].predicates[0], p);
    EXPECT_EQ(repository[first_domain].predicates[1], q);
}

TEST(Loki2SemanticTranslator, RewritesConditionsToNegationNormalForm)
{
    const auto domain_source = std::string {
        "(define (domain nnf)"
        "(:predicates (p) (q) (r))"
        "(:action a :parameters () "
        ":precondition (imply (not (and (p) (q))) (r)) "
        ":effect (and (p)))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& translated_repository = translated_domain.get_context();
    ASSERT_GT(translated_domain.get_data().actions.size(), 0);
    for (auto action_index : translated_domain.get_data().actions)
    {
        const auto& action = translated_repository[action_index];
        ASSERT_TRUE(action.precondition.has_value());
        EXPECT_FALSE(contains_not_or_imply(*action.precondition, translated_repository));
    }
}


TEST(Loki2SemanticTranslator, RenamesQuantifiedVariablesDeterministically)
{
    const auto domain_source = std::string {
        "(define (domain rename)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (exists (?x - object) (p ?x)) "
        ":effect (and (p ?x)))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();
    ASSERT_EQ(translated_domain.get_data().actions.size(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_EQ(action.parameters.size(), 2);
    const auto action_variable = repository[action.parameters.front()].variable;
    const auto lifted_variable = repository[action.parameters.back()].variable;
    EXPECT_EQ(std::string(repository[action_variable].name), "x_0");
    EXPECT_EQ(std::string(repository[lifted_variable].name), "x_1");
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_FALSE(contains_exists(*action.precondition, repository));
}

TEST(Loki2SemanticTranslator, RemovesUniversalQuantifiersWithDerivedAxioms)
{
    const auto domain_source = std::string {
        "(define (domain universal)"
        "(:requirements :typing :universal-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (forall (?y - object) (p ?x ?y)) "
        ":effect (and))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto original_axioms = domain.get_data().axioms.size();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    EXPECT_GT(translated_domain.get_data().axioms.size(), original_axioms);
    ASSERT_EQ(translated_domain.get_data().actions.size(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_FALSE(contains_forall(*action.precondition, repository));
}

TEST(Loki2SemanticTranslator, SplitsDisjunctiveActionPreconditionsAfterDnf)
{
    const auto domain_source = std::string {
        "(define (domain split)"
        "(:requirements :disjunctive-preconditions)"
        "(:predicates (p) (q) (r))"
        "(:action a :parameters () "
        ":precondition (and (p) (or (q) (r))) "
        ":effect (and))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    ASSERT_EQ(translated_domain.get_data().actions.size(), 2);
    for (auto action_index : translated_domain.get_data().actions)
    {
        const auto& action = repository[action_index];
        ASSERT_TRUE(action.precondition.has_value());
        EXPECT_FALSE(std::visit(Overloaded {
            [&](ygg::Index<pddl::ConditionOr>) { return true; },
            [&](auto) { return false; },
        }, repository[*action.precondition].value));
    }
}

TEST(Loki2SemanticTranslator, MovesExistentialPreconditionVariablesToActionParameters)
{
    const auto domain_source = std::string {
        "(define (domain exists)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object ?y - object) (q ?x - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (and (q ?x) (exists (?y - object) (p ?x ?y))) "
        ":effect (and))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();
    ASSERT_EQ(translated_domain.get_data().actions.size(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    EXPECT_EQ(action.parameters.size(), 2);
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_FALSE(contains_exists(*action.precondition, repository));
}

TEST(Loki2SemanticTranslator, SplitsDisjunctiveWhenEffectsAndFlattensConjunctions)
{
    const auto domain_source = std::string {
        "(define (domain effects)"
        "(:requirements :conditional-effects :disjunctive-preconditions)"
        "(:predicates (p) (q) (r) (s))"
        "(:action a :parameters () "
        ":precondition () "
        ":effect (when (or (p) (q)) (and (r) (s))))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    ASSERT_EQ(translated_domain.get_data().actions.size(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_TRUE(is_effect_and(*action.effect, repository));
    EXPECT_EQ(count_effect_when(*action.effect, repository), 4);
}

TEST(Loki2SemanticTranslator, AddsTypePredicatesAndRemovesTypingByDefault)
{
    const auto domain_source = std::string {
        "(define (domain typing)"
        "(:requirements :typing)"
        "(:types thing)"
        "(:constants c - thing)"
        "(:predicates (p ?x - thing))"
        "(:action a :parameters (?x - thing) "
        ":precondition (p ?x) "
        ":effect (and (p ?x)))"
        ")" };
    const auto task_source = std::string {
        "(define (problem typing-problem) (:domain typing)"
        "(:objects o - thing)"
        "(:init)"
        "(:goal (p o))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    EXPECT_TRUE(has_predicate_named(translated_domain, "thing"));
    EXPECT_FALSE(has_requirement_kind(translated_domain.get_data().requirements, pddl::RequirementKind::Typing, repository));
    EXPECT_TRUE(translated_domain.get_data().types.empty());
    ASSERT_EQ(translated_domain.get_data().actions.size(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_EQ(action.parameters.size(), 1);
    EXPECT_TRUE(repository[action.parameters.front()].types.empty());
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_TRUE(condition_mentions_predicate(*action.precondition, repository, "thing"));

    const auto task = parser.parse_task(task_source);
    const auto translated_task_result = semantic::translate(task, translation);
    const auto translated_task = translated_task_result.get_translated_task();
    EXPECT_GE(count_initial_literals_for_predicate(translated_task.get_data().initial_literals, translated_task.get_context(), "thing"), 2);
}

TEST(Loki2SemanticTranslator, InitializesEqualityForConstantsAndTaskObjects)
{
    const auto domain_source = std::string {
        "(define (domain equality)"
        "(:requirements :typing :equality)"
        "(:types thing)"
        "(:constants c - thing)"
        "(:predicates (p))"
        ")" };
    const auto task_source = std::string {
        "(define (problem equality-problem) (:domain equality)"
        "(:objects o - thing)"
        "(:init)"
        "(:goal (p))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);
    const auto task = parser.parse_task(task_source);
    const auto translated_result = semantic::translate(task, translation);
    const auto translated = translated_result.get_translated_task();

    EXPECT_TRUE(has_equality_predicate(translated.get_domain()));
    EXPECT_EQ(count_equality_literals(translated.get_data().initial_literals, translated.get_context()),
              count_unique_object_names(translation.get_translated_domain(), translated));
    EXPECT_EQ(&translated.get_domain().get_context().get_root(), &translation.get_translated_domain().get_context().get_root());
}

TEST(Loki2SemanticTranslator, SimplifiesComplexTaskGoalsWithTaskAxioms)
{
    const auto domain_source = std::string {
        "(define (domain goal)"
        "(:requirements :disjunctive-preconditions)"
        "(:predicates (p) (q))"
        ")" };
    const auto task_source = std::string {
        "(define (problem goal-problem) (:domain goal)"
        "(:init)"
        "(:goal (or (p) (q)))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto domain_translation = semantic::translate(domain);
    const auto original_translated_domain_axioms = domain_translation.get_translated_domain().get_data().axioms.size();
    const auto task = parser.parse_task(task_source);
    const auto translated_result = semantic::translate(task, domain_translation);
    const auto translated = translated_result.get_translated_task();
    const auto& repository = translated.get_context();

    ASSERT_TRUE(translated.get_data().goal.has_value());
    EXPECT_FALSE(contains_not_or_imply(*translated.get_data().goal, repository));
    EXPECT_EQ(translated.get_data().axioms.size(), 1);
    EXPECT_EQ(domain_translation.get_translated_domain().get_data().axioms.size(), original_translated_domain_axioms);
    EXPECT_GT(translated.get_domain().get_data().predicates.size(), domain.get_data().predicates.size());
}


TEST(Loki2SemanticParser, StrictModeRejectsUndefinedPredicates)
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
    auto parser = semantic::Parser(options);

    try
    {
        parser.parse_domain(domain);
        FAIL() << "Expected UndefinedPredicateError";
    }
    catch (const semantic::SemanticError& error)
    {
        EXPECT_EQ(error.code(), semantic::SemanticErrorCode::UndefinedPredicate);
        ASSERT_TRUE(error.source_range().has_value());
        EXPECT_EQ(error.source_range()->begin.line, 6);
        EXPECT_EQ(error.source_range()->begin.column, 20);
    }
}

TEST(Loki2SemanticParser, ReportsDuplicatePredicateDefinitions)
{
    const auto domain = std::string { R"(
(define (domain duplicate-predicate)
  (:predicates (p) (p))
)
)" };

    auto parser = semantic::Parser();
    try
    {
        parser.parse_domain(domain);
        FAIL() << "Expected duplicate predicate error";
    }
    catch (const semantic::SemanticError& error)
    {
        EXPECT_EQ(error.code(), semantic::SemanticErrorCode::DuplicatePredicate);
    }
}

TEST(Loki2SemanticParser, ReportsDeclaredPredicateArityMismatch)
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

    auto parser = semantic::Parser();
    try
    {
        parser.parse_domain(domain);
        FAIL() << "Expected arity mismatch";
    }
    catch (const semantic::SemanticError& error)
    {
        EXPECT_EQ(error.code(), semantic::SemanticErrorCode::ArityMismatch);
    }
}

TEST(Loki2SemanticParser, PermissiveModeKeepsImplicitPredicateCompatibility)
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

    auto parser = semantic::Parser();
    EXPECT_NO_THROW(parser.parse_domain(domain));
}

TEST(Loki2SemanticParser, ParsesAndTranslatesAllSuiteCases)
{
    const auto cases = semantic_suite::load_cases();
    ASSERT_FALSE(cases.empty());

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        try
        {
            semantic::Parser parser;
            const auto domain = parser.parse_domain_file(item.domain_file);
            const auto translation = semantic::translate(domain);
            const auto task = parser.parse_task_file(item.task_file);
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

TEST(Loki2SemanticParser, ParsesAllSuiteCasesWithContiguousTopLevelIndices)
{
    const auto cases = semantic_suite::load_cases();
    ASSERT_FALSE(cases.empty());

    for (const auto& item : cases)
    {
        SCOPED_TRACE(item.name);
        try
        {
            semantic::Parser parser;
            const auto domain = parser.parse_domain_file(item.domain_file);
            const auto task = parser.parse_task_file(item.task_file);
            EXPECT_EQ(task.get_domain().get_index(), domain.get_index());
            expect_contiguous_domain_indices(domain);
            expect_contiguous_task_indices(task);
        }
        catch (const std::exception& e)
        {
            ADD_FAILURE() << item.domain_file << " / " << item.task_file << ": " << e.what();
        }
    }
}

TEST(Loki2SemanticParser, ParsesDomainAndManyTasks)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests" / "classical" / "gripper";

    semantic::Parser parser;
    const auto domain = parser.parse_domain_file(root / "domain.pddl");
    EXPECT_EQ(std::string(domain.get_name()), "gripper-strips");
    EXPECT_GT(domain.get_actions().get_data().size(), 0);

    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    EXPECT_EQ(std::string(translated_domain.get_name()), "gripper-strips");
    EXPECT_NE(translated_domain.get_context().get_index(), domain.get_context().get_index());
    EXPECT_EQ(semantic::domain_translation_steps().size(), 9);
    EXPECT_EQ(semantic::task_translation_steps().size(), 10);
    const auto task1 = parser.parse_task_file(root / "test-1.pddl");
    const auto translated1_result = semantic::translate(task1, translation);
    const auto translated1 = translated1_result.get_translated_task();
    EXPECT_EQ(std::string(translated1.get_domain().get_name()), "gripper-strips");
    EXPECT_NE(translated1.get_context().get_index(), translated_domain.get_context().get_index());
    EXPECT_EQ(&translated1.get_domain().get_context().get_root(), &translated_domain.get_context().get_root());
    EXPECT_GT(translated1.get_initial_literals().get_data().size(), 0);

    const auto task2 = parser.parse_task_file(root / "test-1.pddl");
    const auto translated2_result = semantic::translate(task2, translation);
    const auto translated2 = translated2_result.get_translated_task();
    EXPECT_EQ(std::string(translated2.get_domain().get_name()), "gripper-strips");
    EXPECT_NE(translated2.get_context().get_index(), translated_domain.get_context().get_index());
    EXPECT_EQ(&translated2.get_domain().get_context().get_root(), &translated_domain.get_context().get_root());
    EXPECT_GT(translated2.get_initial_literals().get_data().size(), 0);
}

TEST(Loki2SemanticParser, ParsesAndTranslatesDistinctTasksAfterOneDomain)
{
    const auto domain_source = std::string {
        "(define (domain many)"
        "(:predicates (p ?x))"
        ")" };
    const auto first_task_source = std::string {
        "(define (problem first) (:domain many)"
        "(:objects a)"
        "(:init (p a))"
        "(:goal (p a))"
        ")" };
    const auto second_task_source = std::string {
        "(define (problem second) (:domain many)"
        "(:objects b)"
        "(:init (p b))"
        "(:goal (p b))"
        ")" };

    semantic::Parser parser;
    const auto domain = parser.parse_domain(domain_source);
    const auto translation = semantic::translate(domain);

    const auto first_task = parser.parse_task(first_task_source);
    const auto first_translated_result = semantic::translate(first_task, translation);
    const auto first_translated = first_translated_result.get_translated_task();
    const auto second_task = parser.parse_task(second_task_source);
    const auto second_translated_result = semantic::translate(second_task, translation);
    const auto second_translated = second_translated_result.get_translated_task();

    auto has_object = [](pddl::TaskView task, const std::string& name)
    {
        const auto& repository = task.get_context();
        for (auto object : task.get_data().objects)
            if (std::string(repository[object].name) == name)
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
