#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_PARSER_HPP_

#include "context.hpp"

#include "../../common/ast/config.hpp"
#include "../ast/ast.hpp"


namespace loki {

extern pddl::Domain parse(const domain::ast::Domain& domain_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif
