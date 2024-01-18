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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EXCEPTIONS_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include "../../common/exceptions.hpp"

#include <stdexcept>


namespace loki {

/* Type */
class ReservedTypeError : public SemanticParserError {
public:
    ReservedTypeError(const std::string& name, const std::string& error_handler_output);
};

class UnusedTypeError : public SemanticParserError {
public:
    UnusedTypeError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedTypeError : public SemanticParserError {
public:
    UndefinedTypeError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionTypeError : public SemanticParserError {
public:
    MultiDefinitionTypeError(const std::string& name, const std::string& error_handler_output);
};

/* Predicate */
class UnusedPredicateError : public SemanticParserError {
public:
    UnusedPredicateError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedPredicateError : public SemanticParserError {
public:
    UndefinedPredicateError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionPredicateError : public SemanticParserError {
public:
    MultiDefinitionPredicateError(const std::string& name, const std::string& error_handler_output);
};

/* Constant */
// UnusedConstantError is not a valid exception because constants are not required to be used in all problems
class UndefinedConstantError : public SemanticParserError {
public:
    UndefinedConstantError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionConstantError : public SemanticParserError {
public:
    MultiDefinitionConstantError(const std::string& name, const std::string& error_handler_output);
};

/* Variable */
class UnusedVariableError : public SemanticParserError {
public:
    UnusedVariableError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedVariableError : public SemanticParserError {
public:
    UndefinedVariableError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionVariableError : public SemanticParserError {
public:
    MultiDefinitionVariableError(const std::string& name, const std::string& error_handler_output);
};

/* FunctionSkeleton */
class UnusedFunctionSkeletonError : public SemanticParserError {
public:
    UnusedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedFunctionSkeletonError : public SemanticParserError {
public:
    UndefinedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionFunctionSkeletonError : public SemanticParserError {
public:
    MultiDefinitionFunctionSkeletonError(const std::string& name, const std::string& error_handler_output);
};

/* Requirement */
class UnusedRequirementError : public SemanticParserError {
public:
    UnusedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};

class UndefinedRequirementError : public SemanticParserError {
public:
    UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};

class UnsupportedRequirementError : public std::runtime_error {
public:
    explicit UnsupportedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};


/* Compatibility errors */
class MismatchedPredicateTermListError : public SemanticParserError {
public:
    MismatchedPredicateTermListError(
        const pddl::Predicate& predicate,
        const pddl::TermList& term_list,
        const std::string& error_handler_output);
};

class MismatchedFunctionSkeletonTermListError : public SemanticParserError {
public:
    MismatchedFunctionSkeletonTermListError(
        const pddl::FunctionSkeleton& function_skeleton,
        const pddl::TermList& term_list,
        const std::string& error_handler_output);
};

class MismatchedFunctionSkeletonObjectListError : public SemanticParserError {
public:
    MismatchedFunctionSkeletonObjectListError(
        const pddl::FunctionSkeleton& function_skeleton,
        const pddl::ObjectList& object_list,
        const std::string& error_handler_output);
};

}

#endif