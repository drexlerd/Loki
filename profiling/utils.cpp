#include "utils.hpp"

namespace loki::benchmarks
{

loki::AtomList create_atoms(size_t num_objects, size_t num_predicates, loki::Repositories& factories)
{
    // Create num_objects-many objects with name object_1,...,object_<num_objects>
    auto objects = loki::ObjectList();
    for (size_t i = 1; i <= num_objects; ++i)
    {
        objects.push_back(factories.get_or_create_object(("object_" + std::to_string(i)), TypeList()));
    }

    // Create num_predicates-many binary predicates with name predicate_1,...,predicate_<num_predicates>
    auto parameters = loki::ParameterList { factories.get_or_create_parameter(factories.get_or_create_variable("?variable_left"), loki::TypeList {}),
                                            factories.get_or_create_parameter(factories.get_or_create_variable("?variable_right"), loki::TypeList {}) };

    auto predicates = loki::PredicateList();
    for (size_t i = 1; i <= num_predicates; ++i)
    {
        predicates.push_back(factories.get_or_create_predicate(("predicate_" + std::to_string(i)), parameters));
    }

    auto atoms = loki::AtomList();
    // Construct num_objects^2 * num_predicates many atoms
    for (const auto& predicate : predicates)
    {
        for (const auto& object_left : objects)
        {
            for (const auto& object_right : objects)
            {
                atoms.push_back(
                    factories.get_or_create_atom(predicate,
                                                 loki::TermList { factories.get_or_create_term(object_left), factories.get_or_create_term(object_right) }));
            }
        }
    }
    return atoms;
}

}
