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

#include "loki/details/ast/config.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/requirements_enum.hpp"

namespace loki
{

/**
 * Test requirement
 */

template<ParsingContext C>
extern void test_undefined_requirement(RequirementEnum requirement, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const C& context);

/**
 * Test missing definitions
 */

template<ParsingContext C>
extern void test_undefined_constant(const std::string& constant_name, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_object(const std::string& object_name, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_variable(const Variable& variable, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_predicate(const std::string& predicate_name, const Position& position, const C& context);

template<ParsingContext C>
extern void test_undefined_type(const std::string& type_name, const Position& position, const C& context);

/**
 * Test multiple definitions
 */

template<ParsingContext C>
extern void test_multiple_definition_variable(const Variable& variable, const Position& position, const C& context);

template<ParsingContext C>
extern void test_multiple_definition_constant(const Object& constant, const Position& node, const C& context);

template<ParsingContext C>
extern void test_multiple_definition_object(const Object& object, const Position& node, const C& context);

template<ParsingContext C>
extern void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const C& context);

template<ParsingContext C>
extern void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const C& context);

/**
 * Test reserved keyword
 */

template<ParsingContext C>
extern void test_reserved_type(const std::string& type_name, const Position& node, const C& context);

/**
 * Test variable initialization
 */

template<ParsingContext C>
extern void test_nonnegative_number(double number, const Position& position, const C& context);

/**
 * Test compatibility
 */

template<ParsingContext C>
extern void test_arity_compatibility(size_t arity_1, size_t arity_2, const Position& position, const C& context);

template<ParsingContext C>
extern void
test_parameter_type_compatibility(const Parameter& specialized_parameter, const Parameter& generalized_parameter, const Position& position, const C& context);

template<ParsingContext C>
extern void test_incompatible_grounding(const ParameterList& parameters, const TermList& terms, const PositionList& positions, const C& context);

/**
 * Test references
 */

template<ParsingContext C>
extern void test_variable_references(const ParameterList& parameter_list, const C& context);

template<ParsingContext C>
extern void test_object_references(const ObjectList& object_list, const C& context);

template<ParsingContext C>
extern void test_predicate_references(const PredicateList& predicate_list, const C& context);

template<ParsingContext C>
extern void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const C& context);

}

#endif
