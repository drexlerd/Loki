#ifndef LOKI_INCLUDE_LOKI_PROBLEM_AST_ERROR_HANDLER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_AST_ERROR_HANDLER_HPP_

#include "../../domain/ast/error_handler.hpp"

#include <unordered_map>
#include <iostream>


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
