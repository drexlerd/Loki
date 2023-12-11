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

#include "initial.hpp"

#include "literal.hpp"

#include "../../../../include/loki/common/exceptions.hpp"


namespace loki {

InitialElementVisitor::InitialElementVisitor(Context& context_)
    : context(context_) { }

boost::variant<pddl::Literal> InitialElementVisitor::operator()(const problem::ast::InitialElementLiteral& node) {
    return parse(node.literal, context);
}

boost::variant<pddl::Literal> InitialElementVisitor::operator()(const problem::ast::InitialElementTimedLiterals& node) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementTimedLiterals& node)");
}

boost::variant<pddl::Literal> InitialElementVisitor::operator()(const problem::ast::InitialElementNumericFluents& node) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementNumericFluents& node)");
}

boost::variant<pddl::Literal> InitialElementVisitor::operator()(const problem::ast::InitialElementObjectFluents& node) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementObjectFluents& node)");
}


std::vector<boost::variant<pddl::Literal>> parse(
    const problem::ast::Initial& initial_node, Context& context) {
    std::vector<boost::variant<pddl::Literal>> initial_element_list;
    for (const auto& initial_element : initial_node.initial_elements) {
        initial_element_list.push_back(boost::apply_visitor(InitialElementVisitor(context), initial_element));
    }
    return initial_element_list;
}

}
