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

#ifndef LOKI_SRC_PDDL_PARSER_PARAMETERS_HPP_
#define LOKI_SRC_PDDL_PARSER_PARAMETERS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"

namespace loki
{

template<ParsingContext C>
struct ParameterListVisitor : boost::static_visitor<ParameterList>
{
    C& context;

    ParameterListVisitor(C& context_);

    ParameterList operator()(const std::vector<ast::Variable>& nodes);

    ParameterList operator()(const ast::TypedListOfVariablesRecursively& node);
};

}

#endif
