#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_LITERAL_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

extern pddl::Literal parse(const domain::ast::Atom& literal_node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Literal parse(const domain::ast::NegatedAtom& literal_node, const error_handler_type& error_handler, domain::Context& context);

extern pddl::Literal parse(const domain::ast::Literal& literal_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_OBJECTS_HPP_
