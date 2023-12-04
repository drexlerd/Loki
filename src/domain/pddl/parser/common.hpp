#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

/* Name */
extern std::string parse(const domain::ast::Name& name_node);

/* Variable */
extern pddl::Variable parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context);

/* Term */
struct TermDeclarationVisitor : boost::static_visitor<pddl::Term> {
    const error_handler_type& error_handler;
    domain::Context& context;

    TermDeclarationVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Term operator()(const domain::ast::Name& name_node) const;
    pddl::Term operator()(const domain::ast::Variable& variable_node) const;
    pddl::Term operator()(const domain::ast::FunctionTerm& function_term_node) const;
};

struct TermReferenceVisitor : boost::static_visitor<pddl::Term> {
    const error_handler_type& error_handler;
    domain::Context& context;

    TermReferenceVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::Term operator()(const domain::ast::Name& name_node) const;
    pddl::Term operator()(const domain::ast::Variable& variable_node) const;
    pddl::Term operator()(const domain::ast::FunctionTerm& function_term_node) const;
};


/* Number */
extern double parse(const domain::ast::Number& number_node);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_COMMON_HPP_
