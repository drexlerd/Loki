#ifndef LOKI_INCLUDE_LOKI_COMMON_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_EXCEPTIONS_HPP_

#include <stdexcept>

#include "../domain/pddl/requirements.hpp"


namespace loki {
class SyntaxParserError : public std::runtime_error {
public:
    explicit SyntaxParserError(const std::string& message);
};


class SemanticParserError : public std::runtime_error {
public:
    explicit SemanticParserError(const std::string& message, const std::string& error_handler_output);
};

class UndefinedTypeError : public SemanticParserError {
public:
    UndefinedTypeError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedPredicateError : public SemanticParserError {
public:
    UndefinedPredicateError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedConstantError : public SemanticParserError {
public:
    UndefinedConstantError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedVariableError : public SemanticParserError {
public:
    UndefinedVariableError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedRequirementError : public SemanticParserError {
public:
    UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};


// this exception is still weirdly constructed
/*
class MismatchedArgumentError : public SemanticParserError {
public:
    MismatchedArgumentError(
        const std::string& parameter_name, const std::string& variable_name,
        int parameter_list_arity, int variable_list_arity);
};
*/


class NotImplementedError : public std::runtime_error {
public:
    explicit NotImplementedError(const std::string& message);
};


class NotSupportedError : public std::runtime_error {
public:
    explicit NotSupportedError(const std::string& message);
};

}

#endif