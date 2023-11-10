#ifndef LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_
#define LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/ast/ast.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"

#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>

#include <iostream>


namespace loki::domain {


/// @brief Provide template spezializations for specific nodes.
struct domain_annotate_on_success : boost::spirit::x3::annotate_on_success
{
    // Name;
    // Variable;
    // Number;
    // Term;


    // RequirementStrips;
    // RequirementTyping;
    // RequirementNegativePreconditions;
    // RequirementDisjunctivePreconditions;
    // RequirementEquality;
    // RequirementExistentialPreconditions;
    // RequirementUniversalPreconditions;
    // RequirementQuantifiedPreconditions;
    // RequirementConditionalEffects;
    // RequirementFluents;                   // PDDL 3.1 = :object-fluents + :numeric-fluents
    // RequirementObjectFluents;             // PDDL 3.1
    // RequirementNumericFluents;            // PDDL 3.1
    // RequirementAdl;
    // RequirementDurativeActions;
    // RequirementDerivedPredicates;
    // RequirementTimedInitialLiterals;
    // RequirementPreferences;
    // RequirementConstraints;
    // RequirementActionCosts;
    // Requirement;

    /* RequirementStrips */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementStrips& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.strips = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementTyping */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementTyping& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.typing = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementNegativePreconditions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementNegativePreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.negative_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementDisjunctivePreconditions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDisjunctivePreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.disjunctive_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementEquality */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementEquality& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.equality = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementExistentialPreconditions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementExistentialPreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.existential_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementUniversalPreconditions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementUniversalPreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.universal_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementQuantifiedPreconditions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementQuantifiedPreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.quantified_preconditions = true;
        pddl_context.domain_context.requirements.existential_preconditions = true;
        pddl_context.domain_context.requirements.universal_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementConditionalEffects */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementConditionalEffects& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.conditional_effects = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementFluents */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementFluents& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.fluents = true;
        pddl_context.domain_context.requirements.object_fluents = true;
        pddl_context.domain_context.requirements.numeric_fluents = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementObjectFluents */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementObjectFluents& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.object_fluents = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementNumericFluents */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementNumericFluents& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.numeric_fluents = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementAdl */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementAdl& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.adl = true;
        pddl_context.domain_context.requirements.strips = true;
        pddl_context.domain_context.requirements.typing = true;
        pddl_context.domain_context.requirements.disjunctive_preconditions = true;
        pddl_context.domain_context.requirements.equality = true;
        pddl_context.domain_context.requirements.quantified_preconditions = true;
        pddl_context.domain_context.requirements.conditional_effects = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementDurativeActions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDurativeActions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.durative_actions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementDerivedPredicates */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDerivedPredicates& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.derived_predicates = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementTimedInitialLiterals */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementTimedInitialLiterals& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.timed_initial_literals = true;
        pddl_context.domain_context.requirements.durative_actions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementPreferences */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementPreferences& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.preferences = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementConstraints */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementConstraints& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.constraints = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* RequirementActionCosts */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementActionCosts& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.action_costs = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }


    // Type;
    // TypeObject;
    // TypeEither;
    // TypedListOfNamesRecursively;  // :typing
    // TypedListOfNames;
    // TypedListOfVariablesRecursively;  // :typing
    // TypedListOfVariables;

    /* TypedListOfNamesRecursively */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::TypedListOfNamesRecursively& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.typing) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":typing in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* TypedListOfVariablesRecursively */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::TypedListOfVariablesRecursively& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.typing) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":typing in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }


    // Predicate;
    // AtomicFormulaSkeleton;

    // FunctionSymbol;
    // FunctionTerm;
    // FunctionTypeNumber;                                       // :numeric-fluents
    // FunctionTypeType;                                         // :object-fluents :typing
    // FunctionType;
    // AtomicFunctionSkeleton;
    // FunctionTypedListOfAtomicFunctionSkeletonsRecursively;
    // FunctionTypedListOfAtomicFunctionSkeletons;

    // AtomicFormulaOfTermsPredicate;
    // AtomicFormulaOfTermsEquality;         // :equality
    // AtomicFormulaOfTerms;
    // Atom;
    // NegatedAtom;
    // Literal;

    // MultiOperatorMul;
    // MultiOperatorPlus;
    // MultiOperator;
    // BinaryOperatorMinus;
    // BinaryOperatorDiv;
    // BinaryOperator;

    // BinaryComparatorGreater;
    // BinaryComparatorLess;
    // BinaryComparatorEqual;
    // BinaryComparatorGreaterEqual;
    // BinaryComparatorLessEqual;
    // BinaryComparator;

    // FunctionHead;
    // FunctionExpression;                   // :numeric-fluents
    // FunctionExpressionNumber;
    // FunctionExpressionBinaryOp;
    // FunctionExpressionMinus;
    // FunctionExpressionHead;

    // GoalDescriptor;
    // GoalDescriptorAtom;
    // GoalDescriptorLiteral;                // :negative-preconditions
    // GoalDescriptorAnd;
    // GoalDescriptorOr;                     // :disjunctive-preconditions
    // GoalDescriptorNot;                    // :disjunctive-preconditions
    // GoalDescriptorImply;                  // :disjunctive-preconditions
    // GoalDescriptorExists;                 // :existential-preconditions
    // GoalDescriptorForall;                 // :universal-preconditions
    // GoalDescriptorFunctionComparison;     // :numeric-fluents

    // ConstraintGoalDescriptor;
    // ConstraintGoalDescriptorAnd;
    // ConstraintGoalDescriptorForall;
    // ConstraintGoalDescriptorAtEnd;
    // ConstraintGoalDescriptorAlways;
    // ConstraintGoalDescriptorSometime;
    // ConstraintGoalDescriptorWithin;
    // ConstraintGoalDescriptorAtMostOnce;
    // ConstraintGoalDescriptorSometimeAfter;
    // ConstraintGoalDescriptorSometimeBefore;
    // ConstraintGoalDescriptorAlwaysWithin;
    // ConstraintGoalDescriptorHoldDuring;
    // ConstraintGoalDescriptorHoldAfter;

    // PreferenceName;
    // PreconditionGoalDescriptor;
    // PreconditionGoalDescriptorSimple;
    // PreconditionGoalDescriptorAnd;
    // PreconditionGoalDescriptorPreference; // :preferences
    // PreconditionGoalDescriptorForall;     // :universal-preconditions

    // AssignOperatorAssign;
    // AssignOperatorScaleUp;
    // AssignOperatorScaleDown;
    // AssignOperatorIncrease;
    // AssignOperatorDecrease;
    // AssignOperator;

    // Effect;
    // EffectProductionLiteral;
    // EffectProductionNumericFluent;
    // EffectProductionObjectFluent;
    // EffectProduction;
    // EffectConditionalForall;
    // EffectConditionalWhen;
    // EffectConditional;

    /* ConditionalEffect */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::EffectConditional& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.conditional_effects) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":conditional-effects in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }


    // ActionSymbol;
    // ActionBody;


    // Action;
    // DerivedPredicate;                     // :derived-predicates

    /* DerivedPredicate */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::DerivedPredicate& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.derived_predicates) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":derived-predicates in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }


    // DomainName;
    // Requirements;
    // Types;                                // : typing
    // Constants;
    // Predicates;
    // Functions;
    // Constraints;                          // :constraints
    // Structure;
    // Domain;

    /* Types */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Types& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.typing) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":typing in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* Predicates */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Predicates& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        for (const auto& atomic_formula_skeleton : ast.atomic_formula_skeletons) {
            auto name = atomic_formula_skeleton.predicate.name.get_name();
            // TODO: set arity and check types
            int arity = 0;
            pddl_context.domain_context.predicates.emplace(name, pddl::Predicate{name, arity});
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* Functions */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Functions& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!(pddl_context.domain_context.requirements.numeric_fluents
            || pddl_context.domain_context.requirements.numeric_fluents)) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":object-fluents or :numeric-fluents in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* Constraints */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Constraints& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        if (!pddl_context.domain_context.requirements.constraints) {
            boost::throw_exception(
                x3::expectation_failure<Iterator>(
                    first, ":constraints in the requirements."));
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }
};

}

#endif
