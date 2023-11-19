#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

class ParameterListVisitor : boost::static_visitor<pddl::ParameterList> {
private:
    const error_handler_type& error_handler;
    domain::Context& context;

public:
    ParameterListVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::ParameterList operator()(const std::vector<domain::ast::Variable>& variable_nodes);

    pddl::ParameterList operator()(const domain::ast::TypedListOfVariablesRecursively& typed_variables_node);

    pddl::ParameterList operator()(const domain::ast::TypedListOfVariables& node);
};


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_
