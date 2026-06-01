/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_ERRORS_HPP_
#define LOKI_SEMANTIC_ERRORS_HPP_

#include "loki/parser/error_handler.hpp"

#include <optional>
#include <stdexcept>
#include <string>

namespace loki::semantic
{

enum class SemanticErrorCode
{
    Generic,
    ParseFailure,
    UnsupportedRequirement,
    MissingRequirement,
    UndefinedType,
    UndefinedPredicate,
    UndefinedObject,
    UndefinedVariable,
    UndefinedFunction,
    DuplicateType,
    DuplicatePredicate,
    DuplicateObject,
    DuplicateVariable,
    DuplicateFunction,
    DuplicateAction,
    ArityMismatch,
    TypeMismatch,
    InvalidMetric,
    InvalidNumericConstraint,
    InvalidNumericEffect,
    InvalidProbabilisticEffect,
    InvalidEquality,
    MismatchedDomain,
    MissingDomain,
};

class SemanticError : public std::runtime_error
{
public:
    SemanticError(SemanticErrorCode code, std::string message) :
        std::runtime_error(message),
        m_code(code)
    {
    }

    SemanticError(SemanticErrorCode code, std::string message, parser::SourceRange source_range) :
        std::runtime_error(std::move(message)),
        m_code(code),
        m_source_range(source_range)
    {
    }

    explicit SemanticError(std::string message) :
        SemanticError(SemanticErrorCode::Generic, std::move(message))
    {
    }

    SemanticErrorCode code() const noexcept { return m_code; }
    const std::optional<parser::SourceRange>& source_range() const noexcept { return m_source_range; }
    void set_source_range(parser::SourceRange source_range) { m_source_range = source_range; }

private:
    SemanticErrorCode m_code;
    std::optional<parser::SourceRange> m_source_range;
};

class ParseError : public SemanticError
{
public:
    explicit ParseError(std::string message) : SemanticError(SemanticErrorCode::ParseFailure, std::move(message)) {}
    ParseError(std::string message, parser::SourceRange source_range) :
        SemanticError(SemanticErrorCode::ParseFailure, std::move(message), source_range)
    {
    }
};

class UnsupportedRequirementError : public SemanticError
{
public:
    explicit UnsupportedRequirementError(const std::string& requirement) :
        SemanticError(SemanticErrorCode::UnsupportedRequirement, "Unsupported requirement: :" + requirement)
    {
    }
};

class MissingRequirementError : public SemanticError
{
public:
    explicit MissingRequirementError(const std::string& requirement) :
        SemanticError(SemanticErrorCode::MissingRequirement, "Missing required PDDL requirement: :" + requirement)
    {
    }
};

class UndefinedTypeError : public SemanticError
{
public:
    explicit UndefinedTypeError(const std::string& name) : SemanticError(SemanticErrorCode::UndefinedType, "Undefined type: " + name) {}
};

class UndefinedPredicateError : public SemanticError
{
public:
    explicit UndefinedPredicateError(const std::string& name) : SemanticError(SemanticErrorCode::UndefinedPredicate, "Undefined predicate: " + name) {}
};

class UndefinedObjectError : public SemanticError
{
public:
    explicit UndefinedObjectError(const std::string& name) : SemanticError(SemanticErrorCode::UndefinedObject, "Undefined object: " + name) {}
};

class UndefinedVariableError : public SemanticError
{
public:
    explicit UndefinedVariableError(const std::string& name) : SemanticError(SemanticErrorCode::UndefinedVariable, "Undefined variable: ?" + name) {}
};

class UndefinedFunctionError : public SemanticError
{
public:
    explicit UndefinedFunctionError(const std::string& name) : SemanticError(SemanticErrorCode::UndefinedFunction, "Undefined function: " + name) {}
};

class DuplicateDefinitionError : public SemanticError
{
public:
    DuplicateDefinitionError(SemanticErrorCode code, const std::string& kind, const std::string& name) :
        SemanticError(code, "Duplicate " + kind + " definition: " + name)
    {
    }
};

class ArityMismatchError : public SemanticError
{
public:
    ArityMismatchError(const std::string& name, std::size_t expected, std::size_t actual) :
        SemanticError(SemanticErrorCode::ArityMismatch,
                      "Arity mismatch for " + name + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual))
    {
    }
};

class TypeMismatchError : public SemanticError
{
public:
    explicit TypeMismatchError(const std::string& name) :
        SemanticError(SemanticErrorCode::TypeMismatch, "Type mismatch for argument of " + name)
    {
    }
};

class InvalidMetricError : public SemanticError
{
public:
    explicit InvalidMetricError(const std::string& optimization) :
        SemanticError(SemanticErrorCode::InvalidMetric, "Invalid metric optimization: " + optimization)
    {
    }
};

class InvalidNumericConstraintError : public SemanticError
{
public:
    explicit InvalidNumericConstraintError(const std::string& op) :
        SemanticError(SemanticErrorCode::InvalidNumericConstraint, "Invalid numeric constraint comparator: " + op)
    {
    }
};

class InvalidNumericEffectError : public SemanticError
{
public:
    explicit InvalidNumericEffectError(const std::string& op) :
        SemanticError(SemanticErrorCode::InvalidNumericEffect, "Invalid numeric effect operator: " + op)
    {
    }
};

class InvalidProbabilisticEffectError : public SemanticError
{
public:
    explicit InvalidProbabilisticEffectError(const std::string& message) :
        SemanticError(SemanticErrorCode::InvalidProbabilisticEffect, "Invalid probabilistic effect: " + message)
    {
    }
};

class InvalidEqualityError : public SemanticError
{
public:
    explicit InvalidEqualityError(const std::string& message) :
        SemanticError(SemanticErrorCode::InvalidEquality, "Invalid equality expression: " + message)
    {
    }
};

class MismatchedDomainError : public SemanticError
{
public:
    MismatchedDomainError(const std::string& expected, const std::string& actual) :
        SemanticError(SemanticErrorCode::MismatchedDomain,
                      "Task references domain '" + actual + "' but parser holds domain '" + expected + "'.")
    {
    }
};

} // namespace loki::semantic

#endif
