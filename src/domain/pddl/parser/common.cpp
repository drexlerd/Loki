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
pddl::Variable parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context) {
    stringstream ss;
    ss << variable_node.question_mark << parse(variable_node.name);
    return context.cache.get_or_create<pddl::VariableImpl>(ss.str());
}

/* Term */
TermDeclarationVisitor::TermDeclarationVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Term TermDeclarationVisitor::operator()(const domain::ast::Name& name_node) const {
    auto constant_name = parse(name_node);
    auto constant = context.scopes.back()->get<pddl::ObjectImpl>(constant_name);
    if (!constant) {
        error_handler(name_node, "");
        throw UndefinedConstantError(constant_name, context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::TermConstantImpl>(constant);
}

pddl::Term TermDeclarationVisitor::operator()(const domain::ast::Variable& variable_node) const {
    auto variable = parse(variable_node, error_handler, context);
    if (context.scopes.back()->get<pddl::VariableImpl>(variable->get_name())) {
        error_handler(variable_node, "");
        throw MultiDefinitionVariableError(variable->get_name(), context.error_stream->str());
    }
    context.scopes.back()->insert<pddl::VariableImpl>(variable->get_name(), variable);
    return context.cache.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermDeclarationVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    error_handler(function_term_node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, context.error_stream->str());
}


TermReferenceVisitor::TermReferenceVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Term TermReferenceVisitor::operator()(const domain::ast::Name& name_node) const {
    auto constant_name = parse(name_node);
    auto constant = context.scopes.back()->get<pddl::ObjectImpl>(constant_name);
    if (!constant) {
        error_handler(name_node, "");
        throw UndefinedConstantError(constant_name, context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::TermConstantImpl>(constant);
}

pddl::Term TermReferenceVisitor::operator()(const domain::ast::Variable& variable_node) const {
    auto variable = parse(variable_node, error_handler, context);
    if (!context.scopes.back()->get<pddl::VariableImpl>(variable->get_name())) {
        error_handler(variable_node, "");
        throw UndefinedVariableError(variable->get_name(), context.error_stream->str());
    }
    return context.cache.get_or_create<pddl::TermVariableImpl>(variable);
}

pddl::Term TermReferenceVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    error_handler(function_term_node, "");
    throw NotSupportedError(pddl::RequirementEnum::OBJECT_FLUENTS, context.error_stream->str());
}


/* Number */
double parse(const domain::ast::Number& number_node) {
    return number_node.value;
}

}
