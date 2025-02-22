/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "common.hpp"

#include "error_handling.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"

using namespace std;

namespace loki
{

/* Name */
string parse(const ast::Name& node) { return node.characters; }

/* Variable */
template<ParsingContext C>
Variable parse(const ast::Variable& node, C& context)
{
    const auto variable = context.builder.get_or_create_variable(node.characters);
    context.references.untrack(variable);
    context.positions.push_back(variable, node);
    return variable;
}

template Variable parse(const ast::Variable& node, DomainParsingContext& context);
template Variable parse(const ast::Variable& node, ProblemParsingContext& context);

/* Term */
template<ParsingContext C>
TermDeclarationTermVisitor<C>::TermDeclarationTermVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
Term TermDeclarationTermVisitor<C>::operator()(const ast::Name& node) const
{
    const auto constant_name = parse(node);
    test_undefined_constant(constant_name, node, context);  // TODO must be undefined object!
    // Constant are not tracked and hence must not be untracked.
    const auto binding = context.scopes.top().get_object(constant_name);
    const auto [constant, _position, _error_handler] = binding.value();
    const auto term = context.builder.get_or_create_term(constant);
    context.positions.push_back(term, node);
    return term;
}

template<ParsingContext C>
Term TermDeclarationTermVisitor<C>::operator()(const ast::Variable& node) const
{
    const auto variable = parse(node, context);
    test_multiple_definition_variable(variable, node, context);
    context.scopes.top().insert_variable(variable->get_name(), variable, node);
    const auto term = context.builder.get_or_create_term(variable);
    context.positions.push_back(term, node);
    return term;
}

template struct TermDeclarationTermVisitor<DomainParsingContext>;
template struct TermDeclarationTermVisitor<ProblemParsingContext>;

template<ParsingContext C>
TermReferenceTermVisitor<C>::TermReferenceTermVisitor(C& context_) : context(context_)
{
}

template<ParsingContext C>
Term TermReferenceTermVisitor<C>::operator()(const ast::Name& node) const
{
    const auto object_name = parse(node);
    test_undefined_constant(object_name, node, context);
    const auto binding = context.scopes.top().get_object(object_name);
    const auto [object, _position, _error_handler] = binding.value();
    context.references.untrack(object);
    const auto term = context.builder.get_or_create_term(object);
    context.positions.push_back(term, node);
    return term;
}

template<ParsingContext C>
Term TermReferenceTermVisitor<C>::operator()(const ast::Variable& node) const
{
    const auto variable = parse(node, context);
    if (!context.options.allow_free_variables)
    {
        test_undefined_variable(variable, node, context);
    }
    const auto term = context.builder.get_or_create_term(variable);
    context.positions.push_back(term, node);
    return term;
}

template struct TermReferenceTermVisitor<DomainParsingContext>;
template struct TermReferenceTermVisitor<ProblemParsingContext>;

/* Number */
double parse(const ast::Number& node) { return node.value; }

}
