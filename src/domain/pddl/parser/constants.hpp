#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"


namespace loki {

class ConstantListVisitor : boost::static_visitor<pddl::ObjectList> {
private:
    const error_handler_type& error_handler;
    domain::Context& context;

public:
    ConstantListVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::ObjectList operator()(const std::vector<domain::ast::Name>& name_nodes);

    pddl::ObjectList operator()(const domain::ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);
};

extern pddl::ObjectList parse(const domain::ast::Constants& constants_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_
