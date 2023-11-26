#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EXCEPTIONS_HPP_

#include "declarations.hpp"
#include "requirements.hpp"

#include "../../common/exceptions.hpp"

#include <stdexcept>


namespace loki {


class UndefinedTypeError : public SemanticParserError {
public:
    UndefinedTypeError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionTypeError : public SemanticParserError {
public:
    MultiDefinitionTypeError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedPredicateError : public SemanticParserError {
public:
    UndefinedPredicateError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionPredicateError : public SemanticParserError {
public:
    MultiDefinitionPredicateError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedConstantError : public SemanticParserError {
public:
    UndefinedConstantError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionConstantError : public SemanticParserError {
public:
    MultiDefinitionConstantError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedVariableError : public SemanticParserError {
public:
    UndefinedVariableError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionVariableError : public SemanticParserError {
public:
    MultiDefinitionVariableError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedFunctionSkeletonError : public SemanticParserError {
public:
    UndefinedFunctionSkeletonError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionFunctionSkeletonError : public SemanticParserError {
public:
    MultiDefinitionFunctionSkeletonError(const std::string& name, const std::string& error_handler_output);
};

class UndefinedRequirementError : public SemanticParserError {
public:
    UndefinedRequirementError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};

class MismatchedPredicateTermListError : public SemanticParserError {
public:
    MismatchedPredicateTermListError(
        const pddl::Predicate& predicate,
        const pddl::TermList& term_list,
        const std::string& error_handler_output);
};

class NotSupportedError : public std::runtime_error {
public:
    explicit NotSupportedError(pddl::RequirementEnum requirement, const std::string& error_handler_output);
};

}

#endif