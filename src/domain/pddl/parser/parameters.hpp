#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parameter.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

class ParameterListVisitor : boost::static_visitor<pddl::ParameterList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    ParameterListVisitor(const error_handler_type& error_handler_, Context& context_);

    pddl::ParameterList operator()(const std::vector<ast::Variable>& variable_nodes);

    pddl::ParameterList operator()(const ast::TypedListOfVariablesRecursively& typed_variables_node);

    pddl::ParameterList operator()(const ast::TypedListOfVariables& node);
};


}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_PARAMETERS_HPP_
