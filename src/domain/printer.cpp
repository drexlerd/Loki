#include "include/loki/domain/printer.hpp"

#include <sstream>

using namespace std;


namespace loki::domain {

std::string parse_text(const ast::Name& node) {
    stringstream ss;
    ss << node.alpha << node.suffix;
    return ss.str();
}

}