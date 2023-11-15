#include "common.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

/* Name */
string parse(const domain::ast::Name& name_node, const error_handler_type&, domain::Context&) {
    stringstream ss;
    ss << name_node.alpha << name_node.suffix;
    return ss.str();
}

/* Variable */
string parse(const domain::ast::Variable& variable_node, const error_handler_type& error_handler, domain::Context& context) {
    stringstream ss;
    ss << variable_node.question_mark << parse(variable_node.name, error_handler, context);
    return ss.str();
}

/* Number */
double parse(const domain::ast::Number& number_node, const error_handler_type&, domain::Context&) {
    return number_node.value;
}

}
