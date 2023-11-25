#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"
#include "../../../../include/loki/domain/pddl/declarations.hpp"
#include "../../../../include/loki/domain/pddl/function_skeleton.hpp"

#include <variant>


namespace loki {

class FunctionSkeletonListVisitor : boost::static_visitor<pddl::FunctionSkeletonList> {
private:
    const error_handler_type& error_handler;
    domain::Context& context;

public:
    FunctionSkeletonListVisitor(const error_handler_type& error_handler_, domain::Context& context_);

    pddl::FunctionSkeletonList operator()(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes);

    pddl::FunctionSkeletonList operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node);

    pddl::FunctionSkeletonList operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& function_skeleton_list_node);
};

extern pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, const error_handler_type& error_handler, domain::Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_FUNCTIONS_HPP_
