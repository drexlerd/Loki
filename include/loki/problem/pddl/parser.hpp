#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PARSER_HPP_

#include "context.hpp"
#include "declarations.hpp"

#include "../../common/ast/config.hpp"
#include "../ast/ast.hpp"


namespace loki {

extern pddl::Problem parse(const problem::ast::Problem& problem_node, const error_handler_type& error_handler, problem::Context& context, const pddl::Domain& domain);

}

#endif
