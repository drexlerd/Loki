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
extern std::string parse_text(const ast::TypeObject& node);
extern std::string parse_text(const ast::TypeEither& node);
extern std::string parse_text(const ast::TypedListOfNamesRecursively& node);
extern std::string parse_text(const ast::TypedListOfNames& node);
extern std::string parse_text(const ast::TypedListOfVariablesRecursively& node);
extern std::string parse_text(const ast::TypedListOfVariables& node);

extern std::string parse_text(const ast::Predicate& node);
extern std::string parse_text(const ast::AtomicFormulaSkeleton& node);

extern std::string parse_text(const ast::FunctionSymbol& node);
extern std::string parse_text(const ast::FunctionTerm& node);
extern std::string parse_text(const ast::FunctionTypeNumber& node);
extern std::string parse_text(const ast::FunctionTypeType& node);
extern std::string parse_text(const ast::FunctionType& node);
extern std::string parse_text(const ast::AtomicFunctionSkeleton& node);

}

#endif