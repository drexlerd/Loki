#ifndef LOKI_INCLUDE_LOKI_PROBLEM_ERROR_HANDLER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_ERROR_HANDLER_HPP_

#include <unordered_map>
#include <iostream>

#include "include/loki/domain/error_handler.hpp"


namespace loki::problem
{
    namespace x3 = boost::spirit::x3;

    struct error_handler_problem : domain::error_handler_domain {
        error_handler_problem() : domain::error_handler_domain() {
            id_map["name"] = "Name";
        }
    };
}

#endif
