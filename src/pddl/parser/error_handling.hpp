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

#ifndef LOKI_SRC_PDDL_PARSER_ERROR_HANDLING_HPP_
#define LOKI_SRC_PDDL_PARSER_ERROR_HANDLING_HPP_

#include "loki/details/pddl/context.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/parameter.hpp"

namespace loki
{

/**
 * Test requirement
 */

extern void test_undefined_requirement(RequirementEnum requirement, const Position& position, const Context& context);

/**
 * Test missing definitions
 */

extern void test_undefined_constant(const std::string& constant_name, const Position& position, const Context& context);

extern void test_undefined_variable(const Variable& variable, const Position& position, const Context& context);

extern void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const Context& context);

/**
 * Test multiple definitions
 */

extern void test_multiple_definition_variable(const Variable& variable, const Position& position, const Context& context);

extern void test_multiple_definition_constant(const Object& constant, const Position& node, const Context& context);

/**
 * Test assignment
 */

extern void
test_consistent_object_to_variable_assignment(const ParameterList& parameters, const TermList& terms, const PositionList& positions, const Context& context);

/**
 * Test references
 */

extern void test_variable_references(const ParameterList& parameter_list, const Context& context);

extern void test_object_references(const ObjectList& object_list, const Context& context);

extern void test_predicate_references(const PredicateList& predicate_list, const Context& context);

extern void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const Context& context);

}

#endif