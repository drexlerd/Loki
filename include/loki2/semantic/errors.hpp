/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_ERRORS_HPP_
#define LOKI2_SEMANTIC_ERRORS_HPP_

#include "loki2/parser/error_handler.hpp"

#include <optional>
#include <stdexcept>
#include <string>

namespace loki2::semantic
{

enum class SemanticErrorCode
{
    Generic,
    ParseFailure,
    UnsupportedRequirement,
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
};

class UnsupportedRequirementError : public SemanticError
{
public:
    explicit UnsupportedRequirementError(const std::string& requirement) :
        SemanticError(SemanticErrorCode::UnsupportedRequirement, "Unsupported requirement: :" + requirement)
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

class MismatchedDomainError : public SemanticError
{
public:
    MismatchedDomainError(const std::string& expected, const std::string& actual) :
        SemanticError(SemanticErrorCode::MismatchedDomain,
                      "Task references domain '" + actual + "' but parser holds domain '" + expected + "'.")
    {
    }
};

} // namespace loki2::semantic

#endif
