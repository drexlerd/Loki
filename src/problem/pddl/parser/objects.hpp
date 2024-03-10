/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_OBJECTS_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_OBJECTS_HPP_

#include "loki/problem/ast/ast.hpp"
#include "loki/problem/pddl/parser.hpp"

namespace loki
{

/* Object */
extern pddl::Object parse_object_reference(const domain::ast::Name& name_node, Context& context);

extern pddl::ObjectList parse(const problem::ast::Objects& objects_node, Context& context);

class ObjectListVisitor : boost::static_visitor<pddl::ObjectList>
{
private:
    Context& context;

public:
    ObjectListVisitor(Context& context_);

    pddl::ObjectList operator()(const std::vector<domain::ast::Name>& name_nodes);

    pddl::ObjectList operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
