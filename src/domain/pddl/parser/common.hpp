#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

/* Name */
extern std::string parse(const domain::ast::Name& name_node, const error_handler_type& error_handler, domain::Context& context);

/* Variable */
extern pddl::Variable parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context);

/* Term */
struct TermVisitor : boost::static_visitor<pddl::Term> {
    const error_handler_type& error_handler;
    domain::Context& context;

    TermVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Term operator()(const domain::ast::Name& name_node) const;
    pddl::Term operator()(const domain::ast::Variable& variable_node) const; 
    pddl::Term operator()(const domain::ast::FunctionTerm& function_term_node) const;
};

extern pddl::Term parse(const domain::ast::Term& term_node, const error_handler_type& error_handler, domain::Context& context);

/* Number */
extern double parse(const domain::ast::Number& number_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
