#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

/* Name */
std::string parse(const domain::ast::Name& name_node, const error_handler_type& error_handler, domain::Context& context);

/* Variable */
std::string parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context);

/* Number */
double parse(const domain::ast::Number& number_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
