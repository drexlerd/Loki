/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <gtest/gtest.h>

#include "../benchmark_utils.hpp"

#include <loki/semantic.hpp>

#include <yggdrasil/serialization/json_suite.hpp>

#include <algorithm>
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

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


} // namespace semantic_suite


template<class... Ts>
struct Overloaded : Ts...
{
    using Ts::operator()...;
};
template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

template<typename T>
void expect_contiguous_indices(const ygg::IndexList<T>& indices, const std::string& label, const formalism::Repository& repository, bool require_order = true)
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

void expect_contiguous_domain_indices(formalism::DomainView domain)
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

void expect_contiguous_task_indices(formalism::TaskView task)
{
    const auto& data = task.get_data();
    expect_contiguous_indices(data.requirements, "task.requirements", task.get_context());
    expect_contiguous_indices(data.objects, "task.objects", task.get_context());
    expect_contiguous_indices(data.initial_literals, "task.initial_literals", task.get_context());
    expect_contiguous_indices(data.initial_function_values, "task.initial_function_values", task.get_context());
    expect_contiguous_indices(data.axioms, "task.axioms", task.get_context());
}

bool contains_not_or_imply(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::ConditionLiteral>) { return false; },
            [&](ygg::Index<formalism::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<formalism::ConditionNot>) { return true; },
            [&](ygg::Index<formalism::ConditionImply>) { return true; },
            [&](ygg::Index<formalism::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_not_or_imply(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_not_or_imply(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionExists> node) { return contains_not_or_imply(repository[node].condition, repository); },
            [&](ygg::Index<formalism::ConditionForall> node) { return contains_not_or_imply(repository[node].condition, repository); },
        },
        repository[condition].value);
}

bool contains_forall(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::ConditionLiteral>) { return false; },
            [&](ygg::Index<formalism::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<formalism::ConditionForall>) { return true; },
            [&](ygg::Index<formalism::ConditionNot> node) { return contains_forall(repository[node].condition, repository); },
            [&](ygg::Index<formalism::ConditionImply> node) { return contains_forall(repository[node].left, repository) || contains_forall(repository[node].right, repository); },
            [&](ygg::Index<formalism::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_forall(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_forall(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionExists> node) { return contains_forall(repository[node].condition, repository); },
        },
        repository[condition].value);
}

bool contains_exists(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::ConditionLiteral>) { return false; },
            [&](ygg::Index<formalism::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<formalism::ConditionExists>) { return true; },
            [&](ygg::Index<formalism::ConditionNot> node) { return contains_exists(repository[node].condition, repository); },
            [&](ygg::Index<formalism::ConditionImply> node) { return contains_exists(repository[node].left, repository) || contains_exists(repository[node].right, repository); },
            [&](ygg::Index<formalism::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_exists(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (contains_exists(child, repository))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionForall> node) { return contains_exists(repository[node].condition, repository); },
        },
        repository[condition].value);
}




std::size_t count_effect_when(ygg::Index<formalism::Effect> effect, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::EffectLiteral>) -> std::size_t { return 0; },
            [&](ygg::Index<formalism::EffectNumeric>) -> std::size_t { return 0; },
            [&](ygg::Index<formalism::EffectOneOf> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : repository[node].effects) result += count_effect_when(child, repository);
                return result;
            },
            [&](ygg::Index<formalism::EffectProbabilistic> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto alternative : repository[node].alternatives) result += count_effect_when(repository[alternative].effect, repository);
                return result;
            },
            [&](ygg::Index<formalism::EffectAnd> node) -> std::size_t
            {
                std::size_t result = 0;
                for (auto child : repository[node].effects) result += count_effect_when(child, repository);
                return result;
            },
            [&](ygg::Index<formalism::EffectForall> node) { return count_effect_when(repository[node].effect, repository); },
            [&](ygg::Index<formalism::EffectWhen> node) { return std::size_t { 1 } + count_effect_when(repository[node].effect, repository); },
        },
        repository[effect].value);
}

bool is_effect_and(ygg::Index<formalism::Effect> effect, const formalism::Repository& repository)
{
    return std::visit(Overloaded { [&](ygg::Index<formalism::EffectAnd>) { return true; }, [&](auto) { return false; } }, repository[effect].value);
}

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
        if (!literal.is_positive())
            continue;
        const auto atom = literal.get_atom();
        if (std::string(atom.get_predicate().get_name()) != "=" || atom.get_num_terms() != 2)
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

bool has_requirement_kind(const ygg::IndexList<formalism::Requirement>& requirements, formalism::RequirementKind kind, const formalism::Repository& repository)
{
    for (auto requirement : requirements)
        if (repository[requirement].kind == kind)
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
        if (!literal.is_positive())
            continue;
        const auto atom = literal.get_atom();
        if (atom.get_num_terms() != 1 || std::string(atom.get_predicate().get_name()) != predicate_name)
            continue;
        const auto object = object_term_name(atom.get_terms()[0]);
        if (object && *object == object_name)
            return true;
    }
    return false;
}

bool condition_mentions_predicate(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository, const std::string& name)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::ConditionLiteral> node)
            {
                const auto literal = repository[node].literal;
                const auto atom = repository[literal].atom;
                const auto predicate = repository[atom].predicate;
                return std::string(repository[predicate].name) == name;
            },
            [&](ygg::Index<formalism::ConditionNumericConstraint>) { return false; },
            [&](ygg::Index<formalism::ConditionNot> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
            [&](ygg::Index<formalism::ConditionImply> node) {
                return condition_mentions_predicate(repository[node].left, repository, name)
                       || condition_mentions_predicate(repository[node].right, repository, name);
            },
            [&](ygg::Index<formalism::ConditionAnd> node)
            {
                for (auto child : repository[node].conditions)
                    if (condition_mentions_predicate(child, repository, name))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionOr> node)
            {
                for (auto child : repository[node].conditions)
                    if (condition_mentions_predicate(child, repository, name))
                        return true;
                return false;
            },
            [&](ygg::Index<formalism::ConditionExists> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
            [&](ygg::Index<formalism::ConditionForall> node) { return condition_mentions_predicate(repository[node].condition, repository, name); },
        },
        repository[condition].value);
}

std::optional<std::string> variable_term_name(ygg::Index<formalism::Term> term, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::Variable> variable) -> std::optional<std::string> { return std::string(repository[variable].name); },
            [&](auto) -> std::optional<std::string> { return std::nullopt; },
        },
        repository[term].value);
}

bool has_top_level_effect_literal_with_terms(ygg::Index<formalism::Effect> effect,
                                             const formalism::Repository& repository,
                                             const std::string& predicate_name,
                                             const std::string& first_variable,
                                             const std::string& second_variable)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::EffectLiteral> node)
            {
                const auto literal = repository[node].literal;
                const auto atom = repository[repository[literal].atom];
                const auto predicate = atom.predicate;
                if (std::string(repository[predicate].name) != predicate_name || atom.terms.size() != 2)
                    return false;
                const auto first = variable_term_name(atom.terms[0], repository);
                const auto second = variable_term_name(atom.terms[1], repository);
                return first && second && *first == first_variable && *second == second_variable;
            },
            [&](ygg::Index<formalism::EffectAnd> node)
            {
                for (auto child : repository[node].effects)
                {
                    if (has_top_level_effect_literal_with_terms(child, repository, predicate_name, first_variable, second_variable))
                        return true;
                }
                return false;
            },
            [&](auto) { return false; },
        },
        repository[effect].value);
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
    auto names = std::unordered_set<std::string> {};
    for (auto object : domain.get_constants())
        names.insert(std::string(object.get_name()));
    for (auto object : task.get_objects())
        names.insert(std::string(object.get_name()));
    return names.size();
}

std::optional<std::string> first_exists_parameter_name(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit(
        Overloaded {
            [&](ygg::Index<formalism::ConditionLiteral>) -> std::optional<std::string> { return std::nullopt; },
            [&](ygg::Index<formalism::ConditionNumericConstraint>) -> std::optional<std::string> { return std::nullopt; },
            [&](ygg::Index<formalism::ConditionNot> node) { return first_exists_parameter_name(repository[node].condition, repository); },
            [&](ygg::Index<formalism::ConditionImply> node) -> std::optional<std::string>
            {
                if (auto result = first_exists_parameter_name(repository[node].left, repository))
                    return result;
                return first_exists_parameter_name(repository[node].right, repository);
            },
            [&](ygg::Index<formalism::ConditionAnd> node) -> std::optional<std::string>
            {
                for (auto child : repository[node].conditions)
                    if (auto result = first_exists_parameter_name(child, repository))
                        return result;
                return std::nullopt;
            },
            [&](ygg::Index<formalism::ConditionOr> node) -> std::optional<std::string>
            {
                for (auto child : repository[node].conditions)
                    if (auto result = first_exists_parameter_name(child, repository))
                        return result;
                return std::nullopt;
            },
            [&](ygg::Index<formalism::ConditionExists> node) -> std::optional<std::string>
            {
                const auto& exists = repository[node];
                if (exists.parameters.empty())
                    return std::nullopt;
                const auto variable = repository[exists.parameters.front()].variable;
                return std::string(repository[variable].name);
            },
            [&](ygg::Index<formalism::ConditionForall> node) { return first_exists_parameter_name(repository[node].condition, repository); },
        },
        repository[condition].value);
}

TEST(LokiCanonicalization, SortsSemanticFreeListsLexicographicallyBeforeInterning)
{
    auto repository = formalism::Repository(0);
    const auto p = formalism::get_or_create<formalism::Predicate>(repository, cista::offset::string("p"), ygg::IndexList<formalism::Parameter> {}).get_index();
    const auto q = formalism::get_or_create<formalism::Predicate>(repository, cista::offset::string("q"), ygg::IndexList<formalism::Parameter> {}).get_index();
    const auto p_atom = formalism::get_or_create<formalism::Atom>(repository, p, ygg::IndexList<formalism::Term> {}).get_index();
    const auto q_atom = formalism::get_or_create<formalism::Atom>(repository, q, ygg::IndexList<formalism::Term> {}).get_index();
    const auto p_literal = formalism::get_or_create<formalism::Literal>(repository, true, p_atom).get_index();
    const auto q_literal = formalism::get_or_create<formalism::Literal>(repository, true, q_atom).get_index();
    const auto p_condition = formalism::get_or_create<formalism::Condition>(repository, ygg::Data<formalism::Condition>::Variant(formalism::get_or_create<formalism::ConditionLiteral>(repository, p_literal).get_index())).get_index();
    const auto q_condition = formalism::get_or_create<formalism::Condition>(repository, ygg::Data<formalism::Condition>::Variant(formalism::get_or_create<formalism::ConditionLiteral>(repository, q_literal).get_index())).get_index();

    auto first_conditions = ygg::IndexList<formalism::Condition> {};
    first_conditions.push_back(q_condition);
    first_conditions.push_back(p_condition);
    const auto first = formalism::get_or_create<formalism::ConditionAnd>(repository, std::move(first_conditions)).get_index();

    auto second_conditions = ygg::IndexList<formalism::Condition> {};
    second_conditions.push_back(p_condition);
    second_conditions.push_back(q_condition);
    const auto second = formalism::get_or_create<formalism::ConditionAnd>(repository, std::move(second_conditions)).get_index();

    EXPECT_EQ(first, second);
    ASSERT_EQ(repository[first].conditions.size(), 2);
    EXPECT_EQ(repository[first].conditions[0], p_condition);
    EXPECT_EQ(repository[first].conditions[1], q_condition);

    auto first_predicates = ygg::IndexList<formalism::Predicate> {};
    first_predicates.push_back(q);
    first_predicates.push_back(p);
    const auto first_domain = formalism::get_or_create<formalism::Domain>(repository, ygg::Data<formalism::Domain>(cista::offset::string("d"), ygg::IndexList<formalism::Requirement> {}, ygg::IndexList<formalism::Type> {}, ygg::IndexList<formalism::Object> {}, std::move(first_predicates), ygg::IndexList<formalism::FunctionSkeleton> {}, ygg::IndexList<formalism::Action> {}, ygg::IndexList<formalism::Axiom> {})).get_index();

    auto second_predicates = ygg::IndexList<formalism::Predicate> {};
    second_predicates.push_back(p);
    second_predicates.push_back(q);
    const auto second_domain = formalism::get_or_create<formalism::Domain>(repository, ygg::Data<formalism::Domain>(cista::offset::string("d"), ygg::IndexList<formalism::Requirement> {}, ygg::IndexList<formalism::Type> {}, ygg::IndexList<formalism::Object> {}, std::move(second_predicates), ygg::IndexList<formalism::FunctionSkeleton> {}, ygg::IndexList<formalism::Action> {}, ygg::IndexList<formalism::Axiom> {})).get_index();

    EXPECT_EQ(first_domain, second_domain);
    ASSERT_EQ(repository[first_domain].predicates.size(), 2);
    EXPECT_EQ(repository[first_domain].predicates[0], p);
    EXPECT_EQ(repository[first_domain].predicates[1], q);
}

TEST(LokiSemanticTranslator, RewritesConditionsToNegationNormalForm)
{
    const auto domain_source = std::string {
        "(define (domain nnf)"
        "(:predicates (p) (q) (r))"
        "(:action a :parameters () "
        ":precondition (imply (not (and (p) (q))) (r)) "
        ":effect (and (p)))"
        ")" };

    semantic::Parser parser(domain_source);


    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& translated_repository = translated_domain.get_context();
    ASSERT_GT(translated_domain.get_num_actions(), 0);
    for (auto action : translated_domain.get_actions())
    {
        ASSERT_TRUE(action.has_precondition());
        EXPECT_FALSE(contains_not_or_imply(action.get_precondition().value().get_index(), translated_repository));
    }
}


TEST(LokiSemanticTranslator, RenamesQuantifiedVariablesDeterministically)
{
    const auto domain_source = std::string {
        "(define (domain rename)"
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
    const auto& repository = translated_domain.get_context();
    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_num_parameters(), 2);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "x_0");
    EXPECT_EQ(std::string(action.get_parameters()[1].get_variable().get_name()), "x_1");
    ASSERT_TRUE(action.has_precondition());
    EXPECT_FALSE(contains_exists(action.get_precondition().value().get_index(), repository));
}


TEST(LokiSemanticTranslator, RenamesBeforeNegationNormalFormOnlyOnce)
{
    const auto domain_source = std::string {
        "(define (domain rename-once)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (not (exists (?x - object) (p ?x ?x))) "
        ":effect (and))"
        ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    ASSERT_EQ(translated_domain.get_num_axioms(), 1);
    const auto axiom = translated_domain.get_axioms()[0];
    ASSERT_EQ(axiom.get_num_parameters(), 1);
    EXPECT_EQ(std::string(axiom.get_parameters()[0].get_variable().get_name()), "x_1");

    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_EQ(action.get_num_parameters(), 1);
    EXPECT_EQ(std::string(action.get_parameters()[0].get_variable().get_name()), "x_0");
}

TEST(LokiSemanticTranslator, RemovesUniversalQuantifiersWithDerivedAxioms)
{
    const auto domain_source = std::string {
        "(define (domain universal)"
        "(:requirements :typing :universal-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (forall (?y - object) (p ?x ?y)) "
        ":effect (and))"
        ")" };

    semantic::Parser parser(domain_source);


    const auto domain = parser.get_domain();
    const auto original_axioms = domain.get_num_axioms();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    EXPECT_GT(translated_domain.get_num_axioms(), original_axioms);
    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_TRUE(action.has_precondition());
    EXPECT_FALSE(contains_forall(action.get_precondition().value().get_index(), repository));
}

TEST(LokiSemanticTranslator, LowersNegatedExistsWithoutNegatingInnerCondition)
{
    const auto domain_source = std::string {
        "(define (domain negated-exists)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (not (exists (?y - object) (p ?x ?y))) "
        ":effect (and))"
        ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();
    ASSERT_EQ(translated_domain.get_num_axioms(), 1);
    const auto axiom = translated_domain.get_axioms()[0];
    EXPECT_TRUE(condition_mentions_predicate(axiom.get_condition().get_index(), repository, "p"));
    EXPECT_TRUE(condition_mentions_predicate(axiom.get_condition().get_index(), repository, "object"));
    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto action = translated_domain.get_actions()[0];
    ASSERT_TRUE(action.has_precondition());
    EXPECT_FALSE(contains_exists(action.get_precondition().value().get_index(), repository));
}


TEST(LokiSemanticTranslator, GeneratedAxiomParametersMatchHeadPredicateArity)
{
    const auto domain_source = std::string {
        "(define (domain generated-axiom-arity)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        "(:action a :parameters (?x - object) "
        ":precondition (not (exists (?y - object) (p ?x ?y))) "
        ":effect (and))"
        ")" };

    semantic::Parser parser(domain_source);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    ASSERT_EQ(translated_domain.get_num_axioms(), 1);
    const auto& axiom = repository[translated_domain.get_data().axioms.front()];
    const auto& literal = repository[axiom.head];
    const auto& atom = repository[literal.atom];
    const auto& predicate = repository[atom.predicate];

    EXPECT_EQ(predicate.parameters.size(), atom.terms.size());
    EXPECT_GE(axiom.parameters.size(), atom.terms.size());
    EXPECT_FALSE(contains_exists(axiom.condition, repository));
}


TEST(LokiSemanticTranslator, GeneratesFreshAxiomsForIdenticalUniversalConditions)
{
    const auto domain_source = std::string {
        "(define (domain universal-cache)"
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
    const auto& repository = translated_domain.get_context();

    auto generated_predicates = std::size_t {};
    auto generated_parameter_types = std::unordered_set<std::string> {};
    for (auto predicate : translated_domain.get_data().predicates)
    {
        const auto& predicate_data = repository[predicate];
        if (!std::string(predicate_data.name).starts_with("_universal_"))
            continue;
        ++generated_predicates;
        ASSERT_EQ(predicate_data.parameters.size(), std::size_t { 1 });
        const auto& parameter = repository[predicate_data.parameters.front()];
        ASSERT_EQ(parameter.types.size(), std::size_t { 1 });
        generated_parameter_types.insert(std::string(repository[parameter.types.front()].name));
    }

    EXPECT_EQ(generated_predicates, 2);
    EXPECT_EQ(generated_parameter_types.size(), std::size_t { 2 });
    EXPECT_TRUE(generated_parameter_types.contains("left"));
    EXPECT_TRUE(generated_parameter_types.contains("right"));
    EXPECT_EQ(translated_domain.get_num_axioms(), 2);
    ASSERT_EQ(translated_domain.get_num_actions(), 2);
    for (auto action_index : translated_domain.get_data().actions)
    {
        const auto& action = repository[action_index];
        ASSERT_TRUE(action.precondition.has_value());
        EXPECT_FALSE(contains_forall(*action.precondition, repository));
    }
}


TEST(LokiSemanticTranslator, SplitsDisjunctiveActionPreconditionsAfterDnf)
{
    const auto domain_source = std::string {
        "(define (domain split)"
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
    const auto& repository = translated_domain.get_context();

    ASSERT_EQ(translated_domain.get_num_actions(), 2);
    for (auto action_index : translated_domain.get_data().actions)
    {
        const auto& action = repository[action_index];
        ASSERT_TRUE(action.precondition.has_value());
        EXPECT_FALSE(std::visit(Overloaded {
            [&](ygg::Index<formalism::ConditionOr>) { return true; },
            [&](auto) { return false; },
        }, repository[*action.precondition].value));
    }
}

TEST(LokiSemanticTranslator, MovesExistentialPreconditionVariablesToActionParameters)
{
    const auto domain_source = std::string {
        "(define (domain exists)"
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
    const auto& repository = translated_domain.get_context();
    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    EXPECT_EQ(action.parameters.size(), 2);
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_FALSE(contains_exists(*action.precondition, repository));
}

TEST(LokiSemanticTranslator, SplitsDisjunctiveWhenEffectsAndFlattensConjunctions)
{
    const auto domain_source = std::string {
        "(define (domain effects)"
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
    const auto& repository = translated_domain.get_context();

    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_TRUE(action.effect.has_value());
    EXPECT_TRUE(is_effect_and(*action.effect, repository));
    EXPECT_EQ(count_effect_when(*action.effect, repository), 4);
}


TEST(LokiSemanticTranslator, KeepsActionScopedEffectVariablesAfterQuantifierRenaming)
{
    const auto domain_path = fs::path(std::string(DATA_DIR)) / "planning-benchmarks/tests/classical/airport/domain.pddl";
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(domain_path);

    semantic::Parser parser(domain_path);

    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    auto found_takeoff = false;
    for (auto action_index : translated_domain.get_data().actions)
    {
        const auto& action = repository[action_index];
        if (std::string(action.name) != "takeoff")
            continue;

        found_takeoff = true;
        ASSERT_TRUE(action.effect.has_value());
        EXPECT_TRUE(has_top_level_effect_literal_with_terms(*action.effect, repository, "blocked", "s_0", "a_0"));
        EXPECT_FALSE(has_top_level_effect_literal_with_terms(*action.effect, repository, "blocked", "s_2", "a_0"));
    }
    EXPECT_TRUE(found_takeoff);
}

TEST(LokiSemanticTranslator, AddsTypePredicatesAndRemovesTypingByDefault)
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

    semantic::Parser parser(domain_source);


    const auto domain = parser.get_domain();
    const auto translation = semantic::translate(domain);
    const auto translated_domain = translation.get_translated_domain();
    const auto& repository = translated_domain.get_context();

    EXPECT_TRUE(has_predicate_named(translated_domain, "thing"));
    EXPECT_TRUE(has_predicate_named(translated_domain, "object"));
    EXPECT_FALSE(has_requirement_kind(translated_domain.get_data().requirements, formalism::RequirementKind::Typing, repository));
    EXPECT_TRUE(translated_domain.get_data().types.empty());
    ASSERT_EQ(translated_domain.get_num_actions(), 1);
    const auto& action = repository[translated_domain.get_data().actions.front()];
    ASSERT_EQ(action.parameters.size(), 1);
    EXPECT_TRUE(repository[action.parameters.front()].types.empty());
    ASSERT_TRUE(action.precondition.has_value());
    EXPECT_TRUE(condition_mentions_predicate(*action.precondition, repository, "thing"));

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

TEST(LokiSemanticTranslator, GeneratedGoalPredicateAvoidsExistingNames)
{
    const auto domain_source = std::string {
        "(define (domain goal-name-collision)"
        "(:requirements :disjunctive-preconditions)"
        "(:predicates (_goal_0) (p) (q))"
        ")" };
    const auto task_source = std::string {
        "(define (problem goal-name-collision-problem) (:domain goal-name-collision)"
        "(:init)"
        "(:goal (or (p) (q)))"
        ")" };

    semantic::Parser parser(domain_source);
    const auto translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(task_source), translation);
    const auto translated = translated_result.get_translated_task();
    const auto& repository = translated.get_context();

    EXPECT_TRUE(has_predicate_named(translation.get_translated_domain(), "_goal_0"));

    auto found_generated = false;
    for (auto predicate : translated.get_data().predicates)
        found_generated |= std::string(repository[predicate].name) == "_goal_1";

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
    const auto domain_source = std::string {
        "(define (domain task-only-equality)"
        "(:requirements :strips)"
        "(:predicates (p))"
        ")" };
    const auto task_source = std::string {
        "(define (problem task-only-equality-problem) (:domain task-only-equality)"
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

    semantic::Parser parser(domain_source);


    const auto domain = parser.get_domain();
    const auto domain_translation = semantic::translate(domain);
    const auto original_translated_domain_axioms = domain_translation.get_translated_domain().get_num_axioms();
    const auto task = parser.parse_task(task_source);
    const auto translated_result = semantic::translate(task, domain_translation);
    const auto translated = translated_result.get_translated_task();
    const auto& repository = translated.get_context();

    ASSERT_TRUE(translated.get_data().goal.has_value());
    EXPECT_FALSE(contains_not_or_imply(*translated.get_data().goal, repository));
    EXPECT_EQ(translated.get_num_axioms(), 2);
    for (const auto axiom : translated.get_data().axioms)
    {
        const auto condition = repository[axiom].condition;
        const auto is_disjunction = std::visit(
            Overloaded {
                [](ygg::Index<formalism::ConditionOr>) { return true; },
                [](auto) { return false; },
            },
            repository[condition].value);
        EXPECT_FALSE(is_disjunction);
    }
    EXPECT_EQ(domain_translation.get_translated_domain().get_num_axioms(), original_translated_domain_axioms);
    EXPECT_GT(translated.get_num_predicates(), 0);
    EXPECT_EQ(translated.get_domain().get_num_predicates(), domain_translation.get_translated_domain().get_num_predicates());
}




TEST(LokiSemanticTranslator, RenamesTaskGoalVariablesBeforeGoalSimplificationOnlyOnce)
{
    const auto domain_source = std::string {
        "(define (domain task-rename-once)"
        "(:requirements :typing :existential-preconditions)"
        "(:predicates (p ?x - object ?y - object))"
        ")" };
    const auto task_source = std::string {
        "(define (problem task-rename-once-problem) (:domain task-rename-once)"
        "(:objects o - object)"
        "(:init)"
        "(:goal (not (exists (?x - object) (p ?x ?x))))"
        ")" };

    semantic::Parser parser(domain_source);
    const auto domain_translation = semantic::translate(parser.get_domain());
    const auto translated_result = semantic::translate(parser.parse_task(task_source), domain_translation);
    const auto translated = translated_result.get_translated_task();
    const auto& repository = translated.get_context();

    ASSERT_EQ(translated.get_num_axioms(), 1);
    const auto& axiom = repository[translated.get_data().axioms.front()];
    ASSERT_EQ(axiom.parameters.size(), 1);
    const auto variable = repository[axiom.parameters.front()].variable;
    EXPECT_EQ(std::string(repository[variable].name), "x_0");
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
        EXPECT_NE(message.find("line 2, column"), std::string::npos);
        ASSERT_TRUE(error.has_source_range());
        ASSERT_TRUE(error.source_range().has_value());
        EXPECT_EQ(error.source_range()->begin.line, 2);
        EXPECT_EQ(error.source_range()->begin.line, error.source_range()->end.line);
        EXPECT_EQ(error.source_range()->begin.column, error.source_range()->end.column);
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

    EXPECT_EQ(parser.get_domain().get_num_actions(), 2);
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
        EXPECT_NE(std::string(error.what()).find(":disjunctive-preconditions"), std::string::npos);
        ASSERT_TRUE(error.has_source_range());
        ASSERT_TRUE(error.source_range().has_value());
        EXPECT_EQ(error.source_range()->begin.line, 6);
        EXPECT_EQ(error.source_range()->begin.column, 19);
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

TEST(LokiSemanticParser, ParsesDomainAndManyTasks)
{
    const auto root = fs::path(std::string(DATA_DIR)) / "planning-benchmarks" / "tests" / "classical" / "gripper";
    LOKI_SKIP_IF_BENCHMARK_FILE_UNAVAILABLE(root / "domain.pddl");
    LOKI_EXPECT_BENCHMARK_FILE_AVAILABLE(root / "test-1.pddl");

    semantic::Parser parser(root / "domain.pddl");


    const auto domain = parser.get_domain();
    EXPECT_EQ(std::string(domain.get_name()), "gripper-strips");
    EXPECT_GT(domain.get_num_actions(), 0);

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
    EXPECT_GT(translated1.get_num_initial_literals(), 0);

    const auto task2 = parser.parse_task(root / "test-1.pddl");
    const auto translated2_result = semantic::translate(task2, translation);
    const auto translated2 = translated2_result.get_translated_task();
    EXPECT_EQ(std::string(translated2.get_domain().get_name()), "gripper-strips");
    EXPECT_NE(translated2.get_context().get_index(), translated_domain.get_context().get_index());
    EXPECT_EQ(&translated2.get_domain().get_context().get_root(), &translated_domain.get_context().get_root());
    EXPECT_GT(translated2.get_num_initial_literals(), 0);
}

TEST(LokiSemanticParser, ParsesAndTranslatesDistinctTasksAfterOneDomain)
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
