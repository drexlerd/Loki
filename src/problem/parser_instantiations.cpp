#include "parser_def.hpp"

#include "include/loki/common/syntactic/config.hpp"


namespace loki::problem::parser
{
    using iterator_type = loki::iterator_type;
    using phrase_context_type = loki::phrase_context_type;
    using context_type = loki::context_type;

    BOOST_SPIRIT_INSTANTIATE(basic_function_term_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(atomic_formula_of_names_predicate_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(atomic_formula_of_names_equality_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(atomic_formula_of_names_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(atom_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(negated_atom_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(literal_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(initial_element_literals_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(initial_element_timed_literals_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(initial_element_numeric_fluents_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(initial_element_object_fluents_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(initial_element_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_number_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_binary_operator_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_multi_operator_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_minus_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_basic_function_term_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_total_time_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_function_expression_preferences_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(optimization_minimize_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(optimization_maximize_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(optimization_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(preference_constraint_goal_descriptor_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(preference_constraint_goal_descriptor_and_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(preference_constraint_goal_descriptor_forall_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(preference_constraint_goal_descriptor_preference_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(preference_constraint_goal_descriptor_simple_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(problem_name_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(domain_name_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(objects_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(initial_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(goal_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(constraints_type, iterator_type, context_type)
    BOOST_SPIRIT_INSTANTIATE(metric_specification_type, iterator_type, context_type)

    BOOST_SPIRIT_INSTANTIATE(problem_type, iterator_type, context_type)
}
