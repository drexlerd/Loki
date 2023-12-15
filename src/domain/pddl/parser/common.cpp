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
string parse(const domain::ast::Name& name_node) {
    stringstream ss;
    ss << name_node.alpha << name_node.suffix;
    return ss.str();
}

/* Variable */
pddl::Variable parse(const domain::ast::Variable& variable_node, Context& context) {
    stringstream ss;
    ss << variable_node.question_mark << parse(variable_node.name);
    return context.factories.variables.get_or_create<pddl::VariableImpl>(ss.str());
}

/* Term */
TermDeclarationTermVisitor::TermDeclarationTermVisitor(Context& context_)
    : context(context_) { }

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::Name& name_node) const {
    const auto constant_name = parse(name_node);
    const auto binding = context.scopes.get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        throw UndefinedConstantError(constant_name, context.scopes.get_error_handler()(name_node, ""));
    }
    const auto& [constant, _position, _error_handler] = binding.value();
    return context.factories.terms.get_or_create<pddl::TermObjectImpl>(constant);
}

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    const auto variable = parse(variable_node, context);
    const auto binding = context.scopes.get<pddl::VariableImpl>(variable->get_name());
    if (binding.has_value()) {
        const auto message_1 = context.scopes.get_error_handler()(variable_node, "Defined here:");
        const auto& [_constant, position, error_handler] = binding.value();
        assert(position.has_value());
        const auto message_2 = error_handler(position.value(), "First defined here:");
        throw MultiDefinitionVariableError(variable->get_name(), message_1 + message_2);
    }
    context.scopes.insert<pddl::VariableImpl>(variable->get_name(), variable, variable_node);
    return context.factories.terms.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    throw NotSupportedError(
        pddl::RequirementEnum::OBJECT_FLUENTS,
        context.scopes.get_error_handler()(function_term_node, ""));
}


TermReferenceTermVisitor::TermReferenceTermVisitor(Context& context_)
    : context(context_) { }

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::Name& name_node) const {
    const auto constant_name = parse(name_node);
    const auto binding = context.scopes.get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        throw UndefinedConstantError(constant_name, context.scopes.get_error_handler()(name_node, ""));
    }
    const auto& [constant, position, error_handler] = binding.value();
    return context.factories.terms.get_or_create<pddl::TermObjectImpl>(constant);
}

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    const auto variable = parse(variable_node, context);
    const auto binding = context.scopes.get<pddl::VariableImpl>(variable->get_name());
    if (!binding.has_value()) {
        throw UndefinedVariableError(variable->get_name(), context.scopes.get_error_handler()(variable_node, ""));
    }
    return context.factories.terms.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    throw NotSupportedError(
        pddl::RequirementEnum::OBJECT_FLUENTS,
        context.scopes.get_error_handler()(function_term_node, ""));
}


/* Number */
double parse(const domain::ast::Number& number_node) {
    return number_node.value;
}

}
