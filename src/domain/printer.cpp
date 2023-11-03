#include "include/loki/domain/printer.hpp"

#include <sstream>
#include <vector>

using namespace std;


namespace loki::domain {

// Printer for std::vector
template<typename T>
inline std::string parse_text(const std::vector<T>& nodes);

// Printer for boost::variant
class NodeVisitorPrinter : public boost::static_visitor<std::string> {
public:
    template<typename Node>
    std::string operator()(const Node& node) const {
        return parse_text(node);
    }
};

string parse_text(const ast::Name& node) {
    stringstream ss;
    ss << node.alpha << node.suffix;
    return ss.str();
}

string parse_text(const ast::Variable& node) {
    stringstream ss;
    ss << node.question_mark << parse_text(node.name);
    return ss.str();
}

string parse_text(const ast::Number& node) {
    stringstream ss;
    ss << node.value;
    return ss.str();
}

string parse_text(const ast::Term& node) {
    return boost::apply_visitor(NodeVisitorPrinter(), node);
}


string parse_text(const ast::Type& node) {
    return boost::apply_visitor(NodeVisitorPrinter(), node);
}

string parse_text(const ast::TypeEither& node) {
    stringstream ss;
    ss << "(either ";
    for (size_t i = 0; i < node.types.size(); ++i) {
        if (i != 0) ss << " ";
        ss << parse_text(node.types[i]);
    }
    ss << ")";
    return ss.str();
}

string parse_text(const ast::TypedListOfNamesRecursively& node) {
    stringstream ss;
    ss << " "; // add leading whitespace so separate from its parent
    for (size_t i = 0; i < node.names.size(); ++i) {
        if (i != 0) ss << " ";
        ss << parse_text(node.names[i]);
    }
    ss << " - " << parse_text(node.type);
    ss << parse_text(node.typed_list_of_names);
    return ss.str();
}

string parse_text(const ast::TypedListOfNames& node) {
    return boost::apply_visitor(NodeVisitorPrinter(), node);
}

template<typename T>
inline std::string parse_text(const std::vector<T>& nodes) {
    std::stringstream ss;
    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i != 0) ss << " ";
        ss << parse_text(nodes[i]);
    }
    return ss.str();
}

}