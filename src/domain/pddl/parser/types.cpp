#include "types.hpp"

#include "common.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

/* TypeDeclarationVisitor */

TypeDeclarationVisitor::TypeDeclarationVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::TypeList TypeDeclarationVisitor::operator()(const ast::Type& type_node) {
    return boost::apply_visitor(*this, type_node);
}

pddl::TypeList TypeDeclarationVisitor::operator()(const ast::Name& name_node) {
    auto name = parse(name_node, error_handler, context);
    return { context.types->get_or_create(name).object };
}

pddl::TypeList TypeDeclarationVisitor::operator()(const ast::TypeObject&) {
    return { context.types->get_or_create("object").object };
}

pddl::TypeList TypeDeclarationVisitor::operator()(const ast::TypeEither& either_type_node) {
    // we flatten nested either types
    pddl::TypeList type_list;
    for (auto& type_node : either_type_node.types) {
        auto types = this->operator()(type_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeReferenceVisitor */

TypeReferenceVisitor::TypeReferenceVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_),
        context(context_) { }

pddl::TypeList TypeReferenceVisitor::operator()(const ast::Type& type_node) {
    return boost::apply_visitor(*this, type_node);
}

pddl::TypeList TypeReferenceVisitor::operator()(const ast::Name& name_node) {
    auto name = parse(name_node, error_handler, context);
    auto insert_result = context.types->get_or_create(name);
    if (insert_result.created) {
        error_handler(name_node, "Used undefined type.");
        throw std::runtime_error("Failed parse.");
    }
    return { insert_result.object };
}

pddl::TypeList TypeReferenceVisitor::operator()(const ast::TypeObject&) {
    assert(!context.types->get_or_create("object").created);
    return { context.types->get_or_create("object").object };
}

pddl::TypeList TypeReferenceVisitor::operator()(const ast::TypeEither& either_type_node) {
    // we flatten nested either types
    pddl::TypeList type_list;
    for (auto& type_node : either_type_node.types) {
        auto types = this->operator()(type_node);
        type_list.insert(type_list.end(), types.begin(), types.end());
    }
    return type_list;
}

/* TypeListVisitor */

TypeListVisitor::TypeListVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_),
        context(context_) { }

pddl::TypeList TypeListVisitor::operator()(const std::vector<ast::Name>& name_nodes) {
    // A visited vector of name has single base type "object"
    pddl::TypeList type_list;
    assert(!context.types->get_or_create("object").created);
    const auto base_type = context.types->get_or_create("object").object;
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node, error_handler, context);
        const auto type = context.types->get_or_create(name, pddl::TypeList{base_type}).object;
        type_list.push_back(type);
    }
    return type_list;
}

pddl::TypeList TypeListVisitor::operator()(const ast::TypedListOfNamesRecursively& typed_list_of_names_recursively_node) {
    pddl::TypeList type_list;
    const auto types = boost::apply_visitor(TypeDeclarationVisitor(error_handler, context),
                                            typed_list_of_names_recursively_node.type);
    // A non-visited vector of names has user defined base types.
    for (const auto& name_node : typed_list_of_names_recursively_node.names) {
        const auto name = parse(name_node, error_handler, context);
        if (name == "object") {
            error_handler(name_node, "Unexpected type name \"object\". It is a reserved type name.");
            throw std::runtime_error("Failed parse.");
        }
        // We also reserve type name number although PDDL specification allows it.
        // However, this allows using regular types as function types for simplicity.
        if (name == "number") {
            error_handler(name_node, "Unexpected type name \"number\". It is a reserved type name.");
            throw std::runtime_error("Failed parse.");
        }
        const auto type = context.types->get_or_create(name, types).object;
        type_list.push_back(type);
    }
    // Recursively add types.
    auto additional_types = this->operator()(typed_list_of_names_recursively_node.typed_list_of_names);
    type_list.insert(type_list.end(), additional_types.begin(), additional_types.end());
    return type_list;
}

pddl::TypeList TypeListVisitor::operator()(const ast::TypedListOfNames& node) {
    return boost::apply_visitor(*this, node);
}

/* Other functions */

pddl::TypeList parse(const ast::Types& types_node, const error_handler_type& error_handler, Context& context) {
    return boost::apply_visitor(TypeListVisitor(error_handler, context), types_node.typed_list_of_names);
}

}
