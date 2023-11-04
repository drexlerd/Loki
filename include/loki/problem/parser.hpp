#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PARSER_HPP_

#include <boost/spirit/home/x3.hpp>

#include "ast.hpp"


namespace loki::problem
{
    namespace x3 = boost::spirit::x3;

    ///////////////////////////////////////////////////////////////////////////
    // parser public interface
    ///////////////////////////////////////////////////////////////////////////
    namespace parser {
        struct BasicFunctionTermArityGreaterZeroClass;
        struct BasicFunctionTermArityZeroClass;
        struct BasicFunctionTermClass;

        struct AtomicFormulaOfNamesPredicateClass;
        struct AtomicFormulaOfNamesEqualityClass;
        struct AtomicFormulaOfNamesClass;
        struct AtomClass;
        struct NegatedAtomClass;
        struct LiteralClass;

        struct InitialElaborationLiteralClass;
        struct InitialElaborationTimedLiteralsClass;
        struct InitialElaborationNumericFluentsClass;
        struct InitialElaborationObjectFluentsClass;
        struct InitialElaborationClass;

        struct MetricFunctionExpressionClass;
        struct MetricFunctionExpressionBinaryOperatorClass;
        struct MetricFunctionExpressionMultiOperatorClass;
        struct MetricFunctionExpressionMinusClass;
        struct MetricFunctionExpressionNumberClass;
        struct MetricFunctionExpressionBasicFunctionTermClass;
        struct MetricFunctionExpressionTotalTimeClass;
        struct MetricFunctionExpressionPreferencesClass;

        struct OptimizationMinimizeClass;
        struct OptimizationMaximizeClass;
        struct OptimizationClass;

        struct PreferenceConstraintGoalDescriptorClass;
        struct PreferenceConstraintGoalDescriptorAndClass;
        struct PreferenceConstraintGoalDescriptorForallClass;
        struct PreferenceConstraintGoalDescriptorPreferenceClass;
        struct PreferenceConstraintGoalDescriptorSimpleClass;

        struct ProblemNameClass;
        struct DomainNameClass;
        struct ObjectsClass;
        struct InitialClass;
        struct GoalClass;
        struct ConstraintsClass;
        struct MetricSpecificationClass;

        struct ProblemClass;


        typedef x3::rule<BasicFunctionTermArityGreaterZeroClass, ast::BasicFunctionTermArityGreaterZero> basic_function_term_arity_greater_zero_type;
        typedef x3::rule<BasicFunctionTermArityZeroClass, ast::BasicFunctionTermArityZero> basic_function_term_arity_zero_type;
        typedef x3::rule<BasicFunctionTermClass, ast::BasicFunctionTerm> basic_function_term_type;

        typedef x3::rule<AtomicFormulaOfNamesPredicateClass, ast::AtomicFormulaOfNamesPredicate> atomic_formula_of_names_predicate_type;
        typedef x3::rule<AtomicFormulaOfNamesEqualityClass, ast::AtomicFormulaOfNamesEquality> atomic_formula_of_names_equality_type;
        typedef x3::rule<AtomicFormulaOfNamesClass, ast::AtomicFormulaOfNames> atomic_formula_of_names_type;
        typedef x3::rule<AtomClass, ast::Atom> atom_type;
        typedef x3::rule<NegatedAtomClass, ast::NegatedAtom> negated_atom_type;
        typedef x3::rule<LiteralClass, ast::Literal> literal_type;

        typedef x3::rule<InitialElaborationLiteralClass, ast::InitialElaborationLiteral> initial_elaboration_literals_type;
        typedef x3::rule<InitialElaborationTimedLiteralsClass, ast::InitialElaborationTimedLiterals> initial_elaboration_timed_literals_type;
        typedef x3::rule<InitialElaborationNumericFluentsClass, ast::InitialElaborationNumericFluents> initial_elaboration_numeric_fluents_type;
        typedef x3::rule<InitialElaborationObjectFluentsClass, ast::InitialElaborationObjectFluents> initial_elaboration_object_fluents_type;
        typedef x3::rule<InitialElaborationClass, ast::InitialElaboration> initial_elaboration_type;

        typedef x3::rule<MetricFunctionExpressionClass, ast::MetricFunctionExpression> metric_function_expression_type;
        typedef x3::rule<MetricFunctionExpressionBinaryOperatorClass, ast::MetricFunctionExpressionBinaryOperator> metric_function_expression_binary_operator_type;
        typedef x3::rule<MetricFunctionExpressionMultiOperatorClass, ast::MetricFunctionExpressionMultiOperator> metric_function_expression_multi_operator_type;
        typedef x3::rule<MetricFunctionExpressionMinusClass, ast::MetricFunctionExpressionMinus> metric_function_expression_minus_type;
        typedef x3::rule<MetricFunctionExpressionNumberClass, ast::MetricFunctionExpressionNumber> metric_function_expression_number_type;
        typedef x3::rule<MetricFunctionExpressionBasicFunctionTermClass, ast::MetricFunctionExpressionBasicFunctionTerm> metric_function_expression_basic_function_term_type;
        typedef x3::rule<MetricFunctionExpressionTotalTimeClass, ast::MetricFunctionExpressionTotalTime> metric_function_expression_total_time_type;
        typedef x3::rule<MetricFunctionExpressionPreferencesClass, ast::MetricFunctionExpressionPreferences> metric_function_expression_preferences_type;

        typedef x3::rule<OptimizationMinimizeClass, ast::OptimizationMinimize> optimization_minimize_type;
        typedef x3::rule<OptimizationMaximizeClass, ast::OptimizationMaximize> optimization_maximize_type;
        typedef x3::rule<OptimizationClass, ast::Optimization> optimization_type;

        typedef x3::rule<PreferenceConstraintGoalDescriptorClass, ast::PreferenceConstraintGoalDescriptor> preference_constraint_goal_descriptor_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorAndClass, ast::PreferenceConstraintGoalDescriptorAnd> preference_constraint_goal_descriptor_and_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorForallClass, ast::PreferenceConstraintGoalDescriptorForall> preference_constraint_goal_descriptor_forall_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorPreferenceClass, ast::PreferenceConstraintGoalDescriptorPreference> preference_constraint_goal_descriptor_preference_type;
        typedef x3::rule<PreferenceConstraintGoalDescriptorSimpleClass, ast::PreferenceConstraintGoalDescriptorSimple> preference_constraint_goal_descriptor_simple_type;

        typedef x3::rule<ProblemNameClass, ast::ProblemName> problem_name_type;
        typedef x3::rule<DomainNameClass, ast::DomainName> domain_name_type;
        typedef x3::rule<ObjectsClass, ast::Objects> objects_type;
        typedef x3::rule<InitialClass, ast::Initial> initial_type;
        typedef x3::rule<GoalClass, ast::Goal> goal_type;
        typedef x3::rule<ConstraintsClass, ast::Constraints> constraints_type;
        typedef x3::rule<MetricSpecificationClass, ast::MetricSpecification> metric_specification_type;

        typedef x3::rule<ProblemClass, ast::Problem> problem_type;


        BOOST_SPIRIT_DECLARE(basic_function_term_arity_greater_zero_type,
            basic_function_term_arity_zero_type, basic_function_term_type)

        BOOST_SPIRIT_DECLARE(atomic_formula_of_names_predicate_type, atomic_formula_of_names_equality_type,
            atomic_formula_of_names_type, atom_type, negated_atom_type, literal_type)

        BOOST_SPIRIT_DECLARE(initial_elaboration_literals_type, initial_elaboration_timed_literals_type,
            initial_elaboration_numeric_fluents_type, initial_elaboration_object_fluents_type,
            initial_elaboration_type)

        BOOST_SPIRIT_DECLARE(metric_function_expression_type,
            metric_function_expression_binary_operator_type,
            metric_function_expression_multi_operator_type,
            metric_function_expression_minus_type, metric_function_expression_number_type,
            metric_function_expression_basic_function_term_type,
            metric_function_expression_total_time_type, metric_function_expression_preferences_type)

        BOOST_SPIRIT_DECLARE(optimization_minimize_type, optimization_maximize_type, optimization_type)

        BOOST_SPIRIT_DECLARE(preference_constraint_goal_descriptor_type,
            preference_constraint_goal_descriptor_and_type,
            preference_constraint_goal_descriptor_forall_type,
            preference_constraint_goal_descriptor_preference_type,
            preference_constraint_goal_descriptor_simple_type)

        BOOST_SPIRIT_DECLARE(problem_name_type, domain_name_type, objects_type, initial_type, goal_type,
            constraints_type, metric_specification_type)

        BOOST_SPIRIT_DECLARE(problem_type)
    }



    parser::problem_type const& problem_type();
}

#endif