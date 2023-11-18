#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/type.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

class TypeDeclarationVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeDeclarationVisitor(const error_handler_type& error_handler_, Context& context_);

    pddl::TypeList operator()(const ast::Type& type_node);

    pddl::TypeList operator()(const ast::Name& name_node);

    pddl::TypeList operator()(const ast::TypeObject&);

    pddl::TypeList operator()(const ast::TypeEither& either_type_node);
};

class TypeReferenceVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeReferenceVisitor(const error_handler_type& error_handler_, Context& context_);

    pddl::TypeList operator()(const ast::Type& type_node);

    pddl::TypeList operator()(const ast::Name& name_node);

    pddl::TypeList operator()(const ast::TypeObject&);

    pddl::TypeList operator()(const ast::TypeEither& either_type_node);
};

class TypeListVisitor : boost::static_visitor<pddl::TypeList> {
private:
    const error_handler_type& error_handler;
    Context& context;

public:
    TypeListVisitor(const error_handler_type& error_handler_, Context& context_);

    pddl::TypeList operator()(const std::vector<ast::Name>& name_nodes);

    pddl::TypeList operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node);

    pddl::TypeList operator()(const ast::TypedListOfNames& node);
};


extern pddl::TypeList parse(const ast::Types& types_node, const error_handler_type& error_handler, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_TYPES_HPP_
