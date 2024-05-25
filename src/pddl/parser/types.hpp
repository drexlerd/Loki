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

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/parser.hpp"
#include "loki/details/pddl/type.hpp"

namespace loki
{

class TypeDeclarationTypeVisitor : boost::static_visitor<TypeList>
{
private:
    Context& context;

public:
    TypeDeclarationTypeVisitor(Context& context_);

    TypeList operator()(const ast::TypeObject& node);

    TypeList operator()(const ast::TypeNumber& node);

    TypeList operator()(const ast::Name& node);

    TypeList operator()(const ast::TypeEither& node);
};

class TypeReferenceTypeVisitor : boost::static_visitor<TypeList>
{
private:
    const Context& context;

public:
    TypeReferenceTypeVisitor(const Context& context_);

    TypeList operator()(const ast::TypeObject& node);

    TypeList operator()(const ast::TypeNumber& node);

    TypeList operator()(const ast::Name& node);

    TypeList operator()(const ast::TypeEither& node);
};

class CollectParentTypesHierarchyVisitor : boost::static_visitor<std::unordered_set<std::string>>
{
private:
    Context& context;
    std::unordered_map<std::string, Position>& type_last_occurrence;

public:
    CollectParentTypesHierarchyVisitor(Context& context_, std::unordered_map<std::string, Position>& type_last_occurrence_);

    std::unordered_set<std::string> operator()(const ast::TypeObject& node);

    std::unordered_set<std::string> operator()(const ast::TypeNumber& node);

    std::unordered_set<std::string> operator()(const ast::Name& node);

    std::unordered_set<std::string> operator()(const ast::TypeEither& node);
};

class CollectTypesHierarchyVisitor
{
private:
    Context& context;
    std::unordered_map<std::string, std::unordered_set<std::string>>& child_types;
    std::unordered_map<std::string, Position>& type_last_occurrence;

public:
    CollectTypesHierarchyVisitor(Context& context_,
                                 std::unordered_map<std::string, std::unordered_set<std::string>>& parent_types_,
                                 std::unordered_map<std::string, Position>& type_last_occurrence_);

    void operator()(const std::vector<ast::Name>& nodes);

    void operator()(const ast::TypedListOfNamesRecursively& node);
};

extern TypeList parse(const ast::Types& node, Context& context);

}
/*
(:types hoist surface place area - object
    container depot - place
    storearea transitarea - area
    area crate - surface)

*/

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
