/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_ERRORS_HPP_
#define LOKI_SEMANTIC_ERRORS_HPP_

#include <cstddef>
#include <stdexcept>
#include <string>
#include <utility>

namespace loki::semantic
{

class SemanticError : public std::runtime_error
{
public:
    explicit SemanticError(std::string message) : std::runtime_error(std::move(message)) {}
};

class ParseError : public SemanticError
{
public:
    explicit ParseError(std::string message) : SemanticError(std::move(message)) {}
};

class UnsupportedRequirementError : public SemanticError
{
public:
    explicit UnsupportedRequirementError(const std::string& requirement) :
        SemanticError("Unsupported requirement: :" + requirement)
    {
    }
};

class MissingRequirementError : public SemanticError
{
public:
    explicit MissingRequirementError(const std::string& requirement) :
        SemanticError("Missing required PDDL requirement: :" + requirement)
    {
    }
};

class UndefinedTypeError : public SemanticError
{
public:
    explicit UndefinedTypeError(const std::string& name) : SemanticError("Undefined type: " + name) {}
};

class UndefinedPredicateError : public SemanticError
{
public:
    explicit UndefinedPredicateError(const std::string& name) : SemanticError("Undefined predicate: " + name) {}
};

class UndefinedObjectError : public SemanticError
{
public:
    explicit UndefinedObjectError(const std::string& name) : SemanticError("Undefined object: " + name) {}
};

class UndefinedVariableError : public SemanticError
{
public:
    explicit UndefinedVariableError(const std::string& name) : SemanticError("Undefined variable: ?" + name) {}
};

class UndefinedFunctionError : public SemanticError
{
public:
    explicit UndefinedFunctionError(const std::string& name) : SemanticError("Undefined function: " + name) {}
};

class DuplicateDefinitionError : public SemanticError
{
protected:
    DuplicateDefinitionError(const std::string& kind, const std::string& name) :
        SemanticError("Duplicate " + kind + " definition: " + name)
    {
    }
};

class DuplicateTypeError : public DuplicateDefinitionError
{
public:
    explicit DuplicateTypeError(const std::string& name) : DuplicateDefinitionError("type", name) {}
};

class DuplicatePredicateError : public DuplicateDefinitionError
{
public:
    explicit DuplicatePredicateError(const std::string& name) : DuplicateDefinitionError("predicate", name) {}
};

class DuplicateObjectError : public DuplicateDefinitionError
{
public:
    explicit DuplicateObjectError(const std::string& name) : DuplicateDefinitionError("object", name) {}
};

class DuplicateVariableError : public DuplicateDefinitionError
{
public:
    explicit DuplicateVariableError(const std::string& name) : DuplicateDefinitionError("variable", name) {}
};

class DuplicateFunctionError : public DuplicateDefinitionError
{
public:
    explicit DuplicateFunctionError(const std::string& name) : DuplicateDefinitionError("function", name) {}
};


class ArityMismatchError : public SemanticError
{
public:
    ArityMismatchError(const std::string& name, std::size_t expected, std::size_t actual) :
        SemanticError("Arity mismatch for " + name + ": expected " + std::to_string(expected) + ", got " + std::to_string(actual))
    {
    }
};

class TypeMismatchError : public SemanticError
{
public:
    explicit TypeMismatchError(const std::string& name) :
        SemanticError("Type mismatch for argument of " + name)
    {
    }
};

class InvalidMetricError : public SemanticError
{
public:
    explicit InvalidMetricError(const std::string& optimization) :
        SemanticError("Invalid metric optimization: " + optimization)
    {
    }
};

class InvalidNumericConstraintError : public SemanticError
{
public:
    explicit InvalidNumericConstraintError(const std::string& op) :
        SemanticError("Invalid numeric constraint comparator: " + op)
    {
    }
};

class InvalidNumericEffectError : public SemanticError
{
public:
    explicit InvalidNumericEffectError(const std::string& op) :
        SemanticError("Invalid numeric effect operator: " + op)
    {
    }
};

class InvalidProbabilisticEffectError : public SemanticError
{
public:
    explicit InvalidProbabilisticEffectError(const std::string& message) :
        SemanticError("Invalid probabilistic effect: " + message)
    {
    }
};

class InvalidEqualityError : public SemanticError
{
public:
    explicit InvalidEqualityError(const std::string& message) :
        SemanticError("Invalid equality expression: " + message)
    {
    }
};

class MismatchedDomainError : public SemanticError
{
public:
    MismatchedDomainError(const std::string& expected, const std::string& actual) :
        SemanticError("Task references domain '" + actual + "' but parser holds domain '" + expected + "'.")
    {
    }
};

class MissingDomainError : public SemanticError
{
public:
    explicit MissingDomainError(std::string message) : SemanticError(std::move(message)) {}
};

} // namespace loki::semantic

#endif
