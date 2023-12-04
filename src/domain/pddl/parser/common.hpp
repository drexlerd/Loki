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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

/* Name */
extern std::string parse(const domain::ast::Name& name_node);

/* Variable */
extern pddl::Variable parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context);

/* Term */
struct TermDeclarationVisitor : boost::static_visitor<pddl::Term> {
    const error_handler_type& error_handler;
    domain::Context& context;

    TermDeclarationVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Term operator()(const domain::ast::Name& name_node) const;
    pddl::Term operator()(const domain::ast::Variable& variable_node) const;
    pddl::Term operator()(const domain::ast::FunctionTerm& function_term_node) const;
};

struct TermReferenceVisitor : boost::static_visitor<pddl::Term> {
    const error_handler_type& error_handler;
    domain::Context& context;

    TermReferenceVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Term operator()(const domain::ast::Name& name_node) const;
    pddl::Term operator()(const domain::ast::Variable& variable_node) const;
    pddl::Term operator()(const domain::ast::FunctionTerm& function_term_node) const;
};


/* Number */
extern double parse(const domain::ast::Number& number_node);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
