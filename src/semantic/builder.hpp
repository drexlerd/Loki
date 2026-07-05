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

#ifndef LOKI_SEMANTIC_BUILDER_HPP_
#define LOKI_SEMANTIC_BUILDER_HPP_

#include "checks.hpp"
#include "context.hpp"
#include "diagnostics.hpp"
#include "loki/ast.hpp"
#include "loki/formalism/formalism.hpp"
#include "loki/parser/options.hpp"

#include <cista/containers/optional.h>
#include <cstddef>
#include <string>
#include <vector>
#include <yggdrasil/containers/associative_containers.hpp>

namespace loki::semantic
{

// Translates a syntactic AST into (un-canonicalized) formalism entities inside the
// repository of domain_context.storage. Storage lifecycle, canonicalization, and
// scope handling across task parses belong to the caller.
class AstBuilder
{
public:
    AstBuilder(const parser::ParserOptions& options, const DiagnosticContext& diagnostics, DomainContext& domain_context, ParseContext& parse_context);

    formalism::DomainView build_domain(const ast::Domain& domain);
    formalism::TaskView build_task(const ast::Task& task);

private:
    const parser::ParserOptions& m_options;
    const DiagnosticContext& m_diagnostics;
    DomainContext& m_domain_context;
    ParseContext& m_parse_context;

    formalism::Repository& repo() noexcept { return m_domain_context.storage->repository; }
    SemanticChecks checks() const { return SemanticChecks { m_options, m_diagnostics, m_domain_context, m_parse_context }; }

    // Symbol lookup with auto-declaration in non-strict mode.
    formalism::PredicateView predicate(const ast::Identifier& identifier, size_t arity);
    formalism::PredicateView equality_predicate(const ast::Identifier& identifier, size_t arity);
    formalism::FunctionSkeletonView function(const ast::Identifier& identifier, size_t arity);

    // Domain and task sections.
    ygg::IndexList<formalism::Requirement> parse_requirements(const std::vector<ast::Requirement>& nodes);
    ygg::IndexList<formalism::Type> parse_types(const std::vector<ast::TypedName>& nodes);
    ygg::IndexList<formalism::Type> parse_type_expression(const ast::TypeExpression& type);
    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::TypeReference& node);
    ygg::IndexList<formalism::Type> parse_type_expression_node(const ast::EitherType& node);
    ygg::IndexList<formalism::Object> parse_objects(const std::vector<ast::TypedName>& nodes, ygg::UnorderedMap<std::string, formalism::ObjectView>& table);
    ygg::IndexList<formalism::Parameter> parse_parameters(const std::vector<ast::TypedVariable>& nodes);
    ygg::IndexList<formalism::Predicate> parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes);
    ygg::IndexList<formalism::FunctionSkeleton> parse_functions(const std::vector<ast::FunctionDeclaration>& nodes);

    // Terms, atoms, and literals.
    ygg::Index<formalism::Term> parse_term(const ast::Term& node);
    ygg::IndexList<formalism::Term> parse_terms(const std::vector<ast::Term>& nodes);
    ygg::Index<formalism::Atom> parse_atom(const ast::Atom& node);
    ygg::Index<formalism::Literal> parse_literal(const ast::Literal& node);

    // Conditions.
    ygg::Index<formalism::Condition> parse_condition(const ast::Condition& condition);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Data<formalism::Condition>::Variant value);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionLiteral& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionAnd& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionOr& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNot& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionImply& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionExists& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionForall& node);
    ygg::Index<formalism::Condition> parse_condition_node(const ast::ConditionNumericConstraint& node);

    // Numeric expressions.
    ygg::Index<formalism::FunctionTerm> parse_function_term(const ast::FunctionTerm& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression(const ast::FunctionExpression& expression);
    ygg::Index<formalism::FunctionExpression> wrap_function_expression(ygg::Data<formalism::FunctionExpression>::Variant value);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionNumber& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionFunction& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionUnary& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionBinary& node);
    ygg::Index<formalism::FunctionExpression> parse_function_expression_node(const ast::FunctionExpressionMulti& node);

    // Effects and top-level constructs.
    ygg::Index<formalism::Effect> parse_effect(const ast::Effect& effect);
    ygg::Index<formalism::Effect> wrap_effect(ygg::Data<formalism::Effect>::Variant value);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectLiteral& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectAnd& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectNumeric& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectForall& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectWhen& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectOneOf& node);
    ygg::Index<formalism::Effect> parse_effect_node(const ast::EffectProbabilistic& node);
    ygg::Index<formalism::Action> parse_action(const ast::Action& node);
    ygg::Index<formalism::Axiom> parse_axiom(const ast::Axiom& node);
    void parse_initial_element(const ast::Literal& literal, ygg::IndexList<formalism::Literal>& literals, ygg::IndexList<formalism::InitialFunctionValue>&);
    void
    parse_initial_element(const ast::InitialFunctionValue& value, ygg::IndexList<formalism::Literal>&, ygg::IndexList<formalism::InitialFunctionValue>& values);
    ygg::Index<formalism::Metric> parse_metric(const ast::Metric& node);

    // Action-costs completion.
    formalism::FunctionSkeletonView total_cost_function();
    ygg::Index<formalism::FunctionTerm> total_cost_term();
    bool writes_total_cost(formalism::EffectView effect);
    ygg::Index<formalism::Action> add_unit_cost(ygg::Index<formalism::Action> action);
    bool has_total_cost_initial_value(const ygg::IndexList<formalism::InitialFunctionValue>& values);
    void complete_action_costs(const ast::Task& task,
                               ygg::IndexList<formalism::InitialFunctionValue>& initial_function_values,
                               cista::optional<ygg::Index<formalism::Metric>>& metric);
};

}  // namespace loki::semantic

#endif
