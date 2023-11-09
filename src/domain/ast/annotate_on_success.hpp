#ifndef LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_
#define LOKI_SRC_DOMAIN_AST_ANNOTATE_ON_SUCCESS_HPP_

#include "../../../include/loki/common/ast/config.hpp"
#include "../../../include/loki/domain/ast/ast.hpp"

#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/directive/expect.hpp>

#include <iostream>


namespace loki::domain {


/// @brief Provide template spezializations for specific nodes.
struct domain_annotate_on_success : boost::spirit::x3::annotate_on_success
{
    /* Requirements */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementStrips& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.strips = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementTyping& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.typing = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementNegativePreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.negative_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDisjunctivePreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.disjunctive_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementEquality& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.equality = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementExistentialPreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.existential_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementUniversalPreconditions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.universal_preconditions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

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

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementConditionalEffects& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.conditional_effects = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

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

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementObjectFluents& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.object_fluents = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementNumericFluents& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.numeric_fluents = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

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

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDurativeActions& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.durative_actions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementDerivedPredicates& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.derived_predicates = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementTimedInitialLiterals& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.timed_initial_literals = true;
        pddl_context.domain_context.requirements.durative_actions = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementPreferences& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.preferences = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementConstraints& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.constraints = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::RequirementActionCosts& ast, Context const& context) {
        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        pddl_context.domain_context.requirements.action_costs = true;

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }


    /* Predicates */
    template <typename Iterator, typename Context>
    inline void on_success(
        Iterator const& first, Iterator const& last,
        loki::domain::ast::Predicates& ast, Context const& context) {

        auto& pddl_context = x3::get<pddl_context_tag>(context).get();
        for (const auto& atomic_formula_skeleton : ast.atomic_formula_skeletons) {
            const auto predicate_name = atomic_formula_skeleton.predicate.name.get_name();
            pddl_context.domain_context.predicate_names.insert(predicate_name);
        }

        boost::spirit::x3::annotate_on_success::on_success(first, last, ast, context);
    }

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
};

}

#endif
