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

#include "../../../../include/loki/domain/ast/ast.hpp"

#include "../../../../include/loki/common/pddl/context.hpp"


namespace loki {

/* Name */
extern std::string parse(const domain::ast::Name& node);

/* Variable */
extern pddl::Variable parse(const domain::ast::Variable& node, Context& context);

/* Term */
struct TermDeclarationTermVisitor : boost::static_visitor<pddl::Term> {
    Context& context;

    TermDeclarationTermVisitor(Context& context_);

    pddl::Term operator()(const domain::ast::Name& node) const;
    pddl::Term operator()(const domain::ast::Variable& node) const;
};

struct TermReferenceTermVisitor : boost::static_visitor<pddl::Term> {
    Context& context;

    TermReferenceTermVisitor(Context& context_);

    pddl::Term operator()(const domain::ast::Name& node) const;
    pddl::Term operator()(const domain::ast::Variable& node) const;
};


/* Number */
extern double parse(const domain::ast::Number& node);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
