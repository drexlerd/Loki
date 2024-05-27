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

#include "error_handling.hpp"

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/exceptions.hpp"

namespace loki
{

/**
 * Test requirement
 */

void test_undefined_requirement(RequirementEnum requirement, const Position& position, const Context& context)
{
    if (!context.requirements->test(requirement))
    {
        throw UndefinedRequirementError(requirement, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const Context& context)
{
    bool found = false;
    for (const auto& requirement : requirements)
    {
        if (context.requirements->test(requirement))
        {
            found = true;
        }
    }
    if (!found)
    {
        throw UndefinedRequirementError(requirements, context.scopes.top().get_error_handler()(position, ""));
    }
}

/**
 * Test missing definitions
 */

void test_undefined_constant(const std::string& constant_name, const Position& position, const Context& context)
{
    const auto binding = context.scopes.top().get_object(constant_name);
    if (!binding.has_value())
    {
        throw UndefinedConstantError(constant_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_object(const std::string& object_name, const Position& position, const Context& context)
{
    const auto binding = context.scopes.top().get_object(object_name);
    if (!binding.has_value())
    {
        throw UndefinedObjectError(object_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_variable(const Variable& variable, const Position& position, const Context& context)
{
    const auto binding = context.scopes.top().get_variable(variable->get_name());
    if (!binding.has_value())
    {
        throw UndefinedVariableError(variable->get_name(), context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const Context& context)
{
    auto binding = context.scopes.top().get_function_skeleton(function_name);
    if (!binding.has_value())
    {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_predicate(const std::string& predicate_name, const Position& position, const Context& context)
{
    const auto binding = context.scopes.top().get_predicate(predicate_name);
    if (!binding.has_value())
    {
        throw UndefinedPredicateError(predicate_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_undefined_type(const std::string& type_name, const Position& position, const Context& context)
{
    auto binding = context.scopes.top().get_type(type_name);
    if (!binding.has_value())
    {
        throw UndefinedTypeError(type_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

/**
 * Test multiple definitions
 */

void test_multiple_definition_variable(const Variable& variable, const Position& position, const Context& context)
{
    const auto binding = context.scopes.top().get_variable(variable->get_name());
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.top().get_error_handler()(position, "Defined here:");
        const auto [_constant, position, error_handler] = binding.value();
        assert(position.has_value());
        const auto message_2 = error_handler(position.value(), "First defined here:");
        throw MultiDefinitionVariableError(variable->get_name(), message_1 + message_2);
    }
}

void test_multiple_definition_constant(const Object& constant, const Position& node, const Context& context)
{
    const auto constant_name = constant->get_name();
    const auto binding = context.scopes.top().get_object(constant_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.top().get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_object, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionConstantError(constant_name, message_1 + message_2);
    }
}

void test_multiple_definition_object(const Object& object, const Position& node, const Context& context)
{
    const auto object_name = object->get_name();
    const auto binding = context.scopes.top().get_object(object_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.top().get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_object, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionObjectError(object_name, message_1 + message_2);
    }
}

void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const Context& context)
{
    const auto predicate_name = predicate->get_name();
    const auto binding = context.scopes.top().get_predicate(predicate_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.top().get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_predicate, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionPredicateError(predicate_name, message_1 + message_2);
    }
}

void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const Context& context)
{
    const auto function_name = function_skeleton->get_name();
    const auto binding = context.scopes.top().get_function_skeleton(function_name);
    if (binding.has_value())
    {
        const auto message_1 = context.scopes.top().get_error_handler()(node, "Defined here:");
        auto message_2 = std::string("");
        const auto [_function_skeleton, position, error_handler] = binding.value();
        if (position.has_value())
        {
            message_2 = error_handler(position.value(), "First defined here:");
        }
        throw MultiDefinitionFunctionSkeletonError(function_name, message_1 + message_2);
    }
}

/**
 * Test reserved keyword
 */

void test_reserved_type(const std::string& type_name, const Position& node, const Context& context)
{
    if (type_name == "object")
    {
        throw ReservedTypeError("object", context.scopes.top().get_error_handler()(node, ""));
    }
    // We also reserve type name number although PDDL specification allows it.
    // However, this allows using regular types as function types for simplicity.
    if (type_name == "number")
    {
        throw ReservedTypeError("number", context.scopes.top().get_error_handler()(node, ""));
    }
}

/**
 * Test arity mismatch
 */

void test_mismatches_arity_between_predicate_and_terms(const Predicate& predicate, const TermList& terms, const Position& position, const Context& context)
{
    if (predicate->get_parameters().size() != terms.size())
    {
        throw MismatchedPredicateTermListError(predicate, terms, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_mismatches_arity_between_function_skeleton_and_terms(const FunctionSkeleton& function_skeleton,
                                                               const TermList& terms,
                                                               const Position& position,
                                                               const Context& context)
{
    if (function_skeleton->get_parameters().size() != terms.size())
    {
        throw MismatchedFunctionSkeletonTermListError(function_skeleton, terms, context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_expected_derived_predicate(const Predicate& predicate, const Position& position, const Context& context)
{
    if (context.derived_predicates.count(predicate) == 0)
    {
        throw ExpectedDerivedPredicate(predicate->get_name(), context.scopes.top().get_error_handler()(position, ""));
    }
}

/**
 * Test variable initialization
 */

void test_nonnegative_number(double number, const Position& position, const Context& context)
{
    if (number < 0)
    {
        throw NegativeCostError(context.positions.get_error_handler()(position, ""));
    }
}

/**
 * Test assignment
 */

static void test_object_type_consistent_with_variable(const Parameter& parameter, const Object& object, const Position& position, const Context& context)
{
    // Object type must match any of those types.
    const auto& parameter_types = TypeSet(parameter->get_bases().begin(), parameter->get_bases().end());
    bool is_consistent = false;
    for (const auto& type : collect_types_from_hierarchy(object->get_bases()))
    {
        if (parameter_types.count(type))
        {
            is_consistent = true;
            break;
        }
    }
    if (!is_consistent)
    {
        throw IncompatibleObjectToVariableError(object, parameter->get_variable(), context.scopes.top().get_error_handler()(position, ""));
    }
}

void test_consistent_object_to_variable_assignment(const ParameterList& parameters,
                                                   const TermList& terms,
                                                   const PositionList& positions,
                                                   const Context& context)
{
    assert(parameters.size() == terms.size());

    for (size_t i = 0; i < parameters.size(); ++i)
    {
        if (const auto term_object = std::get_if<TermObjectImpl>(terms[i]))
        {
            test_object_type_consistent_with_variable(parameters[i], term_object->get_object(), positions[i], context);
        }
    }
}

/**
 * Test references
 */

void test_variable_references(const ParameterList& parameter_list, const Context& context)
{
    if (context.strict)
    {
        for (const auto& parameter : parameter_list)
        {
            if (context.references.exists(parameter->get_variable()))
            {
                const auto [variable, position, error_handler] = context.scopes.top().get_variable(parameter->get_variable()->get_name()).value();
                throw UnusedVariableError(variable->get_name(), error_handler(position.value(), ""));
            }
        }
    }
}

void test_object_references(const ObjectList& object_list, const Context& context)
{
    if (context.strict)
    {
        for (const auto& object : object_list)
        {
            if (context.references.exists(object))
            {
                const auto [_object, position, error_handler] = context.scopes.top().get_object(object->get_name()).value();
                throw UnusedObjectError(object->get_name(), error_handler(position.value(), ""));
            }
        }
    }
}

void test_predicate_references(const PredicateList& predicate_list, const Context& context)
{
    if (context.strict)
    {
        for (const auto& predicate : predicate_list)
        {
            if (context.references.exists(predicate))
            {
                const auto [_predicate, position, error_handler] = context.scopes.top().get_predicate(predicate->get_name()).value();
                throw UnusedPredicateError(predicate->get_name(), error_handler(position.value(), ""));
            }
        }
    }
}

void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const Context& context)
{
    if (context.strict)
    {
        for (const auto& function_skeleton : function_skeleton_list)
        {
            if (context.references.exists(function_skeleton))
            {
                const auto [_function_skeleton, position, error_handler] = context.scopes.top().get_function_skeleton(function_skeleton->get_name()).value();
                throw UnusedFunctionSkeletonError(function_skeleton->get_name(), error_handler(position.value(), ""));
            }
        }
    }
}

}