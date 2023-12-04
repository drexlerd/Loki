#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_EXCEPTIONS_HPP_

#include "declarations.hpp"
#include "../../common/exceptions.hpp"

#include <stdexcept>


namespace loki {

class ObjectIsConstantError : public SemanticParserError {
public:
    ObjectIsConstantError(const std::string& name, const std::string& error_handler_output);
};

class MismatchedDomainError : public SemanticParserError {
public:
    MismatchedDomainError(
        const pddl::Domain& domain,
        const std::string& domain_name,
        const std::string& error_handler_output);
};

class UndefinedObjectError : public SemanticParserError {
public:
    UndefinedObjectError(const std::string& name, const std::string& error_handler_output);
};

class MultiDefinitionObjectError : public SemanticParserError {
public:
    MultiDefinitionObjectError(const std::string& name, const std::string& error_handler_output);
};


}

#endif