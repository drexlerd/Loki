#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_ERROR_HANDLER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_ERROR_HANDLER_HPP_

#include "../../common/ast/error_handler.hpp"

#include <unordered_map>
#include <iostream>


namespace loki::domain
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_domain : error_handler_base {
        error_handler_domain() : error_handler_base() {
            id_map["name"] = "Name";
        }
    };
}

#endif
