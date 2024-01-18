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

#include "function_operators.hpp"


namespace loki {

/* MultiOperator */
pddl::MultiOperatorEnum MultiOperatorVisitor::operator()(const domain::ast::MultiOperatorMul&) const {
    return pddl::MultiOperatorEnum::MUL;
}

pddl::MultiOperatorEnum MultiOperatorVisitor::operator()(const domain::ast::MultiOperatorPlus&) const {
    return pddl::MultiOperatorEnum::PLUS;
}


/* BinaryOperator */
pddl::BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const domain::ast::MultiOperatorMul&) const {
    return pddl::BinaryOperatorEnum::MUL;
}

pddl::BinaryOperatorEnum MultiToBinaryOperatorVisitor::operator()(const domain::ast::MultiOperatorPlus&) const {
    return pddl::BinaryOperatorEnum::PLUS;
}


pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::BinaryOperatorDiv&) const {
    return pddl::BinaryOperatorEnum::DIV;
}

pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::BinaryOperatorMinus&) const {
    return pddl::BinaryOperatorEnum::MINUS;
}

pddl::BinaryOperatorEnum BinaryOperatorVisitor::operator()(const domain::ast::MultiOperator& node) const {
    return boost::apply_visitor(MultiToBinaryOperatorVisitor(), node);
}

}