#ifndef LOKI_INCLUDE_LOKI_DOMAIN_SYNTACTIC_ERROR_HANDLER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_SYNTACTIC_ERROR_HANDLER_HPP_

#include <unordered_map>
#include <iostream>

#include "include/loki/common/syntactic/error_handler.hpp"


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
