#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_

#include "ast.hpp"

#include <sstream>

namespace loki::domain {

// create string representations from ast nodes.
extern std::string parse_text(const ast::Name& node);
extern std::string parse_text(const ast::Variable& node);
extern std::string parse_text(const ast::Number& node);
extern std::string parse_text(const ast::Term& node);

extern std::string parse_text(const ast::Type& node);
extern std::string parse_text(const ast::TypeEither& node);
extern std::string parse_text(const ast::TypedListOfNamesRecursively& node);
extern std::string parse_text(const ast::TypedListOfNames& node);


}

#endif