#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PRINTER_HPP_

#include "ast.hpp"


namespace loki::domain {
// create string representations from ast nodes.
extern std::string parse_text(const ast::Name& node);


}

#endif