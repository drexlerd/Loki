#include "../../include/loki/common/exceptions.hpp"

namespace loki {

SyntaxParserError::SyntaxParserError(const std::string& message)
    : std::runtime_error(message) { }


SemanticParserError::SemanticParserError(const std::string& message)
    : std::runtime_error(message) { }

UndefinedError::UndefinedError(const std::string& object_name, const std::string& type_name)
    : SemanticParserError(
        ("Undefined object with name \"" + object_name + "\" and type" + type_name)) { }

MismatchedArgumentError::MismatchedArgumentError(
    const std::string& parameter_name, const std::string& variable_name,
    int parameter_list_arity, int variable_list_arity)
    : SemanticParserError(
        ("Mismatched arguments \"" 
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