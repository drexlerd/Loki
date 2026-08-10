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
#include "loki/semantic/errors.hpp"
#include "mappings.hpp"

#include <functional>
#include <type_traits>
#include <utility>

namespace loki::semantic
{

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
    auto data = checkout<formalism::Predicate>();
    data->name = to_cista(name);
    auto view = formalism::get_or_create(repo(), *data);
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

    auto variable_data = checkout<formalism::Variable>();
    variable_data->name = cista::offset::string("?lhs");
    const auto left = formalism::get_or_create(repo(), *variable_data).get_index();
    variable_data->clear();
    variable_data->name = cista::offset::string("?rhs");
    const auto right = formalism::get_or_create(repo(), *variable_data).get_index();
    auto parameter_data = checkout<formalism::Parameter>();
    parameter_data->variable = left;
    parameter_data->types.push_back(m_domain_context.object_type.get_index());
    const auto left_parameter = formalism::get_or_create(repo(), *parameter_data);
    parameter_data->clear();
    parameter_data->variable = right;
    parameter_data->types.push_back(m_domain_context.object_type.get_index());
    const auto right_parameter = formalism::get_or_create(repo(), *parameter_data);
    auto predicate_data = checkout<formalism::Predicate>();
    predicate_data->name = cista::offset::string("=");
    predicate_data->parameters.push_back(left_parameter.get_index());
    predicate_data->parameters.push_back(right_parameter.get_index());
    auto view = formalism::get_or_create(repo(), *predicate_data);
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
    auto data = checkout<formalism::FunctionSkeleton>();
    data->name = to_cista(name);
    data->type = m_domain_context.number_type.get_index();
    auto view = formalism::get_or_create(repo(), *data);
    m_domain_context.functions.emplace(name, view);
    return view;
}

std::vector<formalism::RequirementView> AstBuilder::parse_requirements(const std::vector<ast::Requirement>& nodes)
{
    auto result = std::vector<formalism::RequirementView> {};
    auto data = checkout<formalism::Requirement>();
    for (const auto& node : nodes)
    {
        const auto name = key(node.name.text);
        // Aggregates hide which capabilities a model needs; strict mode demands the atomic
        // requirements, and the MissingRequirementError checks then enumerate the right ones.
        if (m_options.strict && (name == "adl" || name == "fluents" || name == "quantified-preconditions"))
            m_diagnostics.throw_at(node.name, AggregateRequirementError(name));
        const auto kind = requirement_kind(node, m_diagnostics);
        if (kind == formalism::RequirementKind::ActionCosts)
            m_parse_context.active_action_costs = true;
        if (kind == formalism::RequirementKind::Fluents || kind == formalism::RequirementKind::NumericFluents)
            m_parse_context.active_numeric_fluents = true;
        for (const auto capability : requirement_capabilities(node, m_diagnostics))
            m_parse_context.active_requirements.insert(capability);
        data->clear();
        data->kind = kind;
        result.push_back(formalism::get_or_create(repo(), *data));
    }
    return result;
}

std::vector<formalism::TypeView> AstBuilder::parse_types(const std::vector<ast::TypedName>& nodes)
{
    if (!nodes.empty())
        checks().require_requirement(formalism::RequirementKind::Typing, nodes.front().name);

    auto declarations = ygg::UnorderedMap<std::string, const ast::TypedName*> {};
    for (const auto& node : nodes)
    {
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicateTypeError>(m_domain_context.declared_types, name, node.name);
        declarations.emplace(name, &node);
    }

    auto resolving = ygg::UnorderedSet<std::string> {};
    auto build_type = std::function<formalism::TypeView(const ast::TypedName&)> {};
    auto resolve_type_expression = std::function<std::vector<formalism::TypeView>(const ast::TypeExpression&)> {};

    // A type may inherit from another type declared later in the same :types section.
    resolve_type_expression = [&](const ast::TypeExpression& expression)
    {
        return boost::apply_visitor(
            [&](const auto& node) -> std::vector<formalism::TypeView>
            {
                using Node = std::decay_t<decltype(node)>;
                if constexpr (std::is_same_v<Node, ast::TypeReference>)
                {
                    const auto name = key(node.name.text);
                    if (const auto it = m_domain_context.types.find(name); it != m_domain_context.types.end())
                        return { it->second };
                    if (const auto it = declarations.find(name); it != declarations.end())
                        return { build_type(*it->second) };
                    return parse_type_expression_node(node);
                }
                else
                {
                    auto result = std::vector<formalism::TypeView> {};
                    for (const auto& alternative : node.alternatives)
                    {
                        auto part = resolve_type_expression(alternative.get());
                        result.insert(result.end(), part.begin(), part.end());
                    }
                    return result;
                }
            },
            expression);
    };

    build_type = [&](const ast::TypedName& node)
    {
        const auto name = key(node.name.text);
        if (const auto it = m_domain_context.types.find(name); it != m_domain_context.types.end())
            return it->second;
        if (!resolving.insert(name).second)
            m_diagnostics.throw_at(node.name, SemanticError("Cyclic type hierarchy involving " + name));

        auto bases = node.type ? resolve_type_expression(*node.type) : std::vector<formalism::TypeView> { m_domain_context.object_type };
        resolving.erase(name);
        return intern_type(m_domain_context, m_builder, repo(), name, bases);
    };

    auto result = std::vector<formalism::TypeView> {};
    for (const auto& node : nodes)
        result.push_back(build_type(node));
    return result;
}

std::vector<formalism::TypeView> AstBuilder::parse_type_expression(const ast::TypeExpression& type)
{
    return boost::apply_visitor([&](const auto& node) { return parse_type_expression_node(node); }, type);
}

std::vector<formalism::TypeView> AstBuilder::parse_type_expression_node(const ast::TypeReference& node)
{
    auto result = std::vector<formalism::TypeView> {};
    auto k = key(node.name.text);
    if (auto it = m_domain_context.types.find(k); it != m_domain_context.types.end())
        result.push_back(it->second);
    else if (m_options.strict)
        m_diagnostics.throw_at(node.name, UndefinedTypeError(k));
    else
    {
        auto bases = std::vector<formalism::TypeView> {};
        if (k != "object" && k != "number")
            bases.push_back(m_domain_context.object_type);
        result.push_back(intern_type(m_domain_context, m_builder, repo(), k, bases));
    }
    return result;
}

std::vector<formalism::TypeView> AstBuilder::parse_type_expression_node(const ast::EitherType& node)
{
    auto result = std::vector<formalism::TypeView> {};
    for (const auto& alternative : node.alternatives)
    {
        auto part = parse_type_expression(alternative.get());
        result.insert(result.end(), part.begin(), part.end());
    }
    return result;
}

std::vector<formalism::ObjectView> AstBuilder::parse_objects(const std::vector<ast::TypedName>& nodes,
                                                             ygg::UnorderedMap<std::string, formalism::ObjectView>& table,
                                                             ygg::UnorderedSet<std::string>& declared_objects)
{
    auto result = std::vector<formalism::ObjectView> {};
    for (const auto& node : nodes)
    {
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicateObjectError>(declared_objects, name, node.name);
        checks().require_typing_if_needed(node.type, node.name);
        auto types = node.type ? parse_type_expression(*node.type) : std::vector<formalism::TypeView> { m_domain_context.object_type };
        auto data = checkout<formalism::Object>();
        data->name = to_cista(name);
        append_indices(types, data->types);
        auto view = formalism::get_or_create(repo(), *data);
        table.emplace(name, view);
        result.push_back(view);
    }
    return result;
}

std::vector<formalism::ParameterView> AstBuilder::parse_parameters(const std::vector<ast::TypedVariable>& nodes)
{
    auto result = std::vector<formalism::ParameterView> {};
    for (const auto& node : nodes)
    {
        const auto name = key(node.variable.text);
        if (!m_parse_context.variable_scopes.empty() && m_parse_context.variable_scopes.back().contains(name))
            m_diagnostics.throw_at(node.variable, DuplicateVariableError(name));
        auto variable_data = checkout<formalism::Variable>();
        variable_data->name = to_cista(name);
        auto variable = formalism::get_or_create(repo(), *variable_data);
        if (!m_parse_context.variable_scopes.empty())
            m_parse_context.variable_scopes.back().emplace(name, variable);
        checks().require_typing_if_needed(node.type, node.variable);
        auto types = node.type ? parse_type_expression(*node.type) : std::vector<formalism::TypeView> { m_domain_context.object_type };
        auto parameter_data = checkout<formalism::Parameter>();
        parameter_data->variable = variable.get_index();
        append_indices(types, parameter_data->types);
        if (auto [it, inserted] = m_parse_context.variable_types.emplace(variable, types); !inserted)
            it->second = std::move(types);
        result.push_back(formalism::get_or_create(repo(), *parameter_data));
    }
    return result;
}

std::vector<formalism::PredicateView> AstBuilder::parse_predicates(const std::vector<ast::PredicateDeclaration>& nodes)
{
    auto result = std::vector<formalism::PredicateView> {};
    for (const auto& node : nodes)
    {
        auto scope = VariableScope(m_parse_context);
        auto parameters = parse_parameters(node.parameters);
        const auto name = key(node.name.text);
        checks().ensure_new<DuplicatePredicateError>(m_domain_context.declared_predicates, name, node.name);
        auto data = checkout<formalism::Predicate>();
        data->name = to_cista(name);
        append_indices(parameters, data->parameters);
        auto view = formalism::get_or_create(repo(), *data);
        m_domain_context.predicates.emplace(name, view);
        result.push_back(view);
    }
    return result;
}

std::vector<formalism::FunctionSkeletonView> AstBuilder::parse_functions(const std::vector<ast::FunctionDeclaration>& nodes)
{
    auto result = std::vector<formalism::FunctionSkeletonView> {};
    for (const auto& node : nodes)
    {
        auto scope = VariableScope(m_parse_context);
        auto parameters = parse_parameters(node.parameters);
        const auto name = key(node.name.text);
        if (m_parse_context.active_action_costs && name == "total-cost")
            checks().mark_requirement_used(formalism::RequirementKind::ActionCosts);
        else
            checks().require_requirement(formalism::RequirementKind::NumericFluents, node.name);
        checks().ensure_new<DuplicateFunctionError>(m_domain_context.declared_functions, name, node.name);
        checks().require_typing_if_needed(node.type, node.name);
        auto type = node.type ? parse_type_expression(*node.type).front().get_index() : m_domain_context.number_type.get_index();
        auto data = checkout<formalism::FunctionSkeleton>();
        data->name = to_cista(name);
        append_indices(parameters, data->parameters);
        data->type = type;
        auto view = formalism::get_or_create(repo(), *data);
        m_domain_context.functions.emplace(name, view);
        result.push_back(view);
    }
    return result;
}

}  // namespace loki::semantic
