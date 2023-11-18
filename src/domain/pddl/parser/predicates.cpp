#include "predicates.hpp"

#include "common.hpp"
#include "parameters.hpp"

using namespace loki::domain;
using namespace std;

namespace loki {

pddl::PredicateList parse(const ast::Predicates& predicates_node, const error_handler_type& error_handler, Context& context) {

    pddl::PredicateList predicate_list;
    for (const auto& atomic_formula_skeleton : predicates_node.atomic_formula_skeletons) {
        const auto name = parse(atomic_formula_skeleton.predicate.name, error_handler, context);
        const auto parameters = boost::apply_visitor(ParameterListVisitor(error_handler, context),
                                                     atomic_formula_skeleton.typed_list_of_variables);
        const auto predicate = context.predicates->get_or_create(name, parameters).object;
        predicate_list.emplace_back(predicate);
    }
    return predicate_list;
}

}
