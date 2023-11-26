#include "../../include/loki/common/exceptions.hpp"


namespace loki {
SyntaxParserError::SyntaxParserError(const std::string& message)
    : std::runtime_error(message) { }

SemanticParserError::SemanticParserError(const std::string& message, const std::string& error_handler_output)
    : std::runtime_error(message + "\n" + error_handler_output) { }

NotImplementedError::NotImplementedError(const std::string& message)
    : std::runtime_error(message) { }

}
