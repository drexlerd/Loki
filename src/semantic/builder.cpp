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

#include "loki/semantic/errors.hpp"
#include "mappings.hpp"

#include <algorithm>
#include <cmath>
#include <type_traits>
#include <utility>

namespace loki::semantic
{

namespace
{
class VariableScope
{
public:
    explicit VariableScope(ParseContext& context) : m_context(context) { m_context.variable_scopes.emplace_back(); }
    ~VariableScope() { m_context.variable_scopes.pop_back(); }

    VariableScope(const VariableScope&) = delete;
    VariableScope& operator=(const VariableScope&) = delete;

private:
    ParseContext& m_context;
};
}  // namespace

AstBuilder::AstBuilder(const ParserOptions& options,
                       const DiagnosticContext& diagnostics,
                       formalism::Repository& repository,
                       DomainContext& domain_context,
                       ParseContext& parse_context) :
    m_options(options),
    m_diagnostics(diagnostics),
    m_repository(repository),
    m_domain_context(domain_context),
    m_parse_context(parse_context)
{
}

formalism::DomainView AstBuilder::build_domain(const ast::Domain& domain)
{
    auto requirements = parse_requirements(domain.requirements);
    // Genuine numeric domains keep their own cost structure; a planner interprets an absent
    // metric as unit costs, so grafting total-cost onto :numeric-fluents would be wrong.
    const auto inject_unit_costs = m_options.add_action_costs && !m_parse_context.active_action_costs && !m_parse_context.active_numeric_fluents;
    if (inject_unit_costs)
    {
        m_parse_context.active_action_costs = true;
        remember_requirement(m_parse_context, formalism::RequirementKind::ActionCosts);
        const auto requirement = formalism::get_or_create<formalism::Requirement>(repo(), formalism::RequirementKind::ActionCosts).get_index();
        if (std::find(requirements.begin(), requirements.end(), requirement) == requirements.end())
            requirements.push_back(requirement);
    }
    m_domain_context.requirement_kinds = m_parse_context.active_requirements;
    m_domain_context.action_costs = m_parse_context.active_action_costs;
    m_domain_context.numeric_fluents = m_parse_context.active_numeric_fluents;
    auto types = parse_types(domain.types);
    auto constants = parse_objects(domain.constants, m_domain_context.objects, m_domain_context.declared_objects);
    auto predicates = parse_predicates(domain.predicates);
    auto functions = parse_functions(domain.functions);
    if (m_parse_context.active_action_costs && !m_domain_context.declared_functions.contains("total-cost"))
    {
        if (m_options.strict && !m_options.add_action_costs)
            m_diagnostics.throw_at(domain.name, SemanticError("Missing total-cost function for :action-costs"));
        m_domain_context.declared_functions.insert("total-cost");
        functions.push_back(total_cost_function().get_index());
    }
    auto axioms = ygg::IndexList<formalism::Axiom> {};
    for (const auto& axiom : domain.axioms)
    {
        checks().require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
        axioms.push_back(parse_axiom(axiom));
    }
    auto actions = ygg::IndexList<formalism::Action> {};
    for (const auto& action : domain.actions)
        actions.push_back(parse_action(action));
    if (inject_unit_costs)
        for (auto& action : actions)
            action = add_unit_cost(action);
    checks().reject_unused_requirements(domain.requirements);

    auto all_types = std::vector<formalism::TypeView> {};
    for (const auto& [_, type] : m_domain_context.types)
        all_types.push_back(type);
    std::sort(all_types.begin(), all_types.end(), [](auto lhs, auto rhs) { return lhs.get_index() < rhs.get_index(); });

    types.clear();
    for (auto type : all_types)
        types.push_back(type.get_index());
    auto data = ygg::Data<formalism::Domain>(to_cista(domain.name.text),
                                             std::move(requirements),
                                             std::move(types),
                                             std::move(constants),
                                             std::move(predicates),
                                             std::move(functions),
                                             std::move(actions),
                                             std::move(axioms));
    return formalism::get_or_create<formalism::Domain>(repo(), std::move(data));
}

formalism::TaskView AstBuilder::build_task(const ast::Task& task)
{
    if (task.domain_name.text != m_domain_context.domain_name)
        m_diagnostics.throw_at(task.domain_name, MismatchedDomainError(m_domain_context.domain_name, task.domain_name.text));

    auto requirements = parse_requirements(task.requirements);
    auto objects = parse_objects(task.objects, m_parse_context.task_objects, m_parse_context.declared_objects);
    auto initial_literals = ygg::IndexList<formalism::Literal> {};
    auto initial_function_values = ygg::IndexList<formalism::InitialFunctionValue> {};
    for (const auto& element : task.initial)
        boost::apply_visitor([&](const auto& node) { parse_initial_element(node, initial_literals, initial_function_values); }, element);

    auto goal = cista::optional<ygg::Index<formalism::Condition>> {};
    if (task.goal)
        goal = parse_condition(*task.goal);

    auto metric = cista::optional<ygg::Index<formalism::Metric>> {};
    if (task.metric)
        metric = parse_metric(*task.metric);
    complete_action_costs(task, initial_function_values, metric);

    auto axioms = ygg::IndexList<formalism::Axiom> {};
    for (const auto& axiom : task.axioms)
    {
        checks().require_requirement(formalism::RequirementKind::DerivedPredicates, axiom);
        axioms.push_back(parse_axiom(axiom));
    }
    checks().reject_unused_requirements(task.requirements);

    auto data = ygg::Data<formalism::Task>(to_cista(task.name.text),
                                           m_domain_context.domain->get_index(),
                                           std::move(requirements),
                                           std::move(objects),
                                           std::move(initial_literals),
                                           std::move(initial_function_values),
                                           goal,
                                           metric,
                                           ygg::IndexList<formalism::Predicate> {},
                                           std::move(axioms));
    return formalism::get_or_create<formalism::Task>(repo(), std::move(data));
}

formalism::PredicateView AstBuilder::predicate(const ast::Identifier& identifier, size_t arity)
{
    const auto name = key(identifier.text);
    if (auto it = m_domain_context.predicates.find(name); it != m_domain_context.predicates.end())
    {
        if (m_domain_context.declared_predicates.contains(name))
            checks().ensure_arity(name, it->second.get_parameters().size(), arity, identifier);
        return it->second;
    }
    if (m_options.strict)
        m_diagnostics.throw_at(identifier, UndefinedPredicateError(name));
    auto view = formalism::get_or_create<formalism::Predicate>(repo(), to_cista(name), ygg::IndexList<formalism::Parameter> {});
    m_domain_context.predicates.emplace(name, view);
    return view;
}

formalism::PredicateView AstBuilder::equality_predicate(const ast::Identifier& identifier, size_t arity)
{
    checks().require_requirement(formalism::RequirementKind::Equality, identifier);
    if (arity != 2)
        m_diagnostics.throw_at(identifier, InvalidEqualityError("expected 2 terms, got " + std::to_string(arity)));

    const auto name = key(identifier.text);
    if (auto it = m_domain_context.predicates.find(name); it != m_domain_context.predicates.end())
    {
        if (m_domain_context.declared_predicates.contains(name))
            checks().ensure_arity(name, it->second.get_parameters().size(), arity, identifier);
        return it->second;
    }

    auto types = ygg::IndexList<formalism::Type> { m_domain_context.object_type.get_index() };
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    const auto left = formalism::get_or_create<formalism::Variable>(repo(), cista::offset::string("?lhs")).get_index();
    const auto right = formalism::get_or_create<formalism::Variable>(repo(), cista::offset::string("?rhs")).get_index();
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(repo(), left, types).get_index());
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(repo(), right, std::move(types)).get_index());
    auto view = formalism::get_or_create<formalism::Predicate>(repo(), cista::offset::string("="), std::move(parameters));
    m_domain_context.predicates.emplace(name, view);
    return view;
}

formalism::FunctionSkeletonView AstBuilder::function(const ast::Identifier& identifier, size_t arity)
{
    const auto name = key(identifier.text);
    if (auto it = m_domain_context.functions.find(name); it != m_domain_context.functions.end())
    {
        if (m_domain_context.declared_functions.contains(name))
            checks().ensure_arity(name, it->second.get_parameters().size(), arity, identifier);
        return it->second;
    }
    if (m_options.strict)
        m_diagnostics.throw_at(identifier, UndefinedFunctionError(name));
    auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(),
                                                                      to_cista(name),
                                                                      ygg::IndexList<formalism::Parameter> {},
                                                                      m_domain_context.number_type.get_index());
    m_domain_context.functions.emplace(name, view);
    return view;
}

ygg::IndexList<formalism::Requirement> AstBuilder::parse_requirements(const std::vector<ast::Requirement>& nodes)
{
    auto result = ygg::IndexList<formalism::Requirement> {};
    for (const auto& node : nodes)
    {
        const auto kind = requirement_kind(node, m_diagnostics);
        if (kind == formalism::RequirementKind::ActionCosts)
            m_parse_context.active_action_costs = true;
        if (kind == formalism::RequirementKind::Fluents || kind == formalism::RequirementKind::NumericFluents)
            m_parse_context.active_numeric_fluents = true;
        for (const auto capability : requirement_capabilities(node, m_diagnostics))
            m_parse_context.active_requirements.insert(capability);
        result.push_back(formalism::get_or_create<formalism::Requirement>(repo(), kind).get_index());
    }
    return result;
}

ygg::IndexList<formalism::Type> AstBuilder::parse_types(const std::vector<ast::TypedName>& nodes)
{
    auto result = ygg::IndexList<formalism::Type> {};
    if (!nodes.empty())
        checks().require_requirement(formalism::RequirementKind::Typing, nodes.front().name);
    for (const auto& node : nodes)
    {
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicateTypeError>(m_domain_context.declared_types, name, node.name);
        auto bases = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_domain_context.object_type.get_index() };
        result.push_back(intern_type(m_domain_context, repo(), name, std::move(bases)).get_index());
    }
    return result;
}

ygg::IndexList<formalism::Type> AstBuilder::parse_type_expression(const ast::TypeExpression& type)
{
    return boost::apply_visitor([&](const auto& node) { return parse_type_expression_node(node); }, type);
}

ygg::IndexList<formalism::Type> AstBuilder::parse_type_expression_node(const ast::TypeReference& node)
{
    auto result = ygg::IndexList<formalism::Type> {};
    auto k = key(node.name.text);
    if (auto it = m_domain_context.types.find(k); it != m_domain_context.types.end())
        result.push_back(it->second.get_index());
    else if (m_options.strict)
        m_diagnostics.throw_at(node.name, UndefinedTypeError(k));
    else
    {
        auto bases = ygg::IndexList<formalism::Type> {};
        if (k != "object" && k != "number")
            bases.push_back(m_domain_context.object_type.get_index());
        result.push_back(intern_type(m_domain_context, repo(), k, std::move(bases)).get_index());
    }
    return result;
}

ygg::IndexList<formalism::Type> AstBuilder::parse_type_expression_node(const ast::EitherType& node)
{
    auto result = ygg::IndexList<formalism::Type> {};
    for (const auto& alternative : node.alternatives)
    {
        auto part = parse_type_expression(alternative.get());
        result.insert(result.end(), part.begin(), part.end());
    }
    return result;
}

ygg::IndexList<formalism::Object> AstBuilder::parse_objects(const std::vector<ast::TypedName>& nodes,
                                                            ygg::UnorderedMap<std::string, formalism::ObjectView>& table,
                                                            ygg::UnorderedSet<std::string>& declared_objects)
{
    auto result = ygg::IndexList<formalism::Object> {};
    for (const auto& node : nodes)
    {
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicateObjectError>(declared_objects, name, node.name);
        checks().require_typing_if_needed(node.type, node.name);
        auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_domain_context.object_type.get_index() };
        auto view = formalism::get_or_create<formalism::Object>(repo(), to_cista(name), std::move(types));
        table.emplace(name, view);
        result.push_back(view.get_index());
    }
    return result;
}

ygg::IndexList<formalism::Parameter> AstBuilder::parse_parameters(const std::vector<ast::TypedVariable>& nodes)
{
    auto result = ygg::IndexList<formalism::Parameter> {};
    for (const auto& node : nodes)
    {
        const auto name = key(node.variable.text);
        if (!m_parse_context.variable_scopes.empty() && m_parse_context.variable_scopes.back().contains(name))
            m_diagnostics.throw_at(node.variable, DuplicateVariableError(name));
        auto variable = formalism::get_or_create<formalism::Variable>(repo(), to_cista(name));
        if (!m_parse_context.variable_scopes.empty())
            m_parse_context.variable_scopes.back().emplace(name, variable);
        checks().require_typing_if_needed(node.type, node.variable);
        auto types = node.type ? parse_type_expression(*node.type) : ygg::IndexList<formalism::Type> { m_domain_context.object_type.get_index() };
        auto type_views = std::vector<formalism::TypeView> {};
        for (auto type : types)
            type_views.emplace_back(type, repo());
        if (auto [it, inserted] = m_parse_context.variable_types.emplace(variable, type_views); !inserted)
            it->second = std::move(type_views);
        result.push_back(formalism::get_or_create<formalism::Parameter>(repo(), variable.get_index(), std::move(types)).get_index());
    }
    return result;
}

ygg::IndexList<formalism::Predicate> AstBuilder::parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes)
{
    auto result = ygg::IndexList<formalism::Predicate> {};
    for (const auto& node : nodes)
    {
        auto scope = VariableScope(m_parse_context);
        auto parameters = parse_parameters(node.parameters);
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicatePredicateError>(m_domain_context.declared_predicates, name, node.name);
        auto view = formalism::get_or_create<formalism::Predicate>(repo(), to_cista(name), std::move(parameters));
        m_domain_context.predicates.emplace(name, view);
        result.push_back(view.get_index());
    }
    return result;
}

ygg::IndexList<formalism::FunctionSkeleton> AstBuilder::parse_functions(const std::vector<ast::FunctionDeclaration>& nodes)
{
    auto result = ygg::IndexList<formalism::FunctionSkeleton> {};
    for (const auto& node : nodes)
    {
        auto scope = VariableScope(m_parse_context);
        auto parameters = parse_parameters(node.parameters);
        const auto name = key(node.name.text);
        checks().require_requirement(formalism::RequirementKind::NumericFluents, node.name);
        checks().ensure_new<DuplicateFunctionError>(m_domain_context.declared_functions, name, node.name);
        checks().require_typing_if_needed(node.type, node.name);
        auto type = node.type ? parse_type_expression(*node.type).front() : m_domain_context.number_type.get_index();
        auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(), to_cista(name), std::move(parameters), type);
        m_domain_context.functions.emplace(name, view);
        result.push_back(view.get_index());
    }
    return result;
}

ygg::Index<formalism::Term> AstBuilder::parse_term(const ast::Term& node)
{
    if (node.variable)
        return formalism::get_or_create<formalism::Term>(
                   repo(),
                   ygg::Data<formalism::Term>::Variant(lookup_variable(m_parse_context, m_diagnostics, node.name).get_index()))
            .get_index();
    return formalism::get_or_create<formalism::Term>(
               repo(),
               ygg::Data<formalism::Term>::Variant(lookup_object(m_domain_context, m_parse_context, m_diagnostics, node.name).get_index()))
        .get_index();
}

ygg::IndexList<formalism::Term> AstBuilder::parse_terms(const std::vector<ast::Term>& nodes)
{
    auto result = ygg::IndexList<formalism::Term> {};
    for (const auto& node : nodes)
        result.push_back(parse_term(node));
    return result;
}

ygg::Index<formalism::Atom> AstBuilder::parse_atom(const ast::Atom& node)
{
    const auto name = key(node.predicate.text);
    auto pred = name == "=" ? equality_predicate(node.predicate, node.terms.size()) : predicate(node.predicate, node.terms.size());
    if (m_domain_context.declared_predicates.contains(name))
        checks().check_argument_types(name, pred.get_parameters(), node.terms, node.predicate);
    auto terms = parse_terms(node.terms);
    return formalism::get_or_create<formalism::Atom>(repo(), pred.get_index(), std::move(terms)).get_index();
}

ygg::Index<formalism::Literal> AstBuilder::parse_literal(const ast::Literal& node)
{
    checks().mark_requirement_used(formalism::RequirementKind::Strips);
    return formalism::get_or_create<formalism::Literal>(repo(), parse_atom(node.atom), node.positive).get_index();
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition(const ast::Condition& condition)
{
    return boost::apply_visitor([&](const auto& node) { return parse_condition_node(node); }, condition);
}

ygg::Index<formalism::Condition> AstBuilder::wrap_condition(ygg::Data<formalism::Condition>::Variant value)
{
    return formalism::get_or_create<formalism::Condition>(repo(), std::move(value)).get_index();
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionLiteral& node)
{
    return wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(repo(), parse_literal(node.literal)).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionAnd& node)
{
    auto list = ygg::IndexList<formalism::Condition> {};
    for (const auto& child : node.conditions)
        list.push_back(parse_condition(child.get()));
    return wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(repo(), std::move(list)).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionOr& node)
{
    checks().require_requirement(formalism::RequirementKind::DisjunctivePreconditions, node);
    auto list = ygg::IndexList<formalism::Condition> {};
    for (const auto& child : node.conditions)
        list.push_back(parse_condition(child.get()));
    return wrap_condition(formalism::get_or_create<formalism::ConditionOr>(repo(), std::move(list)).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionNot& node)
{
    checks().require_requirement(formalism::RequirementKind::NegativePreconditions, node);
    return wrap_condition(formalism::get_or_create<formalism::ConditionNot>(repo(), parse_condition(node.condition.get())).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionImply& node)
{
    checks().require_requirement(formalism::RequirementKind::DisjunctivePreconditions, node);
    return wrap_condition(
        formalism::get_or_create<formalism::ConditionImply>(repo(), parse_condition(node.left.get()), parse_condition(node.right.get())).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionExists& node)
{
    checks().require_requirement(formalism::RequirementKind::ExistentialPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_condition(node.condition.get());
    return wrap_condition(formalism::get_or_create<formalism::ConditionExists>(repo(), std::move(parameters), child).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionForall& node)
{
    checks().require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_condition(node.condition.get());
    return wrap_condition(formalism::get_or_create<formalism::ConditionForall>(repo(), std::move(parameters), child).get_index());
}

ygg::Index<formalism::Condition> AstBuilder::parse_condition_node(const ast::ConditionNumericConstraint& node)
{
    checks().require_requirement(formalism::RequirementKind::NumericFluents, node);
    return wrap_condition(formalism::get_or_create<formalism::ConditionNumericConstraint>(repo(),
                                                                                          comparator(node, m_diagnostics),
                                                                                          parse_function_expression(node.left.get()),
                                                                                          parse_function_expression(node.right.get()))
                              .get_index());
}

ygg::Index<formalism::FunctionTerm> AstBuilder::parse_function_term(const ast::FunctionTerm& node)
{
    checks().require_requirement(formalism::RequirementKind::NumericFluents, node.function);
    const auto name = key(node.function.text);
    if (m_parse_context.active_action_costs && name == "total-cost")
        checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
    auto skeleton = function(node.function, node.terms.size());
    if (m_domain_context.declared_functions.contains(name))
        checks().check_argument_types(name, skeleton.get_parameters(), node.terms, node.function);
    auto terms = parse_terms(node.terms);
    return formalism::get_or_create<formalism::FunctionTerm>(repo(), skeleton.get_index(), std::move(terms)).get_index();
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression(const ast::FunctionExpression& expression)
{
    return boost::apply_visitor([&](const auto& node) { return parse_function_expression_node(node); }, expression);
}

ygg::Index<formalism::FunctionExpression> AstBuilder::wrap_function_expression(ygg::Data<formalism::FunctionExpression>::Variant value)
{
    return formalism::get_or_create<formalism::FunctionExpression>(repo(), std::move(value)).get_index();
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression_node(const ast::FunctionExpressionNumber& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), node.value).get_index());
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression_node(const ast::FunctionExpressionFunction& node)
{
    return wrap_function_expression(parse_function_term(node.term));
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression_node(const ast::FunctionExpressionUnary& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::UnaryFunctionExpression>(repo(),
                                                                                                 formalism::UnaryArithmeticOperator::Minus,
                                                                                                 parse_function_expression(node.expression.get()))
                                        .get_index());
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression_node(const ast::FunctionExpressionBinary& node)
{
    return wrap_function_expression(formalism::get_or_create<formalism::BinaryFunctionExpression>(repo(),
                                                                                                  binary_operator(node.op),
                                                                                                  parse_function_expression(node.left.get()),
                                                                                                  parse_function_expression(node.right.get()))
                                        .get_index());
}

ygg::Index<formalism::FunctionExpression> AstBuilder::parse_function_expression_node(const ast::FunctionExpressionMulti& node)
{
    auto expressions = ygg::IndexList<formalism::FunctionExpression> {};
    for (const auto& child : node.expressions)
        expressions.push_back(parse_function_expression(child.get()));
    return wrap_function_expression(
        formalism::get_or_create<formalism::MultiFunctionExpression>(repo(), multi_operator(node.op), std::move(expressions)).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect(const ast::Effect& effect)
{
    return boost::apply_visitor([&](const auto& node) { return parse_effect_node(node); }, effect);
}

ygg::Index<formalism::Effect> AstBuilder::wrap_effect(ygg::Data<formalism::Effect>::Variant value)
{
    return formalism::get_or_create<formalism::Effect>(repo(), std::move(value)).get_index();
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectLiteral& node)
{
    return wrap_effect(formalism::get_or_create<formalism::EffectLiteral>(repo(), parse_literal(node.literal)).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectAnd& node)
{
    auto list = ygg::IndexList<formalism::Effect> {};
    for (const auto& child : node.effects)
        list.push_back(parse_effect(child.get()));
    return wrap_effect(formalism::get_or_create<formalism::EffectAnd>(repo(), std::move(list)).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectNumeric& node)
{
    checks().require_requirement(formalism::RequirementKind::NumericFluents, node);
    const auto op = numeric_effect_operator(node, m_diagnostics);
    // Bare :action-costs permits only writes of the form (increase (total-cost) ...).
    const auto is_total_cost_increase = op == formalism::NumericEffectOperator::Increase && key(node.function.function.text) == "total-cost";
    if (m_parse_context.active_action_costs && is_total_cost_increase)
        checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
    if (!m_parse_context.active_numeric_fluents && !(m_parse_context.active_action_costs && is_total_cost_increase))
        m_diagnostics.throw_at(node, MissingRequirementError("numeric-fluents"));
    auto skeleton = function(node.function.function, node.function.terms.size());
    if (m_domain_context.declared_functions.contains(key(node.function.function.text)))
        checks().check_argument_types(key(node.function.function.text), skeleton.get_parameters(), node.function.terms, node.function.function);
    return wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(repo(),
                                                                          op,
                                                                          skeleton.get_index(),
                                                                          parse_terms(node.function.terms),
                                                                          parse_function_expression(node.expression.get()))
                           .get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectForall& node)
{
    checks().require_requirement(formalism::RequirementKind::UniversalPreconditions, node);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto child = parse_effect(node.effect.get());
    return wrap_effect(formalism::get_or_create<formalism::EffectForall>(repo(), std::move(parameters), child).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectWhen& node)
{
    checks().require_requirement(formalism::RequirementKind::ConditionalEffects, node);
    return wrap_effect(
        formalism::get_or_create<formalism::EffectWhen>(repo(), parse_condition(node.condition.get()), parse_effect(node.effect.get())).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectOneOf& node)
{
    checks().require_requirement(formalism::RequirementKind::NonDeterministic, node);
    auto list = ygg::IndexList<formalism::Effect> {};
    for (const auto& child : node.effects)
        list.push_back(parse_effect(child.get()));
    return wrap_effect(formalism::get_or_create<formalism::EffectOneOf>(repo(), std::move(list)).get_index());
}

ygg::Index<formalism::Effect> AstBuilder::parse_effect_node(const ast::EffectProbabilistic& node)
{
    checks().require_requirement(formalism::RequirementKind::ProbabilisticEffects, node);
    auto list = ygg::IndexList<formalism::EffectProbabilisticAlternative> {};
    auto total = 0.0;
    for (const auto& alternative : node.alternatives)
    {
        if (!std::isfinite(alternative.probability) || alternative.probability < 0.0 || alternative.probability > 1.0)
            m_diagnostics.throw_at(alternative, InvalidProbabilisticEffectError("probability must be in [0, 1]"));
        total += alternative.probability;
        list.push_back(
            formalism::get_or_create<formalism::EffectProbabilisticAlternative>(repo(), alternative.probability, parse_effect(alternative.effect.get()))
                .get_index());
    }
    if (total > 1.0 + 1e-9)
        m_diagnostics.throw_at(node, InvalidProbabilisticEffectError("probabilities sum to more than 1"));
    return wrap_effect(formalism::get_or_create<formalism::EffectProbabilistic>(repo(), std::move(list)).get_index());
}

ygg::Index<formalism::Action> AstBuilder::parse_action(const ast::Action& node)
{
    checks().mark_requirement_used(formalism::RequirementKind::Strips);
    const auto name = key(node.name.text);
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.parameters);
    auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
    if (node.precondition)
        precondition = parse_condition(*node.precondition);
    auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
    if (node.effect)
        effect = parse_effect(*node.effect);
    return formalism::get_or_create<formalism::Action>(repo(), to_cista(name), std::move(parameters), precondition, effect).get_index();
}

ygg::Index<formalism::Axiom> AstBuilder::parse_axiom(const ast::Axiom& node)
{
    auto scope = VariableScope(m_parse_context);
    auto parameters = parse_parameters(node.head.parameters);
    auto terms = ygg::IndexList<formalism::Term> {};
    for (const auto& parameter : node.head.parameters)
    {
        auto term = ast::Term {};
        term.name = parameter.variable;
        term.variable = true;
        terms.push_back(parse_term(term));
    }
    auto pred = predicate(node.head.name, terms.size());
    auto atom = formalism::get_or_create<formalism::Atom>(repo(), pred.get_index(), std::move(terms)).get_index();
    auto head = formalism::get_or_create<formalism::Literal>(repo(), atom, true).get_index();
    auto condition = parse_condition(node.condition);
    return formalism::get_or_create<formalism::Axiom>(repo(), std::move(parameters), head, condition).get_index();
}

void AstBuilder::parse_initial_element(const ast::Literal& literal,
                                       ygg::IndexList<formalism::Literal>& literals,
                                       ygg::IndexList<formalism::InitialFunctionValue>&)
{
    literals.push_back(parse_literal(literal));
}

void AstBuilder::parse_initial_element(const ast::InitialFunctionValue& value,
                                       ygg::IndexList<formalism::Literal>&,
                                       ygg::IndexList<formalism::InitialFunctionValue>& values)
{
    values.push_back(
        formalism::get_or_create<formalism::InitialFunctionValue>(repo(), parse_function_term(value.function), parse_function_expression(value.value))
            .get_index());
}

ygg::Index<formalism::Metric> AstBuilder::parse_metric(const ast::Metric& node)
{
    checks().require_requirement(formalism::RequirementKind::NumericFluents, node);
    const auto optimization = key(node.optimization.text);
    if (optimization != "minimize" && optimization != "maximize")
        m_diagnostics.throw_at(node.optimization, InvalidMetricError(optimization));
    return formalism::get_or_create<formalism::Metric>(repo(), optimization == "minimize", parse_function_expression(node.expression)).get_index();
}

formalism::FunctionSkeletonView AstBuilder::total_cost_function()
{
    if (auto it = m_domain_context.functions.find("total-cost"); it != m_domain_context.functions.end())
        return it->second;
    auto view = formalism::get_or_create<formalism::FunctionSkeleton>(repo(),
                                                                      cista::offset::string("total-cost"),
                                                                      ygg::IndexList<formalism::Parameter> {},
                                                                      m_domain_context.number_type.get_index());
    m_domain_context.functions.emplace("total-cost", view);
    return view;
}

ygg::Index<formalism::FunctionTerm> AstBuilder::total_cost_term()
{
    return formalism::get_or_create<formalism::FunctionTerm>(repo(), total_cost_function().get_index(), ygg::IndexList<formalism::Term> {}).get_index();
}

bool AstBuilder::writes_total_cost(formalism::EffectView effect)
{
    return ygg::visit(
        [&](const auto& arg) -> bool
        {
            using Node = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Node, formalism::EffectNumericView>)
                return arg.get_function().get_name() == "total-cost";
            else if constexpr (std::is_same_v<Node, formalism::EffectAndView> || std::is_same_v<Node, formalism::EffectOneOfView>)
            {
                for (auto child : arg.get_effects())
                    if (writes_total_cost(child))
                        return true;
                return false;
            }
            else if constexpr (std::is_same_v<Node, formalism::EffectForallView> || std::is_same_v<Node, formalism::EffectWhenView>)
                return writes_total_cost(arg.get_effect());
            else if constexpr (std::is_same_v<Node, formalism::EffectProbabilisticView>)
            {
                for (auto alternative : arg.get_alternatives())
                    if (writes_total_cost(alternative.get_effect()))
                        return true;
                return false;
            }
            else
                return false;
        },
        effect.get_value());
}

ygg::Index<formalism::Action> AstBuilder::add_unit_cost(ygg::Index<formalism::Action> action)
{
    const auto view = ygg::make_view(action, repo());
    if (const auto effect = view.get_effect(); effect && writes_total_cost(effect.value()))
        return action;

    const auto one = wrap_function_expression(formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), 1.0).get_index());
    const auto increase = wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(repo(),
                                                                                         formalism::NumericEffectOperator::Increase,
                                                                                         total_cost_function().get_index(),
                                                                                         ygg::IndexList<formalism::Term> {},
                                                                                         one)
                                          .get_index());
    auto combined = increase;
    if (const auto effect = view.get_effect())
        combined = wrap_effect(
            formalism::get_or_create<formalism::EffectAnd>(repo(), ygg::IndexList<formalism::Effect> { effect.value().get_index(), increase }).get_index());

    auto parameters = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : view.get_parameters())
        parameters.push_back(parameter.get_index());
    auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
    if (const auto condition = view.get_precondition())
        precondition = condition.value().get_index();
    return formalism::get_or_create<formalism::Action>(repo(),
                                                       to_cista(std::string(view.get_name())),
                                                       std::move(parameters),
                                                       precondition,
                                                       cista::optional<ygg::Index<formalism::Effect>>(combined))
        .get_index();
}

bool AstBuilder::has_total_cost_initial_value(const ygg::IndexList<formalism::InitialFunctionValue>& values)
{
    for (auto value : values)
    {
        const auto function = ygg::make_view(value, repo()).get_function();
        if (function.get_function().get_name() == "total-cost" && function.get_terms().empty())
            return true;
    }
    return false;
}

void AstBuilder::complete_action_costs(const ast::Task& task,
                                       ygg::IndexList<formalism::InitialFunctionValue>& initial_function_values,
                                       cista::optional<ygg::Index<formalism::Metric>>& metric)
{
    if (!m_parse_context.active_action_costs)
        return;

    const auto missing_metric = !metric;
    const auto missing_initial_value = !has_total_cost_initial_value(initial_function_values);
    if (m_options.strict && !m_options.add_action_costs)
    {
        if (missing_metric)
            m_diagnostics.throw_at(task, SemanticError("Missing total-cost metric for :action-costs"));
        if (missing_initial_value)
            m_diagnostics.throw_at(task, SemanticError("Missing initial value for total-cost for :action-costs"));
        return;
    }

    if (missing_metric)
        metric = formalism::get_or_create<formalism::Metric>(repo(), true, wrap_function_expression(total_cost_term())).get_index();
    if (missing_initial_value)
    {
        const auto zero = formalism::get_or_create<formalism::FunctionExpressionNumber>(repo(), 0.0).get_index();
        const auto zero_expression = wrap_function_expression(zero);
        initial_function_values.push_back(formalism::get_or_create<formalism::InitialFunctionValue>(repo(), total_cost_term(), zero_expression).get_index());
    }
}

}  // namespace loki::semantic
