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

#include "loki/ast/ast.hpp"
#include "loki/ast/config.hpp"
#include "loki/pddl/parameter.hpp"
#include "loki/pddl/parser.hpp"

namespace loki
{

/* ParameterList */
class ParameterListVisitor : boost::static_visitor<pddl::ParameterList>
{
private:
    Context& context;

public:
    ParameterListVisitor(Context& context_);

    pddl::ParameterList operator()(const std::vector<ast::Variable>& nodes);

    pddl::ParameterList operator()(const ast::TypedListOfVariablesRecursively& node);
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_
