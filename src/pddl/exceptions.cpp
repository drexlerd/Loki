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

#include "loki/pddl/exceptions.hpp"

#include "loki/pddl/domain.hpp"
#include "loki/pddl/function_skeleton.hpp"
#include "loki/pddl/predicate.hpp"

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
UnusedRequirementError::UnusedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output) :
    SemanticParserError("The requirement with name \""s + to_string(requirement) + "\" was never used."s, error_handler_output)
{
}

UndefinedRequirementError::UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output) :
    SemanticParserError("Undefined requirement: "s + to_string(requirement), error_handler_output)
{
}

UnsupportedRequirementError::UnsupportedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output) :
    std::runtime_error("Unsupported requirement: "s + to_string(requirement) + "\n"s + error_handler_output)
{
}

/* Compatibility errors */
MismatchedPredicateTermListError::MismatchedPredicateTermListError(const pddl::Predicate& predicate,
                                                                   const pddl::TermList& term_list,
                                                                   const std::string& error_handler_output) :
    SemanticParserError("Mismatched number of terms for predicate \""s + predicate->get_name() + "\" with sizes "s
                            + std::to_string(predicate->get_parameters().size()) + "!="s + std::to_string(term_list.size()) + "."s,
                        error_handler_output)
{
}

MismatchedFunctionSkeletonTermListError::MismatchedFunctionSkeletonTermListError(const pddl::FunctionSkeleton& function_skeleton,
                                                                                 const pddl::TermList& term_list,
                                                                                 const std::string& error_handler_output) :
    SemanticParserError("Mismatched number of terms for function skeleton \""s + function_skeleton->get_name() + "\" with sizes "s
                            + std::to_string(function_skeleton->get_parameters().size()) + "!="s + std::to_string(term_list.size()) + ".",
                        error_handler_output)
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

/* Compatibility errors */
MismatchedDomainError::MismatchedDomainError(const pddl::Domain& domain, const std::string& domain_name, const std::string& error_handler_output) :
    SemanticParserError("Mismatched domain names \"" + domain->get_name() + "!=" + domain_name + ".", error_handler_output)
{
}

MismatchedPredicateObjectListError::MismatchedPredicateObjectListError(const pddl::Predicate& predicate,
                                                                       const pddl::ObjectList& object_list,
                                                                       const std::string& error_handler_output) :
    SemanticParserError("Mismatched number of terms for predicate \"" + predicate->get_name() + "\" with sizes "
                            + std::to_string(predicate->get_parameters().size()) + "!=" + std::to_string(object_list.size()) + ".",
                        error_handler_output)
{
}

NegativeCostError::NegativeCostError(const std::string& error_handler_output) :
    SemanticParserError("Negative numbers in initial numeric fluents are not allowed with :action-costs", error_handler_output)
{
}
}