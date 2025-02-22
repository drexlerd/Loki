/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_CACHED_BASE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_CACHED_BASE_HPP_

#include "interface.hpp"
#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/domain_builder.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/problem_builder.hpp"

namespace loki
{
template<typename Derived>
class RecursiveCachedBaseTranslator : public ITranslator<RecursiveCachedBaseTranslator<Derived>>
{
private:
    RecursiveCachedBaseTranslator() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

    using TranslatorCache = boost::hana::map<
        boost::hana::pair<boost::hana::type<loki::Requirements>, std::unordered_map<loki::Requirements, loki::Requirements>>,  //
        boost::hana::pair<boost::hana::type<loki::Type>, std::unordered_map<loki::Type, loki::Type>>,
        boost::hana::pair<boost::hana::type<loki::Object>, std::unordered_map<loki::Object, loki::Object>>,
        boost::hana::pair<boost::hana::type<loki::Variable>, std::unordered_map<loki::Variable, loki::Variable>>,
        boost::hana::pair<boost::hana::type<loki::Term>, std::unordered_map<loki::Term, loki::Term>>,
        boost::hana::pair<boost::hana::type<loki::Parameter>, std::unordered_map<loki::Parameter, loki::Parameter>>,
        boost::hana::pair<boost::hana::type<loki::Predicate>, std::unordered_map<loki::Predicate, loki::Predicate>>,
        boost::hana::pair<boost::hana::type<loki::Atom>, std::unordered_map<loki::Atom, loki::Atom>>,
        boost::hana::pair<boost::hana::type<loki::Literal>, std::unordered_map<loki::Literal, loki::Literal>>,
        boost::hana::pair<boost::hana::type<loki::FunctionValue>, std::unordered_map<loki::FunctionValue, loki::FunctionValue>>,
        boost::hana::pair<boost::hana::type<loki::ConditionLiteral>, std::unordered_map<loki::ConditionLiteral, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionAnd>, std::unordered_map<loki::ConditionAnd, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionOr>, std::unordered_map<loki::ConditionOr, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionNot>, std::unordered_map<loki::ConditionNot, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionImply>, std::unordered_map<loki::ConditionImply, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionExists>, std::unordered_map<loki::ConditionExists, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionForall>, std::unordered_map<loki::ConditionForall, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::ConditionNumericConstraint>, std::unordered_map<loki::ConditionNumericConstraint, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::Condition>, std::unordered_map<loki::Condition, loki::Condition>>,
        boost::hana::pair<boost::hana::type<loki::EffectLiteral>, std::unordered_map<loki::EffectLiteral, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectAnd>, std::unordered_map<loki::EffectAnd, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectNumeric>, std::unordered_map<loki::EffectNumeric, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeForall>, std::unordered_map<loki::EffectCompositeForall, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeWhen>, std::unordered_map<loki::EffectCompositeWhen, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeOneof>, std::unordered_map<loki::EffectCompositeOneof, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::EffectCompositeProbabilistic>, std::unordered_map<loki::EffectCompositeProbabilistic, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::Effect>, std::unordered_map<loki::Effect, loki::Effect>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionNumber>, std::unordered_map<loki::FunctionExpressionNumber, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionBinaryOperator>,
                          std::unordered_map<loki::FunctionExpressionBinaryOperator, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionMultiOperator>,
                          std::unordered_map<loki::FunctionExpressionMultiOperator, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionMinus>, std::unordered_map<loki::FunctionExpressionMinus, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpressionFunction>, std::unordered_map<loki::FunctionExpressionFunction, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionExpression>, std::unordered_map<loki::FunctionExpression, loki::FunctionExpression>>,
        boost::hana::pair<boost::hana::type<loki::FunctionSkeleton>, std::unordered_map<loki::FunctionSkeleton, loki::FunctionSkeleton>>,
        boost::hana::pair<boost::hana::type<loki::Function>, std::unordered_map<loki::Function, loki::Function>>,
        boost::hana::pair<boost::hana::type<loki::Action>, std::unordered_map<loki::Action, loki::Action>>,
        boost::hana::pair<boost::hana::type<loki::Axiom>, std::unordered_map<loki::Axiom, loki::Axiom>>,
        boost::hana::pair<boost::hana::type<loki::OptimizationMetric>, std::unordered_map<loki::OptimizationMetric, loki::OptimizationMetric>>>;

    TranslatorCache m_cache;

protected:
    friend class ITranslator<RecursiveCachedBaseTranslator<Derived>>;

    ///////////////////////////////////////////////////////
    /// Prepare
    ///////////////////////////////////////////////////////

    template<std::ranges::forward_range Range>
    void prepare_level_1(const Range& range)
    {
        self().prepare_level_2(range);
    }
    template<std::ranges::forward_range Range>
    void prepare_level_2(const Range& range)
    {
        std::ranges::for_each(range, [&](auto&& arg) { this->prepare_level_0(arg); });
    }
    template<typename T>
    void prepare_level_1(const T& element)
    {
        self().prepare_level_2(element);
    }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    /// @brief Retrieve or create cache entry of translation to avoid recomputations.
    template<typename Impl, typename Impl2, typename TranslateFunc>
    auto cached_translate_level_2(Impl impl, std::unordered_map<Impl, Impl2>& cache, const TranslateFunc& translate_func)
    {
        // Access from cache
        auto it = cache.find(impl);
        if (it != cache.end())
        {
            return it->second;
        }

        // Translate
        auto translated = translate_func(impl);

        // Insert into cache
        cache.emplace(impl, translated);

        return translated;
    }
    /// @brief Translate a container of elements into a container of elements.
    template<IsBackInsertibleRange Range, typename Builder>
    auto translate_level_1(const Range& input, Builder& builder)
    {
        return self().translate_level_2(input, builder);
    }
    /// @brief Translate a container of elements into a container of elements.
    template<IsBackInsertibleRange Range, typename Builder>
    auto translate_level_2(const Range& input, Builder& builder)
    {
        std::remove_cvref_t<Range> output;

        if constexpr (requires { output.reserve(std::ranges::size(input)); })
        {
            output.reserve(std::ranges::size(input));
        }

        std::ranges::transform(input, std::back_inserter(output), [&](auto&& arg) { return this->translate_level_0(arg, builder); });

        return output;
    }
    template<typename T, typename Builder>
    auto translate_level_1(const T& element, Builder& builder)
    {
        return cached_translate_level_2(element,
                                        boost::hana::at_key(m_cache, boost::hana::type<T> {}),
                                        [&](auto&& arg) { return self().translate_level_2(arg, builder); });
    }

    template<typename Builder>
    loki::Requirements translate_level_2(loki::Requirements requirements, Builder& builder)
    {
        return builder.get_repositories().get_or_create_requirements(requirements->get_requirements());
    }
    template<typename Builder>
    loki::Type translate_level_2(loki::Type type, Builder& builder)
    {
        return builder.get_repositories().get_or_create_type(type->get_name(), this->translate_level_0(type->get_bases(), builder));
    }
    template<>
    loki::Type translate_level_2(loki::Type type, ProblemBuilder& builder)
    {
        return builder.get_repositories().get_or_create_type(type->get_name(), this->translate_level_0(type->get_bases(), builder));
    }
    template<typename Builder>
    loki::Object translate_level_2(loki::Object object, Builder& builder)
    {
        return builder.get_repositories().get_or_create_object(object->get_name(), this->translate_level_0(object->get_bases(), builder));
    }
    template<typename Builder>
    loki::Variable translate_level_2(loki::Variable variable, Builder& builder)
    {
        return builder.get_repositories().get_or_create_variable(variable->get_name());
    }
    template<typename Builder>
    loki::Term translate_level_2(loki::Term term, Builder& builder)
    {
        return std::visit([&](auto&& arg) -> loki::Term { return builder.get_repositories().get_or_create_term(this->translate_level_0(arg, builder)); },
                          term->get_object_or_variable());
    }
    template<typename Builder>
    loki::Parameter translate_level_2(loki::Parameter parameter, Builder& builder)
    {
        return builder.get_repositories().get_or_create_parameter(this->translate_level_0(parameter->get_variable(), builder),
                                                                  this->translate_level_0(parameter->get_bases(), builder));
    }
    template<typename Builder>
    loki::Predicate translate_level_2(loki::Predicate predicate, Builder& builder)
    {
        return builder.get_repositories().get_or_create_predicate(predicate->get_name(), this->translate_level_0(predicate->get_parameters(), builder));
    }
    template<typename Builder>
    loki::Atom translate_level_2(loki::Atom atom, Builder& builder)
    {
        return builder.get_repositories().get_or_create_atom(this->translate_level_0(atom->get_predicate(), builder),
                                                             this->translate_level_0(atom->get_terms(), builder));
    }
    template<typename Builder>
    loki::Literal translate_level_2(loki::Literal literal, Builder& builder)
    {
        return builder.get_repositories().get_or_create_literal(literal->is_negated(), this->translate_level_0(literal->get_atom(), builder));
    }
    template<typename Builder>
    loki::FunctionValue translate_level_2(loki::FunctionValue function_value, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_value(this->translate_level_0(function_value->get_function(), builder),
                                                                       function_value->get_number());
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionLiteral condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_literal(this->translate_level_0(condition->get_literal(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionAnd condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_and(this->translate_level_0(condition->get_conditions(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionOr condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_or(this->translate_level_0(condition->get_conditions(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionNot condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_not(this->translate_level_0(condition->get_condition(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionImply condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_imply(this->translate_level_0(condition->get_condition_left(), builder),
                                                                     this->translate_level_0(condition->get_condition_right(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionExists condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_exists(this->translate_level_0(condition->get_parameters(), builder),
                                                                      this->translate_level_0(condition->get_condition(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionForall condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(
            builder.get_repositories().get_or_create_condition_forall(this->translate_level_0(condition->get_parameters(), builder),
                                                                      this->translate_level_0(condition->get_condition(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::ConditionNumericConstraint condition, Builder& builder)
    {
        return builder.get_repositories().get_or_create_condition(builder.get_repositories().get_or_create_condition_numeric_constraint(
            condition->get_binary_comparator(),
            this->translate_level_0(condition->get_function_expression_left(), builder),
            this->translate_level_0(condition->get_function_expression_right(), builder)));
    }
    template<typename Builder>
    loki::Condition translate_level_2(loki::Condition condition, Builder& builder)
    {
        return std::visit([&](auto&& arg) -> loki::Condition { return this->translate_level_0(arg, builder); }, condition->get_condition());
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectLiteral effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_literal(this->translate_level_0(effect->get_literal(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectAnd effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_and(this->translate_level_0(effect->get_effects(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectNumeric effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_numeric(effect->get_assign_operator(),
                                                                    this->translate_level_0(effect->get_function(), builder),
                                                                    this->translate_level_0(effect->get_function_expression(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectCompositeForall effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_composite_forall(this->translate_level_0(effect->get_parameters(), builder),
                                                                             this->translate_level_0(effect->get_effect(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectCompositeWhen effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_composite_when(this->translate_level_0(effect->get_condition(), builder),
                                                                           this->translate_level_0(effect->get_effect(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectCompositeOneof effect, Builder& builder)
    {
        return builder.get_repositories().get_or_create_effect(
            builder.get_repositories().get_or_create_effect_composite_oneof(this->translate_level_0(effect->get_effects(), builder)));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::EffectCompositeProbabilistic effect, Builder& builder)
    {
        auto distribution = loki::EffectDistribution();
        for (const auto& [probability, effect] : effect->get_effect_distribution())
        {
            distribution.emplace_back(probability, this->translate_level_0(effect, builder));
        }
        return builder.get_repositories().get_or_create_effect(builder.get_repositories().get_or_create_effect_composite_probabilistic(distribution));
    }
    template<typename Builder>
    loki::Effect translate_level_2(loki::Effect effect, Builder& builder)
    {
        return std::visit([&](auto&& arg) -> loki::Effect { return this->translate_level_0(arg, builder); }, effect->get_effect());
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionNumber function_expression, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_expression(
            builder.get_repositories().get_or_create_function_expression_number(function_expression->get_number()));
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionBinaryOperator function_expression, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_expression(builder.get_repositories().get_or_create_function_expression_binary_operator(
            function_expression->get_binary_operator(),
            this->translate_level_0(function_expression->get_left_function_expression(), builder),
            this->translate_level_0(function_expression->get_right_function_expression(), builder)));
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionMultiOperator function_expression, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_expression(builder.get_repositories().get_or_create_function_expression_multi_operator(
            function_expression->get_multi_operator(),
            this->translate_level_0(function_expression->get_function_expressions(), builder)));
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionMinus function_expression, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_expression(builder.get_repositories().get_or_create_function_expression_minus(
            this->translate_level_0(function_expression->get_function_expression(), builder)));
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionFunction function_expression, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_expression(
            builder.get_repositories().get_or_create_function_expression_function(this->translate_level_0(function_expression->get_function(), builder)));
    }
    template<typename Builder>
    loki::FunctionExpression translate_level_2(loki::FunctionExpression function_expression, Builder& builder)
    {
        return std::visit([&](auto&& arg) -> loki::FunctionExpression { return this->translate_level_0(arg, builder); },
                          function_expression->get_function_expression());
    }
    template<typename Builder>
    loki::FunctionSkeleton translate_level_2(loki::FunctionSkeleton function_skeleton, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function_skeleton(function_skeleton->get_name(),
                                                                          this->translate_level_0(function_skeleton->get_parameters(), builder),
                                                                          this->translate_level_0(function_skeleton->get_type(), builder));
    }
    template<typename Builder>
    loki::Function translate_level_2(loki::Function function, Builder& builder)
    {
        return builder.get_repositories().get_or_create_function(this->translate_level_0(function->get_function_skeleton(), builder),
                                                                 this->translate_level_0(function->get_terms(), builder));
    }
    template<typename Builder>
    loki::Action translate_level_2(loki::Action action, Builder& builder)
    {
        return builder.get_repositories().get_or_create_action(
            action->get_name(),
            action->get_original_arity(),
            this->translate_level_0(action->get_parameters(), builder),
            (action->get_condition().has_value() ? std::optional<loki::Condition>(this->translate_level_0(action->get_condition().value(), builder)) :
                                                   std::nullopt),
            (action->get_effect().has_value() ? std::optional<loki::Effect>(this->translate_level_0(action->get_effect().value(), builder)) : std::nullopt));
    }
    template<typename Builder>
    loki::Axiom translate_level_2(loki::Axiom axiom, Builder& builder)
    {
        return builder.get_repositories().get_or_create_axiom(this->translate_level_0(axiom->get_parameters(), builder),
                                                              this->translate_level_0(axiom->get_literal(), builder),
                                                              this->translate_level_0(axiom->get_condition(), builder));
    }
    template<typename Builder>
    loki::OptimizationMetric translate_level_2(loki::OptimizationMetric metric, Builder& builder)
    {
        return builder.get_repositories().get_or_create_optimization_metric(metric->get_optimization_metric(),
                                                                            this->translate_level_0(metric->get_function_expression(), builder));
    }

    auto translate_level_1(const Domain& domain, DomainBuilder& builder) { return self().translate_level_2(domain, builder); }

    auto translate_level_2(const Domain& domain, DomainBuilder& builder)
    {
        builder.get_name() = domain->get_name();
        builder.get_filepath() = domain->get_filepath();
        builder.get_requirements() = this->translate_level_0(domain->get_requirements(), builder);
        builder.get_types() = this->translate_level_0(domain->get_types(), builder);
        builder.get_constants() = this->translate_level_0(domain->get_constants(), builder);
        builder.get_predicates() = this->translate_level_0(domain->get_predicates(), builder);
        builder.get_function_skeletons() = this->translate_level_0(domain->get_functions(), builder);
        builder.get_actions() = this->translate_level_0(domain->get_actions(), builder);
        builder.get_axioms() = this->translate_level_0(domain->get_axioms(), builder);

        return builder.get_result();
    }

    auto translate_level_1(const Problem& problem, ProblemBuilder& builder) { return self().translate_level_2(problem, builder); }

    auto translate_level_2(const Problem& problem, ProblemBuilder& builder)
    {
        builder.get_filepath() = problem->get_filepath();
        builder.get_name() = problem->get_name();
        builder.get_requirements() = this->translate_level_0(problem->get_requirements(), builder);
        builder.get_objects() = this->translate_level_0(problem->get_objects(), builder);
        builder.get_predicates() = this->translate_level_0(problem->get_predicates(), builder);
        builder.get_initial_literals() = this->translate_level_0(problem->get_initial_literals(), builder);
        builder.get_function_values() = this->translate_level_0(problem->get_function_values(), builder);
        if (problem->get_goal_condition().has_value())
            builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition().value(), builder);
        if (problem->get_optimization_metric().has_value())
            builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric().value(), builder);
        builder.get_axioms() = this->translate_level_0(problem->get_axioms(), builder);

        return builder.get_result(problem->get_index());
    }
};
}

#endif
