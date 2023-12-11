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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "../../common/pddl/context.hpp"
#include "declarations.hpp"

#include "../ast/ast.hpp"


namespace loki {

extern pddl::Domain parse(const domain::ast::Domain& domain_node, Context& context);

}

#endif
