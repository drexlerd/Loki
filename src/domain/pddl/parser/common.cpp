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

#include "../../../../include/loki/domain/pddl/exceptions.hpp"

using namespace loki::domain;
using namespace std;


namespace loki {

/* Name */
string parse(const ast::Name& node) {
    return node.characters;
}

/* Variable */
pddl::Variable parse(const ast::Variable& node, Context& context) {
    const auto variable = context.factories.variables.get_or_create<pddl::VariableImpl>(node.characters);
    context.positions.push_back(variable, node);
    return variable;
}

/* Term */
TermDeclarationTermVisitor::TermDeclarationTermVisitor(Context& context_)
    : context(context_) { }


pddl::Term TermDeclarationTermVisitor::operator()(const ast::Name& node) const {
    const auto constant_name = parse(node);
    // Test for undefined constant.
    const auto binding = context.scopes.get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        throw UndefinedConstantError(constant_name, context.scopes.get_error_handler()(node, ""));
    }
    // Constant are not tracked and hence must not be untracked.
    // Construct Term and return it
    const auto& [constant, _position, _error_handler] = binding.value();
    const auto term = context.factories.terms.get_or_create<pddl::TermObjectImpl>(constant);
    // Add position of PDDL object
    context.positions.push_back(term, node);
    return term;
}


pddl::Term TermDeclarationTermVisitor::operator()(const ast::Variable& node) const {
    const auto variable = parse(node, context);
    // Test for multiple definition
    const auto binding = context.scopes.get<pddl::VariableImpl>(variable->get_name());
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(node, "Defined here:");
        const auto& [_constant, position, error_handler] = binding.value();
        assert(position.has_value());
        const auto message_2 = error_handler(position.value(), "First defined here:");
        throw MultiDefinitionVariableError(variable->get_name(), message_1 + message_2);
    }
    // Add binding to scope
    context.scopes.insert<pddl::VariableImpl>(variable->get_name(), variable, node);
    // Construct Term and return it
    const auto term = context.factories.terms.get_or_create<pddl::TermVariableImpl>(variable);
    // Add position of PDDL object
    context.positions.push_back(term, node);
    return term;
}


TermReferenceTermVisitor::TermReferenceTermVisitor(Context& context_)
    : context(context_) { }

pddl::Term TermReferenceTermVisitor::operator()(const ast::Name& node) const {
    const auto constant_name = parse(node);
    // Test for undefined constant.
    const auto binding = context.scopes.get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        throw UndefinedConstantError(constant_name, context.scopes.get_error_handler()(node, ""));
    }
    // Constant are not tracked and hence must not be untracked.
    // Construct Term and return it
    const auto& [constant, _position, _error_handler] = binding.value();
    const auto term = context.factories.terms.get_or_create<pddl::TermObjectImpl>(constant);
    // Add position of PDDL object
    context.positions.push_back(term, node);
    return term;
}

pddl::Term TermReferenceTermVisitor::operator()(const ast::Variable& node) const {
    const auto variable = parse(node, context);
    // Test for undefined variable
    const auto binding = context.scopes.get<pddl::VariableImpl>(variable->get_name());
    if (!binding.has_value()) {
        throw UndefinedVariableError(variable->get_name(), context.scopes.get_error_handler()(node, ""));
    }
    // Declare variable as being referenced.
    context.referenced_pointers.untrack(variable);
    // Construct Term and return it
    const auto term = context.factories.terms.get_or_create<pddl::TermVariableImpl>(variable);
    // Add position of PDDL object
    context.positions.push_back(term, node);
    return term;
}

/* Number */
double parse(const ast::Number& node) {
    return node.value;
}

}
