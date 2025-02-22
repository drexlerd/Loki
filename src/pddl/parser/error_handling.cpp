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
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/parameter.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"
#include "loki/details/pddl/type.hpp"

namespace loki
{

/**
 * Test requirement
 */

template<ParsingContext C>
void test_undefined_requirement(RequirementEnum requirement, const Position& position, const C& context)
{
    if (!context.requirements->test(requirement))
    {
        throw UndefinedRequirementError(requirement, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_requirement(RequirementEnum requirement, const Position& position, const DomainParsingContext& context);
template void test_undefined_requirement(RequirementEnum requirement, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const C& context)
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

template void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const DomainParsingContext& context);
template void test_undefined_requirements(RequirementEnumList requirements, const Position& position, const ProblemParsingContext& context);

/**
 * Test missing definitions
 */

template<ParsingContext C>
void test_undefined_constant(const std::string& constant_name, const Position& position, const C& context)
{
    const auto binding = context.scopes.top().get_object(constant_name);
    if (!binding.has_value())
    {
        throw UndefinedConstantError(constant_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_constant(const std::string& constant_name, const Position& position, const DomainParsingContext& context);
template void test_undefined_constant(const std::string& constant_name, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_object(const std::string& object_name, const Position& position, const C& context)
{
    const auto binding = context.scopes.top().get_object(object_name);
    if (!binding.has_value())
    {
        throw UndefinedObjectError(object_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_object(const std::string& object_name, const Position& position, const DomainParsingContext& context);
template void test_undefined_object(const std::string& object_name, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_variable(const Variable& variable, const Position& position, const C& context)
{
    const auto binding = context.scopes.top().get_variable(variable->get_name());
    if (!binding.has_value())
    {
        throw UndefinedVariableError(variable->get_name(), context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_variable(const Variable& variable, const Position& position, const DomainParsingContext& context);
template void test_undefined_variable(const Variable& variable, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const C& context)
{
    auto binding = context.scopes.top().get_function_skeleton(function_name);
    if (!binding.has_value())
    {
        throw UndefinedFunctionSkeletonError(function_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const DomainParsingContext& context);
template void test_undefined_function_skeleton(const std::string& function_name, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_predicate(const std::string& predicate_name, const Position& position, const C& context)
{
    const auto binding = context.scopes.top().get_predicate(predicate_name);
    if (!binding.has_value())
    {
        throw UndefinedPredicateError(predicate_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_predicate(const std::string& predicate_name, const Position& position, const DomainParsingContext& context);
template void test_undefined_predicate(const std::string& predicate_name, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_undefined_type(const std::string& type_name, const Position& position, const C& context)
{
    auto binding = context.scopes.top().get_type(type_name);
    if (!binding.has_value())
    {
        throw UndefinedTypeError(type_name, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_undefined_type(const std::string& type_name, const Position& position, const DomainParsingContext& context);
template void test_undefined_type(const std::string& type_name, const Position& position, const ProblemParsingContext& context);

/**
 * Test multiple definitions
 */

template<ParsingContext C>
void test_multiple_definition_variable(const Variable& variable, const Position& position, const C& context)
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

template void test_multiple_definition_variable(const Variable& variable, const Position& position, const DomainParsingContext& context);
template void test_multiple_definition_variable(const Variable& variable, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_multiple_definition_constant(const Object& constant, const Position& node, const C& context)
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

template void test_multiple_definition_constant(const Object& constant, const Position& node, const DomainParsingContext& context);
template void test_multiple_definition_constant(const Object& constant, const Position& node, const ProblemParsingContext& context);

template<ParsingContext C>
void test_multiple_definition_object(const Object& object, const Position& node, const C& context)
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

template void test_multiple_definition_object(const Object& object, const Position& node, const DomainParsingContext& context);
template void test_multiple_definition_object(const Object& object, const Position& node, const ProblemParsingContext& context);

template<ParsingContext C>
void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const C& context)
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

template void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const DomainParsingContext& context);
template void test_multiple_definition_predicate(const Predicate& predicate, const Position& node, const ProblemParsingContext& context);

template<ParsingContext C>
void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const C& context)
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

template void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const DomainParsingContext& context);
template void test_multiple_definition_function_skeleton(const FunctionSkeleton& function_skeleton, const Position& node, const ProblemParsingContext& context);

/**
 * Test reserved keyword
 */

template<ParsingContext C>
void test_reserved_type(const std::string& type_name, const Position& node, const C& context)
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

template void test_reserved_type(const std::string& type_name, const Position& node, const DomainParsingContext& context);
template void test_reserved_type(const std::string& type_name, const Position& node, const ProblemParsingContext& context);

/**
 * Test arity mismatch
 */

template<ParsingContext C>
void test_arity_compatibility(size_t arity_1, size_t arity_2, const Position& position, const C& context)
{
    if (arity_1 != arity_2)
    {
        throw IncompatibleArityError(arity_1, arity_2, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_arity_compatibility(size_t arity_1, size_t arity_2, const Position& position, const DomainParsingContext& context);
template void test_arity_compatibility(size_t arity_1, size_t arity_2, const Position& position, const ProblemParsingContext& context);

template<ParsingContext C>
void test_parameter_type_compatibility(const Parameter& specialized_parameter,
                                       const Parameter& generalized_parameter,
                                       const Position& position,
                                       const C& context)
{
    if (!is_specialized_parameter(specialized_parameter, generalized_parameter))
    {
        throw IncompatibleParameterTypesError(specialized_parameter, generalized_parameter, context.scopes.top().get_error_handler()(position, ""));
    }
}

template void test_parameter_type_compatibility(const Parameter& specialized_parameter,
                                                const Parameter& generalized_parameter,
                                                const Position& position,
                                                const DomainParsingContext& context);
template void test_parameter_type_compatibility(const Parameter& specialized_parameter,
                                                const Parameter& generalized_parameter,
                                                const Position& position,
                                                const ProblemParsingContext& context);

/**
 * Test variable initialization
 */

template<ParsingContext C>
void test_nonnegative_number(double number, const Position& position, const C& context)
{
    if (number < 0)
    {
        throw NegativeCostError(context.positions.get_error_handler()(position, ""));
    }
}

template void test_nonnegative_number(double number, const Position& position, const DomainParsingContext& context);
template void test_nonnegative_number(double number, const Position& position, const ProblemParsingContext& context);

/**
 * Test assignment
 */

template<ParsingContext C>
static void test_incompatible_grounding_helper(Parameter parameter, Object object, const Position& position, const C& context)
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
        throw IncompatibleVariableGroundingError(object, parameter->get_variable(), context.scopes.top().get_error_handler()(position, ""));
    }
}

template<ParsingContext C>
void test_incompatible_grounding(const ParameterList& parameters, const TermList& terms, const PositionList& positions, const C& context)
{
    assert(parameters.size() == terms.size());

    for (size_t i = 0; i < parameters.size(); ++i)
    {
        if (const auto object = std::get_if<Object>(&terms[i]->get_object_or_variable()))
        {
            test_incompatible_grounding_helper(parameters[i], *object, positions[i], context);
        }
    }
}

template void
test_incompatible_grounding(const ParameterList& parameters, const TermList& terms, const PositionList& positions, const DomainParsingContext& context);
template void
test_incompatible_grounding(const ParameterList& parameters, const TermList& terms, const PositionList& positions, const ProblemParsingContext& context);

/**
 * Test references
 */

template<ParsingContext C>
void test_variable_references(const ParameterList& parameter_list, const C& context)
{
    if (context.options.strict)
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

template void test_variable_references(const ParameterList& parameter_list, const DomainParsingContext& context);
template void test_variable_references(const ParameterList& parameter_list, const ProblemParsingContext& context);

template<ParsingContext C>
void test_object_references(const ObjectList& object_list, const C& context)
{
    if (context.options.strict)
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

template void test_object_references(const ObjectList& object_list, const DomainParsingContext& context);
template void test_object_references(const ObjectList& object_list, const ProblemParsingContext& context);

template<ParsingContext C>
void test_predicate_references(const PredicateList& predicate_list, const C& context)
{
    if (context.options.strict)
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

template void test_predicate_references(const PredicateList& predicate_list, const DomainParsingContext& context);
template void test_predicate_references(const PredicateList& predicate_list, const ProblemParsingContext& context);

template<ParsingContext C>
void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const C& context)
{
    if (context.options.strict)
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

template void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const DomainParsingContext& context);
template void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const ProblemParsingContext& context);

}