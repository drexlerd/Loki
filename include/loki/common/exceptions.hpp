#ifndef LOKI_INCLUDE_LOKI_COMMON_EXCEPTIONS_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_EXCEPTIONS_HPP_

#include <stdexcept>


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