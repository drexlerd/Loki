#include "../../include/loki/common/exceptions.hpp"

namespace loki {

SyntaxParserError::SyntaxParserError(const std::string& message)
    : std::runtime_error(message) { }

SemanticParserError::SemanticParserError(const std::string& message)
    : std::runtime_error(message) { }

UndefinedError::UndefinedError(const std::string& object_name, const std::string& type_name)
    : SemanticParserError(
        ("Undefined object with name \"" + object_name + "\" and type" + type_name)) { }


NotImplementedError::NotImplementedError(const std::string& message)
    : std::runtime_error(message) { }

NotSupportedError::NotSupportedError(const std::string& message)
    : std::runtime_error(message) { }

}