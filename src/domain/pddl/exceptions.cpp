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

#include "../../../include/loki/domain/pddl/exceptions.hpp"

#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/domain.hpp"
#include "../../../include/loki/domain/pddl/function_skeleton.hpp"


namespace loki {
/* Type */
UnusedTypeError::UnusedTypeError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The type with name \"" + name + "\" was never referred to.", error_handler_output) { }

UndefinedTypeError::UndefinedTypeError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The type with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionTypeError::MultiDefinitionTypeError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The type with name \"" + name + "\" has already been defined.", error_handler_output) { }

/* Predicate */
UnusedPredicateError::UnusedPredicateError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The predicate with name \"" + name + "\" was never referred to.", error_handler_output) { }

UndefinedPredicateError::UndefinedPredicateError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The predicate with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionPredicateError::MultiDefinitionPredicateError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The predicate with name \"" + name + "\" has already been defined.", error_handler_output) { }

/* Constant */
UndefinedConstantError::UndefinedConstantError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The constant with name \"" + name + "\" is undefined.", error_handler_output) { }

MultiDefinitionConstantError::MultiDefinitionConstantError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The constant with name \"" + name + "\" has already been defined.", error_handler_output) { }

/* Variable */
UnusedVariableError::UnusedVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" was never referred to.", error_handler_output) { }

UndefinedVariableError::UndefinedVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" is not defined in the current scope.", error_handler_output) { }

MultiDefinitionVariableError::MultiDefinitionVariableError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The variable with name \"" + name + "\" has already been defined.", error_handler_output) { }

/* FunctionSkeleton */
UnusedFunctionSkeletonError::UnusedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The function skeleton with name \"" + name + "\" was never referred to.", error_handler_output) { }

UndefinedFunctionSkeletonError::UndefinedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The function skeleton with name \"" + name + "\" is not defined in the current scope.", error_handler_output) { }

MultiDefinitionFunctionSkeletonError::MultiDefinitionFunctionSkeletonError(const std::string& name, const std::string& error_handler_output)
    : SemanticParserError(
        "The function skeleton with name \"" + name + "\" has already been defined.", error_handler_output) { }

/* Requirement */
UnusedRequirementError::UnusedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output)
    : SemanticParserError(
        "The requirement with name \"" + to_string(requirement) + "\" was never used.", error_handler_output) { }

UndefinedRequirementError::UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output)
    : SemanticParserError("Undefined requirement: " + to_string(requirement), error_handler_output) { }

UnsupportedRequirementError::UnsupportedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output)
    : std::runtime_error("Unsupported requirement: " + to_string(requirement) + "\n" + error_handler_output) { }

/* Compatibility errors */
MismatchedPredicateTermListError::MismatchedPredicateTermListError(
    const pddl::Predicate& predicate,
    const pddl::TermList& term_list,
    const std::string& error_handler_output)
    : SemanticParserError(
        "Mismatched number of terms for predicate \""
            + predicate->get_name()
            + "\" with sizes "
            + std::to_string(predicate->get_parameters().size())
            + "!="
            + std::to_string(term_list.size())
            + ".",
        error_handler_output) { }

MismatchedFunctionSkeletonTermListError::MismatchedFunctionSkeletonTermListError(
    const pddl::FunctionSkeleton& function_skeleton,
    const pddl::TermList& term_list,
    const std::string& error_handler_output)
    : SemanticParserError(
        "Mismatched number of terms for function skeleton \""
            + function_skeleton->get_name()
            + "\" with sizes "
            + std::to_string(function_skeleton->get_parameters().size())
            + "!="
            + std::to_string(term_list.size())
            + ".",
        error_handler_output) { }


}