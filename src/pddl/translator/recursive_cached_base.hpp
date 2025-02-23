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
    template<IsBackInsertibleRange Range>
    auto translate_level_1(const Range& input, Repositories& repositories)
    {
        return self().translate_level_2(input, repositories);
    }
    /// @brief Translate a container of elements into a container of elements.
    template<IsBackInsertibleRange Range>
    auto translate_level_2(const Range& input, Repositories& repositories)
    {
        std::remove_cvref_t<Range> output;

        if constexpr (requires { output.reserve(std::ranges::size(input)); })
        {
            output.reserve(std::ranges::size(input));
        }

        std::ranges::transform(input, std::back_inserter(output), [&](auto&& arg) { return this->translate_level_0(arg, repositories); });

        return output;
    }
    template<typename T>
    auto translate_level_1(const T& element, Repositories& repositories)
    {
        return cached_translate_level_2(element,
                                        boost::hana::at_key(m_cache, boost::hana::type<T> {}),
                                        [&](auto&& arg) { return self().translate_level_2(arg, repositories); });
    }

    loki::Requirements translate_level_2(loki::Requirements requirements, Repositories& repositories)
    {
        return repositories.get_or_create_requirements(requirements->get_requirements());
    }
    loki::Type translate_level_2(loki::Type type, Repositories& repositories)
    {
        return repositories.get_or_create_type(type->get_name(), this->translate_level_0(type->get_bases(), repositories));
    }
    loki::Object translate_level_2(loki::Object object, Repositories& repositories)
    {
        return repositories.get_or_create_object(object->get_name(), this->translate_level_0(object->get_bases(), repositories));
    }
    loki::Variable translate_level_2(loki::Variable variable, Repositories& repositories) { return repositories.get_or_create_variable(variable->get_name()); }
    loki::Term translate_level_2(loki::Term term, Repositories& repositories)
    {
        return std::visit([&](auto&& arg) -> loki::Term { return repositories.get_or_create_term(this->translate_level_0(arg, repositories)); },
                          term->get_object_or_variable());
    }
    loki::Parameter translate_level_2(loki::Parameter parameter, Repositories& repositories)
    {
        return repositories.get_or_create_parameter(this->translate_level_0(parameter->get_variable(), repositories),
                                                    this->translate_level_0(parameter->get_bases(), repositories));
    }
    loki::Predicate translate_level_2(loki::Predicate predicate, Repositories& repositories)
    {
        return repositories.get_or_create_predicate(predicate->get_name(), this->translate_level_0(predicate->get_parameters(), repositories));
    }
    loki::Atom translate_level_2(loki::Atom atom, Repositories& repositories)
    {
        return repositories.get_or_create_atom(this->translate_level_0(atom->get_predicate(), repositories),
                                               this->translate_level_0(atom->get_terms(), repositories));
    }
    loki::Literal translate_level_2(loki::Literal literal, Repositories& repositories)
    {
        return repositories.get_or_create_literal(literal->is_negated(), this->translate_level_0(literal->get_atom(), repositories));
    }
    loki::FunctionValue translate_level_2(loki::FunctionValue function_value, Repositories& repositories)
    {
        return repositories.get_or_create_function_value(this->translate_level_0(function_value->get_function(), repositories), function_value->get_number());
    }
    loki::Condition translate_level_2(loki::ConditionLiteral condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_literal(this->translate_level_0(condition->get_literal(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionAnd condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_and(this->translate_level_0(condition->get_conditions(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionOr condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_or(this->translate_level_0(condition->get_conditions(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionNot condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_not(this->translate_level_0(condition->get_condition(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionImply condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_imply(this->translate_level_0(condition->get_condition_left(), repositories),
                                                       this->translate_level_0(condition->get_condition_right(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionExists condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_exists(this->translate_level_0(condition->get_parameters(), repositories),
                                                        this->translate_level_0(condition->get_condition(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionForall condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_forall(this->translate_level_0(condition->get_parameters(), repositories),
                                                        this->translate_level_0(condition->get_condition(), repositories)));
    }
    loki::Condition translate_level_2(loki::ConditionNumericConstraint condition, Repositories& repositories)
    {
        return repositories.get_or_create_condition(
            repositories.get_or_create_condition_numeric_constraint(condition->get_binary_comparator(),
                                                                    this->translate_level_0(condition->get_function_expression_left(), repositories),
                                                                    this->translate_level_0(condition->get_function_expression_right(), repositories)));
    }
    loki::Condition translate_level_2(loki::Condition condition, Repositories& repositories)
    {
        return std::visit([&](auto&& arg) -> loki::Condition { return this->translate_level_0(arg, repositories); }, condition->get_condition());
    }
    loki::Effect translate_level_2(loki::EffectLiteral effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(repositories.get_or_create_effect_literal(this->translate_level_0(effect->get_literal(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectAnd effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(repositories.get_or_create_effect_and(this->translate_level_0(effect->get_effects(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectNumeric effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(
            repositories.get_or_create_effect_numeric(effect->get_assign_operator(),
                                                      this->translate_level_0(effect->get_function(), repositories),
                                                      this->translate_level_0(effect->get_function_expression(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectCompositeForall effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(
            repositories.get_or_create_effect_composite_forall(this->translate_level_0(effect->get_parameters(), repositories),
                                                               this->translate_level_0(effect->get_effect(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectCompositeWhen effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(
            repositories.get_or_create_effect_composite_when(this->translate_level_0(effect->get_condition(), repositories),
                                                             this->translate_level_0(effect->get_effect(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectCompositeOneof effect, Repositories& repositories)
    {
        return repositories.get_or_create_effect(
            repositories.get_or_create_effect_composite_oneof(this->translate_level_0(effect->get_effects(), repositories)));
    }
    loki::Effect translate_level_2(loki::EffectCompositeProbabilistic effect, Repositories& repositories)
    {
        auto distribution = loki::EffectDistribution();
        for (const auto& [probability, effect] : effect->get_effect_distribution())
        {
            distribution.emplace_back(probability, this->translate_level_0(effect, repositories));
        }
        return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_probabilistic(distribution));
    }
    loki::Effect translate_level_2(loki::Effect effect, Repositories& repositories)
    {
        return std::visit([&](auto&& arg) -> loki::Effect { return this->translate_level_0(arg, repositories); }, effect->get_effect());
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionNumber function_expression, Repositories& repositories)
    {
        return repositories.get_or_create_function_expression(repositories.get_or_create_function_expression_number(function_expression->get_number()));
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionBinaryOperator function_expression, Repositories& repositories)
    {
        return repositories.get_or_create_function_expression(repositories.get_or_create_function_expression_binary_operator(
            function_expression->get_binary_operator(),
            this->translate_level_0(function_expression->get_left_function_expression(), repositories),
            this->translate_level_0(function_expression->get_right_function_expression(), repositories)));
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionMultiOperator function_expression, Repositories& repositories)
    {
        return repositories.get_or_create_function_expression(repositories.get_or_create_function_expression_multi_operator(
            function_expression->get_multi_operator(),
            this->translate_level_0(function_expression->get_function_expressions(), repositories)));
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionMinus function_expression, Repositories& repositories)
    {
        return repositories.get_or_create_function_expression(
            repositories.get_or_create_function_expression_minus(this->translate_level_0(function_expression->get_function_expression(), repositories)));
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpressionFunction function_expression, Repositories& repositories)
    {
        return repositories.get_or_create_function_expression(
            repositories.get_or_create_function_expression_function(this->translate_level_0(function_expression->get_function(), repositories)));
    }
    loki::FunctionExpression translate_level_2(loki::FunctionExpression function_expression, Repositories& repositories)
    {
        return std::visit([&](auto&& arg) -> loki::FunctionExpression { return this->translate_level_0(arg, repositories); },
                          function_expression->get_function_expression());
    }
    loki::FunctionSkeleton translate_level_2(loki::FunctionSkeleton function_skeleton, Repositories& repositories)
    {
        return repositories.get_or_create_function_skeleton(function_skeleton->get_name(),
                                                            this->translate_level_0(function_skeleton->get_parameters(), repositories),
                                                            this->translate_level_0(function_skeleton->get_type(), repositories));
    }
    loki::Function translate_level_2(loki::Function function, Repositories& repositories)
    {
        return repositories.get_or_create_function(this->translate_level_0(function->get_function_skeleton(), repositories),
                                                   this->translate_level_0(function->get_terms(), repositories));
    }
    loki::Action translate_level_2(loki::Action action, Repositories& repositories)
    {
        return repositories.get_or_create_action(
            action->get_name(),
            action->get_original_arity(),
            this->translate_level_0(action->get_parameters(), repositories),
            (action->get_condition().has_value() ? std::optional<loki::Condition>(this->translate_level_0(action->get_condition().value(), repositories)) :
                                                   std::nullopt),
            (action->get_effect().has_value() ? std::optional<loki::Effect>(this->translate_level_0(action->get_effect().value(), repositories)) :
                                                std::nullopt));
    }
    loki::Axiom translate_level_2(loki::Axiom axiom, Repositories& repositories)
    {
        return repositories.get_or_create_axiom(this->translate_level_0(axiom->get_parameters(), repositories),
                                                this->translate_level_0(axiom->get_literal(), repositories),
                                                this->translate_level_0(axiom->get_condition(), repositories));
    }
    loki::OptimizationMetric translate_level_2(loki::OptimizationMetric metric, Repositories& repositories)
    {
        return repositories.get_or_create_optimization_metric(metric->get_optimization_metric(),
                                                              this->translate_level_0(metric->get_function_expression(), repositories));
    }

    Domain translate_level_1(const Domain& domain, DomainBuilder& builder) { return self().translate_level_2(domain, builder); }

    Domain translate_level_2(const Domain& domain, DomainBuilder& builder)
    {
        auto& repositories = builder.get_repositories();

        builder.get_name() = domain->get_name();
        builder.get_filepath() = domain->get_filepath();
        builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
        const auto translated_types = this->translate_level_0(domain->get_types(), repositories);
        builder.get_types().insert(builder.get_types().end(), translated_types.begin(), translated_types.end());
        const auto translated_constants = this->translate_level_0(domain->get_constants(), repositories);
        builder.get_constants().insert(builder.get_constants().end(), translated_constants.begin(), translated_constants.end());
        const auto translated_predicates = this->translate_level_0(domain->get_predicates(), repositories);
        builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
        const auto translated_function_skeletons = this->translate_level_0(domain->get_function_skeletons(), repositories);
        builder.get_function_skeletons().insert(builder.get_function_skeletons().end(),
                                                translated_function_skeletons.begin(),
                                                translated_function_skeletons.end());
        const auto translated_actions = this->translate_level_0(domain->get_actions(), repositories);
        builder.get_actions().insert(builder.get_actions().end(), translated_actions.begin(), translated_actions.end());
        const auto translated_axioms = this->translate_level_0(domain->get_axioms(), repositories);
        builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

        return builder.get_result();
    }

    Problem translate_level_1(const Problem& problem, ProblemBuilder& builder) { return self().translate_level_2(problem, builder); }

    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder)
    {
        auto& repositories = builder.get_repositories();

        builder.get_filepath() = problem->get_filepath();
        builder.get_name() = problem->get_name();
        builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
        const auto translated_objects = this->translate_level_0(problem->get_objects(), repositories);
        builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
        const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
        builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());
        const auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);
        builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());
        const auto translated_initial_function_values = this->translate_level_0(problem->get_initial_function_values(), repositories);
        builder.get_initial_function_values().insert(builder.get_initial_function_values().end(),
                                             translated_initial_function_values.begin(),
                                             translated_initial_function_values.end());
        if (problem->get_goal_condition().has_value())
            builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition().value(), repositories);
        if (problem->get_optimization_metric().has_value())
            builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric().value(), repositories);
        const auto translated_axioms = this->translate_level_0(problem->get_axioms(), repositories);
        builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

        return builder.get_result(problem->get_index());
    }
};
}

#endif
