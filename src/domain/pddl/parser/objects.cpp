#include "objects.hpp"

#include "constants.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

pddl::ObjectList parse(const ast::Constants& constants_node, const error_handler_type& error_handler, Context& context) {
    return boost::apply_visitor(ConstantListVisitor(error_handler, context), constants_node.typed_list_of_names);
}

}
