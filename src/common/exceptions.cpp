#include "../../include/loki/common/exceptions.hpp"

namespace loki {

SyntaxParserError::SyntaxParserError(const std::string& message)
    : std::runtime_error(message) { }


SemanticParserError::SemanticParserError(const std::string& message)
    : std::runtime_error(message) { }

UndefinedPredicateError::UndefinedPredicateError(const std::string& name)
    : SemanticParserError(
        ("Undefined predicate with name \"" + name + "\".")) { }

UndefinedConstantError::UndefinedConstantError(const std::string& name)
    : SemanticParserError(
        ("Undefined constant with name \"" + name + "\".")) { }

UndefinedRequirementError::UndefinedRequirementError(pddl::RequirementEnum requirement)
    : SemanticParserError("") { }

MismatchedArgumentError::MismatchedArgumentError(
    const std::string& parameter_name, const std::string& variable_name,
    int parameter_list_arity, int variable_list_arity)
    : SemanticParserError(
        ("Mismatched arguments to \"" 
            + parameter_name 
            + "\" and \"" 
            + variable_name 
            + "\" with sizes " 
            + std::to_string(parameter_list_arity))
            + "!="
            + std::to_string(variable_list_arity)
            + ".") { }


NotImplementedError::NotImplementedError(const std::string& message)
    : std::runtime_error(message) { }


NotSupportedError::NotSupportedError(const std::string& message)
    : std::runtime_error(message) { }

}