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

#include "common.hpp"
#include "error_handling.hpp"
#include "functions.hpp"
#include "ground_literal.hpp"
#include "literal.hpp"
#include "loki/details/pddl/exceptions.hpp"

namespace loki
{

std::vector<std::variant<Literal, NumericFluent>> parse(const ast::Initial& initial_node, Context& context)
{
    // TODO: IllformedFunctionDefinitionMissingValue and IllformedFunctionDefinitionMultipleValues using value_definitions.
    std::unordered_map<FunctionSkeleton, std::unordered_set<Function>> value_definitions;

    auto initial_element_list = std::vector<std::variant<Literal, NumericFluent>>();
    for (const auto& initial_element : initial_node.initial_elements)
    {
        initial_element_list.push_back(boost::apply_visitor(InitialElementVisitor(context), initial_element));
    }
    return initial_element_list;
}

std::variant<Literal, NumericFluent> parse(const ast::InitialElementLiteral& node, Context& context) { return parse(node.literal, context); }

std::variant<Literal, NumericFluent> parse(const ast::InitialElementTimedLiteral& /*node*/, Context& /*context*/)
{
    throw NotImplementedError("InitialElementVisitor::operator()(const ast::InitialElementTimedLiteral& node)");
}

std::variant<Literal, NumericFluent> parse(const ast::InitialElementNumericFluent& node, Context& context)
{
    test_undefined_requirements(RequirementEnumList { RequirementEnum::ACTION_COSTS, RequirementEnum::NUMERIC_FLUENTS }, node, context);
    context.references.untrack(RequirementEnum::ACTION_COSTS);
    context.references.untrack(RequirementEnum::NUMERIC_FLUENTS);

    const auto basic_function_term = parse(node.basic_function_term, context);
    double number = parse(node.number);
    test_nonnegative_number(number, node.number, context);
    return context.factories.get_or_create_numeric_fluent(basic_function_term, number);
}

InitialElementVisitor::InitialElementVisitor(Context& context_) : context(context_) {}

}
