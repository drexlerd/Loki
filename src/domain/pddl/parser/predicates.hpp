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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_

#include "../../../../include/domain/ast/ast.hpp"
#include "../../../../include/domain/pddl/parser.hpp"
#include "../../../../include/domain/pddl/predicate.hpp"


namespace loki {

extern pddl::PredicateList parse(const domain::ast::Predicates& predicates_node, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_PREDICATES_HPP_
