#include "include/loki/domain/printer.hpp"

#include <sstream>

using namespace std;


namespace loki::domain {

class NodeVisitor : public boost::static_visitor<std::string> {
public:
    template<typename Node>
    std::string operator()(const Node& node) const {
        return parse_text(node);
    }
};

std::string parse_text(const ast::Name& node) {
    stringstream ss;
    ss << node.alpha << node.suffix;
    return ss.str();
}

std::string parse_text(const ast::Variable& node) {
    stringstream ss;
    ss << node.question_mark << parse_text(node.name);
    return ss.str();
}

std::string parse_text(const ast::Number& node) {
    stringstream ss;
    ss << node.value;
    return ss.str();
}

std::string parse_text(const ast::Term& node) {
    return boost::apply_visitor(NodeVisitor(), node);
}

}