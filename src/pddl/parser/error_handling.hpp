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

extern void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const Context& context);

/**
 * Test missing definitions
 */

extern void test_undefined_constant(const std::string& constant_name, const Position& position, const Context& context);

extern void test_undefined_object(const std::string& object_name, const Position& position, const Context& context);

extern void test_undefined_variable(const Variable& variable, const Position& position, const Context& context);

extern void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const Context& context);

extern void test_undefined_predicate(const std::string& predicate_name, const Position& position, const Context& context);

extern void test_undefined_type(const std::string& type_name, const Position& position, const Context& context);

/**
 * Test multiple definitions
 */

extern void test_multiple_definition_variable(const Variable& variable, const Position& position, const Context& context);

extern void test_multiple_definition_constant(const Object& constant, const Position& node, const Context& context);

extern void test_multiple_definition_object(const Object& object, const Position& node, const Context& context);

extern void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const Context& context);

extern void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const Context& context);

/**
 * Test reserved keyword
 */

extern void test_reserved_type(const std::string& type_name, const Position& node, const Context& context);

/**
 * Test mismatch
 */

extern void
test_mismatches_arity_between_predicate_and_terms(const Predicate& predicate, const TermList& terms, const Position& position, const Context& context);

extern void test_mismatches_arity_between_function_skeleton_and_terms(const FunctionSkeleton& function_skeleton,
                                                                      const TermList& terms,
                                                                      const Position& position,
                                                                      const Context& context);

extern void test_expected_derived_predicate(const Predicate& predicate, const Position& position, const Context& context);

/**
 * Test variable initialization
 */

extern void test_nonnegative_number(double number, const Position& position, const Context& context);

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
