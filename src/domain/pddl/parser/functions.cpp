#include "functions.hpp"

#include "common.hpp"
#include "parameters.hpp"


namespace loki {

FunctionSkeletonListVisitor::FunctionSkeletonListVisitor(const error_handler_type& error_handler_, domain::Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const std::vector<domain::ast::AtomicFunctionSkeleton>& formula_skeleton_nodes) {
    pddl::FunctionSkeletonList function_skeleton_list;
    auto function_type = context.base_type_number;
    for (const auto& atomic_function_skeleton : formula_skeleton_nodes) {
        auto function_name = parse(atomic_function_skeleton.function_symbol.name);
        auto function_parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context), atomic_function_skeleton.arguments);
        auto function_skeleton = context.cache.get_or_create<pddl::FunctionSkeletonImpl>(function_name, function_parameters, function_type).object;
        context.function_skeletons_by_name.emplace(function_name, function_skeleton);
        function_skeleton_list.push_back(function_skeleton);
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively& function_skeleton_list_recursively_node) {
    pddl::FunctionSkeletonList function_skeleton_list;
    // We do not parse function_skeleton_list_recursively_node.function_type because we do not support :object-fluents
    auto function_type = context.base_type_number;
    for (const auto& atomic_function_skeleton : function_skeleton_list_recursively_node.atomic_function_skeletons) {
        auto function_name = parse(atomic_function_skeleton.function_symbol.name);
        auto function_parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context), atomic_function_skeleton.arguments);
        auto function_skeleton = context.cache.get_or_create<pddl::FunctionSkeletonImpl>(function_name, function_parameters, function_type).object;
        context.function_skeletons_by_name.emplace(function_name, function_skeleton);
        function_skeleton_list.push_back(function_skeleton);
    }
    if (function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.has_value()) {
        auto add_function_skeleton_list = this->operator()(function_skeleton_list_recursively_node.function_typed_list_of_atomic_function_skeletons.value());
    }
    return function_skeleton_list;
}

pddl::FunctionSkeletonList FunctionSkeletonListVisitor::operator()(const domain::ast::FunctionTypedListOfAtomicFunctionSkeletons& function_skeleton_list_node) {
    return this->operator()(function_skeleton_list_node);
}

pddl::FunctionSkeletonList parse(const domain::ast::Functions& functions_node, const error_handler_type& error_handler, domain::Context& context) {
    return boost::apply_visitor(FunctionSkeletonListVisitor(error_handler, context), functions_node.function_types_list_of_atomic_function_skeletons);
}

}