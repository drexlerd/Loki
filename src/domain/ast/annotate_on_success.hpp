#ifndef LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_
#define LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/ast/ast.hpp"
#include "../../../include/loki/domain/pddl/parser.hpp"
#include "../../../include/loki/domain/pddl/predicate.hpp"
#include "../../../include/loki/domain/pddl/type.hpp"

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

    /* Requirements */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Requirements& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements = parse(ast);

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

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

        std::unordered_map<std::string, pddl::Type> types;
        for (const auto& type : parse(ast)) {
            types.emplace(type->name, type);
        }
        pddl_context.domain_context.types = types;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    /* Predicates */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Predicates& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();

        std::unordered_map<std::string, pddl::Predicate> predicates;
        for (const auto& predicate : parse(ast)) {
            predicates.emplace(predicate->name, predicate);
        }
        pddl_context.domain_context.predicates = predicates;

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
