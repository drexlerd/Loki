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

#ifndef LOKI_SRC_PDDL_PARSER_TYPES_HPP_
#define LOKI_SRC_PDDL_PARSER_TYPES_HPP_

#include "loki/ast/ast.hpp"
#include "loki/pddl/parser.hpp"
#include "loki/pddl/type.hpp"

namespace loki
{

class TypeDeclarationTypeVisitor : boost::static_visitor<pddl::TypeList>
{
private:
    Context& context;

public:
    TypeDeclarationTypeVisitor(Context& context_);

    pddl::TypeList operator()(const ast::TypeObject& node);

    pddl::TypeList operator()(const ast::TypeNumber& node);

    pddl::TypeList operator()(const ast::Name& node);

    pddl::TypeList operator()(const ast::TypeEither& node);
};

class TypeReferenceTypeVisitor : boost::static_visitor<pddl::TypeList>
{
private:
    const Context& context;

public:
    TypeReferenceTypeVisitor(const Context& context_);

    pddl::TypeList operator()(const ast::TypeObject& node);

    pddl::TypeList operator()(const ast::TypeNumber& node);

    pddl::TypeList operator()(const ast::Name& node);

    pddl::TypeList operator()(const ast::TypeEither& node);
};

class TypeDeclarationTypedListOfNamesVisitor : boost::static_visitor<pddl::TypeList>
{
private:
    Context& context;

public:
    TypeDeclarationTypedListOfNamesVisitor(Context& context_);

    pddl::TypeList operator()(const std::vector<ast::Name>& nodes);

    pddl::TypeList operator()(const ast::TypedListOfNamesRecursively& node);
};

extern pddl::TypeList parse(const ast::Types& node, Context& context);

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
