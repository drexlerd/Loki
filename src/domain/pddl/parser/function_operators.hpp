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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTION_OPERATORS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTION_OPERATORS_HPP_

#include <loki/domain/ast/ast.hpp>
#include <loki/domain/pddl/parser.hpp>


namespace loki {
/* MultiOperator */
struct MultiOperatorVisitor : boost::static_visitor<pddl::MultiOperatorEnum> {
    pddl::MultiOperatorEnum operator()(const domain::ast::MultiOperatorMul& node) const;
    pddl::MultiOperatorEnum operator()(const domain::ast::MultiOperatorPlus& node) const;
};


/* BinaryOperator */
struct MultiToBinaryOperatorVisitor : boost::static_visitor<pddl::BinaryOperatorEnum> {
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperatorMul& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperatorPlus& node) const;
};

struct BinaryOperatorVisitor : boost::static_visitor<pddl::BinaryOperatorEnum> {
    pddl::BinaryOperatorEnum operator()(const domain::ast::BinaryOperatorDiv& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::BinaryOperatorMinus& node) const;
    pddl::BinaryOperatorEnum operator()(const domain::ast::MultiOperator& node) const;
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
