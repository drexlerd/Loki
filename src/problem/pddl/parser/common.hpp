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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_COMMON_HPP_

#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/problem/pddl/parser.hpp"


namespace loki {

struct ObjectReferenceTermVisitor : boost::static_visitor<pddl::Object> {
    problem::Context& context;

    ObjectReferenceTermVisitor(problem::Context& context_);

    pddl::Object operator()(const domain::ast::Name& name_node) const;
    pddl::Object operator()(const domain::ast::Variable& variable_node) const;
    pddl::Object operator()(const domain::ast::FunctionTerm& function_term_node) const;
};

}

#endif
