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

#include "loki/details/pddl/exceptions.hpp"

#include "loki/details/pddl/formatter.hpp"

#include <string>

using namespace std::string_literals;

namespace loki
{
/**
 * Domain
 */
/* Type */
ReservedTypeError::ReservedTypeError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("Unexpected type name \""s + name + "\". It is a reserved type name"s, error_handler_output)
{
}

UnusedTypeError::UnusedTypeError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The type with name \""s + name + "\" was never referred to."s, error_handler_output)
{
}

UndefinedTypeError::UndefinedTypeError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The type with name \""s + name + "\" is undefined."s, error_handler_output)
{
}

MultiDefinitionTypeError::MultiDefinitionTypeError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The type with name \""s + name + "\" has already been defined."s, error_handler_output)
{
}

/* Predicate */
UnusedPredicateError::UnusedPredicateError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The predicate with name \""s + name + "\" was never referred to."s, error_handler_output)
{
}

UndefinedPredicateError::UndefinedPredicateError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The predicate with name \""s + name + "\" is undefined."s, error_handler_output)
{
}

MultiDefinitionPredicateError::MultiDefinitionPredicateError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The predicate with name \""s + name + "\" has already been defined."s, error_handler_output)
{
}

/* Constant */
UndefinedConstantError::UndefinedConstantError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The constant with name \""s + name + "\" is undefined."s, error_handler_output)
{
}

MultiDefinitionConstantError::MultiDefinitionConstantError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The constant with name \""s + name + "\" has already been defined."s, error_handler_output)
{
}

/* Variable */
UnusedVariableError::UnusedVariableError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The variable with name \""s + name + "\" was never referred to."s, error_handler_output)
{
}

UndefinedVariableError::UndefinedVariableError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The variable with name \""s + name + "\" is not defined in the current scope."s, error_handler_output)
{
}

MultiDefinitionVariableError::MultiDefinitionVariableError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The variable with name \""s + name + "\" has already been defined."s, error_handler_output)
{
}

/* FunctionSkeleton */
UnusedFunctionSkeletonError::UnusedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The function skeleton with name \""s + name + "\" was never referred to."s, error_handler_output)
{
}

UndefinedFunctionSkeletonError::UndefinedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The function skeleton with name \""s + name + "\" is not defined in the current scope."s, error_handler_output)
{
}

MultiDefinitionFunctionSkeletonError::MultiDefinitionFunctionSkeletonError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The function skeleton with name \""s + name + "\" has already been defined."s, error_handler_output)
{
}

/* Requirement */
UnusedRequirementError::UnusedRequirementError(RequirementEnum requirement, const std::string& error_handler_output) :
    SemanticParserError("The requirement with name \""s + to_string(requirement) + "\" was never used."s, error_handler_output)
{
}

UndefinedRequirementError::UndefinedRequirementError(RequirementEnum requirement, const std::string& error_handler_output) :
    UndefinedRequirementError(RequirementEnumList { requirement }, error_handler_output)
{
}

UndefinedRequirementError::UndefinedRequirementError(RequirementEnumList requirements, const std::string& error_handler_output) :
    SemanticParserError(
        "Undefined requirement: "s + [&requirements]() -> std::string
        {
            auto result = std::string {};
            for (size_t i = 0; i < requirements.size(); ++i)
            {
                if (i != 0)
                {
                    result += " or ";
                }
                result += to_string(requirements[i]);
            }
            return result;
        }(),
        error_handler_output)
{
}

UnsupportedRequirementError::UnsupportedRequirementError(RequirementEnum requirement, const std::string& error_handler_output) :
    std::runtime_error("Unsupported requirement: "s + to_string(requirement) + "\n"s + error_handler_output)
{
}

/**
 * Problem
 */
/* Object */
UnusedObjectError::UnusedObjectError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The object with name \"" + name + "\" was never referred to.", error_handler_output)
{
}

UndefinedObjectError::UndefinedObjectError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The object with name \"" + name + "\" is undefined.", error_handler_output)
{
}

MultiDefinitionObjectError::MultiDefinitionObjectError(const std::string& name, const std::string& error_handler_output) :
    SemanticParserError("The object with name \"" + name + "\" has already been defined.", error_handler_output)
{
}

/* Function*/
IllformedFunctionDefinitionMissingValue::IllformedFunctionDefinitionMissingValue(const FunctionSkeleton& function_skeleton,
                                                                                 const Function& values,
                                                                                 const std::string& error_handler_output) :
    SemanticParserError("The function with name \"" + StreamWriter(*function_skeleton).str()
                            + "\n misses a value definition in the initial state for arguments \"" + StreamWriter(*values).str() + "\".",
                        error_handler_output)
{
}

IllformedFunctionDefinitionMultipleValues::IllformedFunctionDefinitionMultipleValues(const FunctionSkeleton& function_skeleton,
                                                                                     const Function& ground_function,
                                                                                     const std::string& error_handler_output) :
    SemanticParserError("The function with name \"" + StreamWriter(*function_skeleton).str() + "\n has multiple values defined for \""
                            + StreamWriter(*ground_function).str() + "\".",
                        error_handler_output)
{
}

/* Compatibility errors */
IncompatibleParameterTypesError::IncompatibleParameterTypesError(const Parameter& specialized_parameter,
                                                                 const Parameter& generalized_parameter,
                                                                 const std::string& error_handler_output) :
    SemanticParserError("The types of the parameter \""s + StreamWriter(*specialized_parameter).str() + "\" are incompatible with the types of parameter \""
                            + StreamWriter(*generalized_parameter).str() + "\"s",
                        error_handler_output)
{
}

IncompatibleArityError::IncompatibleArityError(const size_t arity_1, const size_t arity_2, const std::string& error_handler_output) :
    SemanticParserError("Mismatched arity "s + std::to_string(arity_1) + "!="s + std::to_string(arity_2) + "."s, error_handler_output)
{
}

IncompatibleVariableGroundingError::IncompatibleVariableGroundingError(const Object& object,
                                                                       const Variable& variable,
                                                                       const std::string& error_handler_output) :
    SemanticParserError("The object with name \"" + object->get_name() + "\" does not satisfy the type requirement of variable with name \""
                            + variable->get_name() + "\".",
                        error_handler_output)
{
}

MismatchedDomainError::MismatchedDomainError(const Domain& domain, const std::string& domain_name, const std::string& error_handler_output) :
    SemanticParserError("Mismatched domain names \"" + domain->get_name() + "!=" + domain_name + ".", error_handler_output)
{
}

NegativeCostError::NegativeCostError(const std::string& error_handler_output) :
    SemanticParserError("Negative numbers in initial numeric fluents are not allowed with :action-costs", error_handler_output)
{
}
}