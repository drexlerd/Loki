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

#ifndef LOKI_SRC_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_PDDL_PARSER_COMMON_HPP_

#include "loki/ast/ast.hpp"
#include "loki/pddl/context.hpp"

namespace loki
{

/* Name */
extern std::string parse(const ast::Name& node);

/* Variable */
extern Variable parse(const ast::Variable& node, Context& context);

/* Term */
struct TermDeclarationTermVisitor : boost::static_visitor<Term>
{
    Context& context;

    TermDeclarationTermVisitor(Context& context_);

    Term operator()(const ast::Name& node) const;
    Term operator()(const ast::Variable& node) const;
};

struct TermReferenceTermVisitor : boost::static_visitor<Term>
{
    Context& context;

    TermReferenceTermVisitor(Context& context_);

    Term operator()(const ast::Name& node) const;
    Term operator()(const ast::Variable& node) const;
};

/* Number */
extern double parse(const ast::Number& node);
}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
