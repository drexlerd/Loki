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

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/problem/ast/ast.hpp"
#include "../../../../include/loki/problem/pddl/parser.hpp"


namespace loki {

class ObjectListVisitor : boost::static_visitor<pddl::ObjectList> {
private:
    const error_handler_type& error_handler;
    problem::Context& context;

public:
    ObjectListVisitor(const error_handler_type& error_handler_, problem::Context& context_);

    pddl::ObjectList operator()(const std::vector<domain::ast::Name>& name_nodes);

    pddl::ObjectList operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);

    pddl::ObjectList operator()(const domain::ast::TypedListOfNames& node);
};

extern pddl::ObjectList parse(const problem::ast::Objects& objects_node, const error_handler_type& error_handler, problem::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
