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
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/parser.hpp"
#include "loki/details/pddl/type.hpp"

namespace loki
{

template<ParsingContext C>
struct TypeReferenceTypeVisitor : boost::static_visitor<TypeList>
{
    const C& context;

    TypeReferenceTypeVisitor(const C& context_);

    TypeList operator()(const ast::TypeObject& node);

    TypeList operator()(const ast::TypeNumber& node);

    TypeList operator()(const ast::Name& node);

    TypeList operator()(const ast::TypeEither& node);
};

struct CollectParentTypesHierarchyVisitor : boost::static_visitor<std::unordered_set<std::string>>
{
    DomainParsingContext& context;
    std::unordered_map<std::string, Position>& type_last_occurrence;

    CollectParentTypesHierarchyVisitor(DomainParsingContext& context_, std::unordered_map<std::string, Position>& type_last_occurrence_);

    std::unordered_set<std::string> operator()(const ast::TypeObject& node);

    std::unordered_set<std::string> operator()(const ast::TypeNumber& node);

    std::unordered_set<std::string> operator()(const ast::Name& node);

    std::unordered_set<std::string> operator()(const ast::TypeEither& node);
};

struct CollectTypesHierarchyVisitor
{
    DomainParsingContext& context;
    std::unordered_map<std::string, std::unordered_set<std::string>>& child_types;
    std::unordered_map<std::string, Position>& type_last_occurrence;

    CollectTypesHierarchyVisitor(DomainParsingContext& context_,
                                 std::unordered_map<std::string, std::unordered_set<std::string>>& parent_types_,
                                 std::unordered_map<std::string, Position>& type_last_occurrence_);

    void operator()(const std::vector<ast::Name>& nodes);

    void operator()(const ast::TypedListOfNamesRecursively& node);
};

extern TypeList parse(const ast::Types& node, DomainParsingContext& context);

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
