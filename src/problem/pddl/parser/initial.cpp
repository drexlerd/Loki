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

#include "../../../../include/common/exceptions.hpp"
#include "../../../../include/domain/pddl/exceptions.hpp"
#include "../../../../include/problem/pddl/exceptions.hpp"
#include "../../../domain/pddl/parser/common.hpp"
#include "../../../domain/pddl/parser/functions.hpp"


namespace loki {

std::vector<std::variant<pddl::Literal, pddl::NumericFluent>> parse(
    const problem::ast::Initial& initial_node, Context& context) {
    auto initial_element_list = std::vector<std::variant<pddl::Literal, pddl::NumericFluent>>();
    for (const auto& initial_element : initial_node.initial_elements) {
        initial_element_list.push_back(boost::apply_visitor(InitialElementVisitor(context), initial_element));
    }
    return initial_element_list;
}

std::variant<pddl::Literal, pddl::NumericFluent> parse(const problem::ast::InitialElementLiteral& node, Context& context) {
    return parse(node.literal, context);
}

std::variant<pddl::Literal, pddl::NumericFluent> parse(const problem::ast::InitialElementTimedLiterals& /*node*/, Context& /*context*/) {
    throw NotImplementedError("InitialElementVisitor::operator()(const problem::ast::InitialElementTimedLiterals& node)");
}

std::variant<pddl::Literal, pddl::NumericFluent> parse(const problem::ast::InitialElementNumericFluentsTotalCost& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::ACTION_COSTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::ACTION_COSTS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::ACTION_COSTS);
    const auto function_skeleton = parse_function_skeleton_reference(node.function_symbol_total_cost, context);
    const auto basic_function_term = context.factories.functions.get_or_create<pddl::FunctionImpl>(function_skeleton, pddl::TermList{});
    double number = parse(node.number);
    if (number < 0) {
        throw NegativeCostError(context.positions.get_error_handler()(node.number, ""));
    }
    return context.factories.numeric_fluents.get_or_create<pddl::NumericFluentImpl>(basic_function_term, number);
}

std::variant<pddl::Literal, pddl::NumericFluent> parse(const problem::ast::InitialElementNumericFluentsGeneral& node, Context& context) {
    if (!context.requirements->test(pddl::RequirementEnum::NUMERIC_FLUENTS)) {
        throw UndefinedRequirementError(pddl::RequirementEnum::NUMERIC_FLUENTS, context.scopes.get_error_handler()(node, ""));
    }
    context.references.untrack(pddl::RequirementEnum::NUMERIC_FLUENTS);
    const auto basic_function_term = parse(node.basic_function_term, context);
    double number = parse(node.number);
    if (number < 0 && context.requirements->test(pddl::RequirementEnum::ACTION_COSTS)) {
        throw NegativeCostError(context.positions.get_error_handler()(node.number, ""));
    }
    return context.factories.numeric_fluents.get_or_create<pddl::NumericFluentImpl>(basic_function_term, number);
}

InitialElementVisitor::InitialElementVisitor(Context& context_)
    : context(context_) { }

}
