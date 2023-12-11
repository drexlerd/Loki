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
    return context.cache.get_or_create<pddl::VariableImpl>(ss.str());
}

/* Term */
TermDeclarationTermVisitor::TermDeclarationTermVisitor(Context& context_)
    : context(context_) { }

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::Name& name_node) const {
    const auto constant_name = parse(name_node);
    const auto& scope = context.scopes.get_current_scope();
    const auto binding = scope.get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        scope.get_error_handler()(name_node, "");
        throw UndefinedConstantError(constant_name, scope.get_error_stream().str());
    }
    const auto constant = binding.value().value.object;
    return context.cache.get_or_create<pddl::TermObjectImpl>(constant);
}

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    const auto variable = parse(variable_node, context);
    const auto& current_scope = context.scopes.get_current_scope();
    const auto binding = current_scope.get<pddl::VariableImpl>(variable->get_name());
    if (binding.has_value()) {
        current_scope.get_error_handler()(variable_node, "Defined here:");
        if (binding.value().value.position.has_value()) {
            binding.value().error_handler->operator()(binding.value().value.position.value(), "First defined here:");
        }
        throw MultiDefinitionVariableError(variable->get_name(), scope.get_error_stream().str());
    }
    context.scopes.get_current_scope().insert<pddl::VariableImpl>(variable->get_name(), variable, variable_node);
    return context.cache.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermDeclarationTermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    const auto& scope = context.scopes.get_current_scope();
    scope.get_error_handler()(function_term_node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, scope.get_error_stream().str());
}


TermReferenceTermVisitor::TermReferenceTermVisitor(Context& context_)
    : context(context_) { }

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::Name& name_node) const {
    const auto constant_name = parse(name_node);
    const auto binding = context.scopes.get_current_scope().get<pddl::ObjectImpl>(constant_name);
    if (!binding.has_value()) {
        binding.value().error_handler->operator()(name_node, "");
        throw UndefinedConstantError(constant_name, binding.value().error_stream->str());
    }
    const auto constant = binding.value().value.object;
    return context.cache.get_or_create<pddl::TermObjectImpl>(constant);
}

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    const auto variable = parse(variable_node, context);
    const auto binding = context.scopes.get_current_scope().get<pddl::VariableImpl>(variable->get_name());
    if (!binding.has_value()) {
        binding.value().error_handler->operator()(variable_node, "");
        throw UndefinedVariableError(variable->get_name(), binding.value().error_stream->str());
    }
    return context.cache.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermReferenceTermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    const auto& scope = context.scopes.get_current_scope();
    scope.get_error_handler()(function_term_node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, scope.get_error_stream().str());
}


/* Number */
double parse(const domain::ast::Number& number_node) {
    return number_node.value;
}

}
