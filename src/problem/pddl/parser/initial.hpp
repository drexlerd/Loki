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

#ifndef LOKI_SRC_PROBLEM_PDDL_PARSER_INITIAL_HPP_
#define LOKI_SRC_PROBLEM_PDDL_PARSER_INITIAL_HPP_

#include "../../../../include/problem/ast/ast.hpp"
#include "../../../../include/problem/pddl/parser.hpp"


namespace loki {

class InitialElementVisitor : boost::static_visitor<boost::variant<pddl::Literal, pddl::NumericFluent>> {
private:
    Context& context;

public:
    InitialElementVisitor(Context& context_);

    boost::variant<pddl::Literal, pddl::NumericFluent> operator()(const problem::ast::InitialElementLiteral& node);

    boost::variant<pddl::Literal, pddl::NumericFluent> operator()(const problem::ast::InitialElementTimedLiterals& node);

    boost::variant<pddl::Literal, pddl::NumericFluent> operator()(const problem::ast::InitialElementNumericFluentsTotalCost& node);

    boost::variant<pddl::Literal, pddl::NumericFluent> operator()(const problem::ast::InitialElementNumericFluentsGeneral& node);
};


extern std::vector<boost::variant<pddl::Literal, pddl::NumericFluent>> parse(
    const problem::ast::Initial& initial_node, Context& context);

}

#endif
