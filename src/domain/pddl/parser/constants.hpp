#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/object.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

class ConstantListVisitor : boost::static_visitor<pddl::ObjectList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    ConstantListVisitor(const error_handler_type& error_handler_, Context& context_);

    pddl::ObjectList operator()(const std::vector<ast::Name>& name_nodes);

    pddl::ObjectList operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);

    pddl::ObjectList operator()(const ast::TypedListOfNames& node);
};

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_CONSTANTS_HPP_
