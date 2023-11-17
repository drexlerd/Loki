#include "parameters.hpp"

#include "common.hpp"
#include "types.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

ParameterListVisitor::ParameterListVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::ParameterList ParameterListVisitor::operator()(const std::vector<ast::Variable>& variable_nodes) {
    // A visited vector of variable has single base type "object"
    pddl::ParameterList parameter_list;
    assert(!context.types->get_or_create("object").created);
    const auto type = context.types->get_or_create("object").object;
    for (const auto& variable_node : variable_nodes) {
        const auto name = parse(variable_node, error_handler, context);
        const auto parameter = pddl::create_parameter(name, {type});
        parameter_list.emplace_back(parameter);
    }
    return parameter_list;
}

pddl::ParameterList ParameterListVisitor::operator()(const ast::TypedListOfVariablesRecursively& typed_variables_node) {
    pddl::ParameterList parameter_list;
    const auto types = boost::apply_visitor(TypeReferenceVisitor(error_handler, context),
                                            typed_variables_node.type);
    // A non-visited vector of variables has user defined types
    for (const auto& variable_node : typed_variables_node.variables) {
        const auto name = parse(variable_node, error_handler, context);
        const auto parameter = pddl::create_parameter(name, types);
        parameter_list.emplace_back(parameter);
    }
    // Recursively add parameters.
    auto additional_parameters = this->operator()(typed_variables_node.typed_list_of_variables);
    parameter_list.insert(parameter_list.end(), additional_parameters.begin(), additional_parameters.end());
    return parameter_list;
}

pddl::ParameterList ParameterListVisitor::operator()(const ast::TypedListOfVariables& node) {
    return boost::apply_visitor(*this, node);
}

}
