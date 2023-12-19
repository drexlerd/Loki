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
#include "function.hpp"

#include "../../../../include/loki/common/exceptions.hpp"
#include "../../../../include/loki/domain/pddl/exceptions.hpp"
#include "../../../domain/pddl/parser/common.hpp"



namespace loki {

InitialElementVisitor::InitialElementVisitor(Context& context_)
    : context(context_) { }

boost::variant<pddl::Literal, pddl::NumericFluent> InitialElementVisitor::operator()(const problem::ast::InitialElementLiteral& node) {
    return parse(node.literal, context);
}

boost::variant<pddl::Literal, pddl::NumericFluent> InitialElementVisitor::operator()(const problem::ast::InitialElementTimedLiterals& node) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementTimedLiterals& node)");
}

boost::variant<pddl::Literal, pddl::NumericFluent> InitialElementVisitor::operator()(const problem::ast::InitialElementNumericFluents& node) {
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    context.referenced_values.untrack(pddl::RequirementEnum::NUMERIC_FLUENTS);
    const auto basic_function_term = parse(node.basic_function_term, context);
    double number = parse(node.number);
    return context.factories.numeric_fluents.get_or_create<pddl::NumericFluentImpl>(basic_function_term, number);
}

boost::variant<pddl::Literal, pddl::NumericFluent> InitialElementVisitor::operator()(const problem::ast::InitialElementObjectFluents& node) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementObjectFluents& node)");
}


std::vector<boost::variant<pddl::Literal, pddl::NumericFluent>> parse(
    const problem::ast::Initial& initial_node, Context& context) {
    std::vector<boost::variant<pddl::Literal, pddl::NumericFluent>> initial_element_list;
    for (const auto& initial_element : initial_node.initial_elements) {
        initial_element_list.push_back(boost::apply_visitor(InitialElementVisitor(context), initial_element));
    }
    return initial_element_list;
}

}
