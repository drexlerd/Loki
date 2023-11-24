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

#include "../../../../include/loki/common/exceptions.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

/* Name */
string parse(const domain::ast::Name& name_node, const error_handler_type&, domain::Context&) {
    stringstream ss;
    ss << name_node.alpha << name_node.suffix;
    return ss.str();
}

/* Variable */
pddl::Variable parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context) {
    stringstream ss;
    ss << variable_node.question_mark << parse(variable_node.name, error_handler, context);
    return context.cache.get_or_create<pddl::VariableImpl>(ss.str()).object;
}

/* Term */
TermVisitor::TermVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Term TermVisitor::operator()(const domain::ast::Name& name_node) const {
    auto constant_name = parse(name_node, error_handler, context);
    auto result = context.cache.get_or_create<pddl::ObjectImpl>(constant_name);
    if (result.created) {
        error_handler(name_node, constant_name + " is undefined");
        throw UndefinedError(constant_name, "constant");
    }
    return context.cache.get_or_create<pddl::TermConstantImpl>(result.object).object;
}

pddl::Term TermVisitor::operator()(const domain::ast::Variable& variable_node) const {
    return context.cache.get_or_create<pddl::TermVariableImpl>(parse(variable_node, error_handler, context)).object;
}

pddl::Term TermVisitor::operator()(const domain::ast::FunctionTerm& function_term_node) const {
    throw NotSupportedError("Found unsupported feature FunctionTerm. Are you using :object-fluents?");
}


pddl::Term parse(const domain::ast::Term& term_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(TermVisitor(error_handler, context), term_node);
}

pddl::TermList parse(const std::vector<domain::ast::Term>& term_list_node, const error_handler_type& error_handler, domain::Context& context) {
    pddl::TermList term_list; 
    for (const auto& term_node : term_list_node) {
        term_list.push_back(parse(term_node, error_handler, context));
    }
    return term_list;
}

/* Number */
double parse(const domain::ast::Number& number_node, const error_handler_type&, domain::Context&) {
    return number_node.value;
}

}
