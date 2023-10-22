#ifndef LOKI_INCLUDE_LOKI_DOMAIN_ERROR_HANDLER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_ERROR_HANDLER_HPP_

#include <unordered_map>
#include <iostream>

#include "include/loki/common/error_handler.hpp"


namespace loki::domain
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_domain : common::error_handler_base {
        error_handler_domain() : common::error_handler_base() {
            id_map[""] = "";
        }
    };
}

#endif
