#include "utils.hpp"

namespace loki::benchmarks {

loki::pddl::AtomList create_atoms(
    size_t num_objects,
    size_t num_predicates,
    loki::CollectionOfPDDLFactories& factories) {
    // Create num_objects-many objects with name object_1,...,object_<num_objects>
    auto objects = loki::pddl::ObjectList();
    for (size_t i = 1; i <= num_objects; ++i) {
        objects.push_back(factories.objects.get_or_create<loki::pddl::ObjectImpl>(
            ("object_" + std::to_string(i)))
        );
    }

    // Create num_predicates-many binary predicates with name predicate_1,...,predicate_<num_predicates>
    auto parameters = loki::pddl::ParameterList{
        factories.parameters.get_or_create<loki::pddl::ParameterImpl>(
            factories.variables.get_or_create<loki::pddl::VariableImpl>("?variable_left"),
            loki::pddl::TypeList{}),
        factories.parameters.get_or_create<loki::pddl::ParameterImpl>(
            factories.variables.get_or_create<loki::pddl::VariableImpl>("?variable_right"),
            loki::pddl::TypeList{})
    };

    auto predicates = loki::pddl::PredicateList();
    for (size_t i = 1; i <= num_predicates; ++i) {
        predicates.push_back(factories.predicates.get_or_create<loki::pddl::PredicateImpl>(
            ("predicate_" + std::to_string(i)),
            parameters));
    }

    auto atoms = loki::pddl::AtomList();
    // Construct num_objects^2 * num_predicates many atoms
    for (const auto& predicate : predicates) {
        for (const auto& object_left : objects) {
            for (const auto& object_right : objects) {
                atoms.push_back(factories.problem_atoms.get_or_create<loki::pddl::AtomImpl>(
                    predicate,
                    loki::pddl::TermList{
                        factories.terms.get_or_create<loki::pddl::TermObjectImpl>(object_left),
                        factories.terms.get_or_create<loki::pddl::TermObjectImpl>(object_right)
                    }));
            }
        }
    }
    return atoms;
}

}
