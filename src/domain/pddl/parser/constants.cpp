#include "constants.hpp"

#include "common.hpp"
#include "types.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

ConstantListVisitor::ConstantListVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }


pddl::ObjectList ConstantListVisitor::operator()(const std::vector<ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::ObjectList object_list;
    assert(!context.types->get_or_create("object").created);
    const auto type = context.types->get_or_create("object").object;
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node, error_handler, context);
        const auto object = context.constants->get_or_create(name, pddl::TypeList{type}).object;
        object_list.emplace_back(object);
    }
    return object_list;
}

pddl::ObjectList ConstantListVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::ObjectList object_list;
    const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node, error_handler, context);
        const auto object = context.constants->get_or_create(name, types).object;
        object_list.emplace_back(object);
    }
    // Recursively add objects.
    auto additional_objects = this->operator()(typed_list_of_names_recursively_node.typed_list_of_names);
    object_list.insert(object_list.end(), additional_objects.begin(), additional_objects.end());
    return object_list;
}

pddl::ObjectList ConstantListVisitor::operator()(const ast::TypedListOfNames& node) {
    return boost::apply_visitor(*this, node);
}

}
