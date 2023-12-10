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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_

#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/type.hpp"


namespace loki {

class TypeDeclarationTypeVisitor : boost::static_visitor<pddl::TypeList> {
private:
    domain::Context& context;

public:
    TypeDeclarationTypeVisitor(domain::Context& context_);

    pddl::TypeList operator()(const domain::ast::Type& type_node);

    pddl::TypeList operator()(const domain::ast::Name& name_node);

    pddl::TypeList operator()(const domain::ast::TypeObject&);

    pddl::TypeList operator()(const domain::ast::TypeEither& either_type_node);
};

class TypeReferenceTypeVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const domain::Context& context;

public:
    TypeReferenceTypeVisitor(const domain::Context& context_);

    pddl::TypeList operator()(const domain::ast::Type& type_node);

    pddl::TypeList operator()(const domain::ast::Name& name_node);

    pddl::TypeList operator()(const domain::ast::TypeObject&);

    pddl::TypeList operator()(const domain::ast::TypeEither& either_type_node);
};

class TypeDeclarationTypedListOfNamesVisitor : boost::static_visitor<pddl::TypeList> {
private:
    domain::Context& context;

public:
    TypeDeclarationTypedListOfNamesVisitor(domain::Context& context_);

    pddl::TypeList operator()(const std::vector<domain::ast::Name>& name_nodes);

    pddl::TypeList operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);
};


extern pddl::TypeList parse(const domain::ast::Types& types_node, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
