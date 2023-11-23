#include "../../include/loki/common/exceptions.hpp"

namespace loki {

SyntaxParserError::SyntaxParserError(const std::string& message)
    : std::runtime_error(message) { }

SemanticParserError::SemanticParserError(const std::string& message)
    : std::runtime_error(message) { }

NotImplementedError::NotImplementedError(const std::string& message)
    : std::runtime_error(message) { }

}