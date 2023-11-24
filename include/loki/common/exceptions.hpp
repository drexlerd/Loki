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
    explicit SemanticParserError(const std::string& message);
};

class UndefinedError : public SemanticParserError {
public:
    UndefinedError(const std::string& object_name, const std::string& type_name);
};

class UndefinedRequirementError : public SemanticParserError {
public:
    UndefinedRequirementError(pddl::RequirementEnum requirement);
};

class MismatchedArgumentError : public SemanticParserError {
public:
    MismatchedArgumentError(
        const std::string& parameter_name, const std::string& variable_name,
        int parameter_list_arity, int variable_list_arity);
};



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