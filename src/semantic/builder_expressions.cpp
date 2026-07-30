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

#include "builder.hpp"
#include "checks.hpp"
#include "context.hpp"
#include "diagnostics.hpp"
#include "loki/ast/ast.hpp"
#include "loki/formalism/builder.hpp"
#include "mappings.hpp"

#include <utility>

namespace loki::semantic
{

formalism::TermView AstBuilder::parse_term(const ast::Term& node)
{
    if (node.variable)
        return formalism::get_or_create<formalism::Term>(
            repo(),
            ygg::Data<formalism::Term>::Variant(lookup_variable(m_parse_context, m_diagnostics, node.name).get_index()));
    return formalism::get_or_create<formalism::Term>(
        repo(),
        ygg::Data<formalism::Term>::Variant(lookup_object(m_domain_context, m_parse_context, m_diagnostics, node.name).get_index()));
}

std::vector<formalism::TermView> AstBuilder::parse_terms(const std::vector<ast::Term>& nodes)
{
    auto result = std::vector<formalism::TermView> {};
    for (const auto& node : nodes)
        result.push_back(parse_term(node));
    return result;
}

formalism::AtomView AstBuilder::parse_atom(const ast::Atom& node)
{
    const auto name = key(node.predicate.text);
    auto pred = name == "=" ? equality_predicate(node.predicate, node.terms.size()) : predicate(node.predicate, node.terms.size());
    if (m_domain_context.declared_predicates.contains(name))
        checks().check_argument_types(name, pred.get_parameters(), node.terms, node.predicate);
    auto terms = parse_terms(node.terms);
    return formalism::get_or_create<formalism::Atom>(repo(), pred.get_index(), to_index_list(terms));
}

formalism::LiteralView AstBuilder::parse_literal(const ast::Literal& node)
{
    checks().mark_requirement_used(formalism::RequirementKind::Strips);
    return formalism::get_or_create<formalism::Literal>(repo(), parse_atom(node.atom).get_index(), node.positive);
}

formalism::ConditionView AstBuilder::parse_condition(const ast::Condition& condition)
{
    return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
}

formalism::ConditionView AstBuilder::wrap_condition(ygg::Data<formalism::Condition>::Variant value)
{
    return formalism::get_or_create<formalism::Condition>(repo(), std::move(value));
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionLiteral& node)
{
    return wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(repo(), parse_literal(node.literal).get_index()).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionAnd& node)
{
    auto list = std::vector<formalism::ConditionView> {};
    for (const auto& child : node.conditions)
        list.push_back(parse_condition(child.get()));
    return wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(repo(), to_index_list(list)).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionOr& node)
{
    checks().require_requirement(formalism::RequirementKind::DisjunctivePreconditions, node);
    auto list = std::vector<formalism::ConditionView> {};
    for (const auto& child : node.conditions)
        list.push_back(parse_condition(child.get()));
    return wrap_condition(formalism::get_or_create<formalism::ConditionOr>(repo(), to_index_list(list)).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionNot& node)
{
    checks().require_requirement(formalism::RequirementKind::NegativePreconditions, node);
    return wrap_condition(formalism::get_or_create<formalism::ConditionNot>(repo(), parse_condition(node.condition.get()).get_index()).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionImply& node)
{
    checks().require_requirement(formalism::RequirementKind::DisjunctivePreconditions, node);
    return wrap_condition(
        formalism::get_or_create<formalism::ConditionImply>(repo(), parse_condition(node.left.get()).get_index(), parse_condition(node.right.get()).get_index())
            .get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionExists& node)
{
    checks().require_requirement(formalism::RequirementKind::ExistentialPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_condition(node.condition.get());
    return wrap_condition(formalism::get_or_create<formalism::ConditionExists>(repo(), to_index_list(parameters), child.get_index()).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionForall& node)
{
    checks().require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_condition(node.condition.get());
    return wrap_condition(formalism::get_or_create<formalism::ConditionForall>(repo(), to_index_list(parameters), child.get_index()).get_index());
}

formalism::ConditionView AstBuilder::parse_condition_node(const ast::ConditionNumericConstraint& node)
{
    checks().require_requirement(formalism::RequirementKind::NumericFluents, node);
    return wrap_condition(formalism::get_or_create<formalism::ConditionNumericConstraint>(repo(),
                                                                                          comparator(node, m_diagnostics),
                                                                                          parse_function_expression(node.left.get()).get_index(),
                                                                                          parse_function_expression(node.right.get()).get_index())
                              .get_index());
}

formalism::FunctionTermView AstBuilder::parse_function_term(const ast::FunctionTerm& node)
{
    const auto name = key(node.function.text);
    if (m_parse_context.active_action_costs && name == "total-cost")
        checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
    else
        checks().require_requirement(formalism::RequirementKind::NumericFluents, node.function);
    auto skeleton = function(node.function, node.terms.size());
    if (m_domain_context.declared_functions.contains(name))
        checks().check_argument_types(name, skeleton.get_parameters(), node.terms, node.function);
    auto terms = parse_terms(node.terms);
    return formalism::get_or_create<formalism::FunctionTerm>(repo(), skeleton.get_index(), to_index_list(terms));
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression(const ast::FunctionExpression& expression)
{
    return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
}

formalism::FunctionExpressionView AstBuilder::wrap_function_expression(ygg::Data<formalism::FunctionExpression>::Variant value)
{
    return formalism::get_or_create<formalism::FunctionExpression>(repo(), std::move(value));
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression_node(const ast::FunctionExpressionNumber& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), node.value).get_index());
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression_node(const ast::FunctionExpressionFunction& node)
{
    return wrap_function_expression(parse_function_term(node.term).get_index());
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression_node(const ast::FunctionExpressionUnary& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::UnaryFunctionExpression>(repo(),
                                                                                                 formalism::UnaryArithmeticOperator::Minus,
                                                                                                 parse_function_expression(node.expression.get()).get_index())
                                        .get_index());
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression_node(const ast::FunctionExpressionBinary& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::BinaryFunctionExpression>(repo(),
                                                                                                  binary_operator(node.op),
                                                                                                  parse_function_expression(node.left.get()).get_index(),
                                                                                                  parse_function_expression(node.right.get()).get_index())
                                        .get_index());
}

formalism::FunctionExpressionView AstBuilder::parse_function_expression_node(const ast::FunctionExpressionMulti& node)
{
    auto expressions = std::vector<formalism::FunctionExpressionView> {};
    for (const auto& child : node.expressions)
        expressions.push_back(parse_function_expression(child.get()));
    return wrap_function_expression(
        formalism::get_or_create<formalism::MultiFunctionExpression>(repo(), multi_operator(node.op), to_index_list(expressions)).get_index());
}

}  // namespace loki::semantic
