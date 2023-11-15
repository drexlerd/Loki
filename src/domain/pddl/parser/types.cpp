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
    return { context.types.emplace(name, pddl::create_type(name)).first->second };
}

pddl::TypeList TypeDeclarationVisitor::operator()(const ast::TypeObject&) {
    return { context.types.emplace("object", context.types.at("object")).first->second };
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
    auto it = context.types.find(name);
    if (it == context.types.end()) {
        error_handler(name_node, "Used undefined type.");
        throw std::runtime_error("Failed parse.");
    }
    return { it->second };
}

pddl::TypeList TypeReferenceVisitor::operator()(const ast::TypeObject&) {
    assert(context.types.count("object"));
    return { context.types.at("object") };
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
    const auto base_type = context.types.at("object");
    for (const auto& name_node : name_nodes) {
        const auto name = parse(name_node, error_handler, context);
        const auto type = context.types.emplace(name, pddl::create_type(name, {base_type})).first->second;
        type_list.emplace_back(type);
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
        // Dominik: "number" is not reserved as a type but is reserved as a function type.
        // We either reserve "number" within type as well, or parse a completely separate set of function types
        // from the types where "number" is reserved if :numeric-fluents.
        const auto type = context.types.emplace(name, pddl::create_type(name, types)).first->second;
        type_list.emplace_back(type);
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
