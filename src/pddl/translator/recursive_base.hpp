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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_BASE_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_RECURSIVE_BASE_HPP_

#include "interface.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/domain_builder.hpp"
#include "loki/details/pddl/problem.hpp"
#include "loki/details/pddl/problem_builder.hpp"

namespace loki
{
template<typename Derived>
class RecursiveBaseTranslator : public ITranslator<RecursiveBaseTranslator<Derived>>
{
private:
    RecursiveBaseTranslator() = default;
    friend Derived;

    /// @brief Helper to cast to Derived_.
    constexpr const auto& self() const { return static_cast<const Derived&>(*this); }
    constexpr auto& self() { return static_cast<Derived&>(*this); }

protected:
    friend class ITranslator<RecursiveBaseTranslator<Derived>>;

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
        std::ranges::for_each(range, [this](auto&& arg) { this->prepare_level_0(arg); });
    }
    template<typename T>
    void prepare_level_1(const std::optional<T>& element)
    {
        if (element.has_value())
        {
            self().prepare_level_2(element.value());
        }
    }
    template<typename T>
    void prepare_level_1(const T& element)
    {
        self().prepare_level_2(element);
    }

    void prepare_level_2(loki::Requirements) {}
    void prepare_level_2(loki::Type type) { this->prepare_level_0(type->get_bases()); }
    void prepare_level_2(loki::Object object) { this->prepare_level_0(object->get_bases()); }
    void prepare_level_2(loki::Variable) {}
    void prepare_level_2(loki::Term term)
    {
        std::visit([this](auto&& arg) { return this->prepare_level_0(arg); }, term->get_object_or_variable());
    }
    void prepare_level_2(loki::Parameter parameter) { this->prepare_level_0(parameter->get_variable()); }
    void prepare_level_2(loki::Predicate predicate) { this->prepare_level_0(predicate->get_parameters()); }
    void prepare_level_2(loki::Atom atom)
    {
        this->prepare_level_0(atom->get_predicate());
        this->prepare_level_0(atom->get_terms());
    }
    void prepare_level_2(loki::Literal literal) { this->prepare_level_0(literal->get_atom()); }
    void prepare_level_2(loki::FunctionValue function_value) { this->prepare_level_0(function_value->get_function()); }
    void prepare_level_2(loki::ConditionLiteral condition) { this->prepare_level_0(condition->get_literal()); }
    void prepare_level_2(loki::ConditionAnd condition) { this->prepare_level_0(condition->get_conditions()); }
    void prepare_level_2(loki::ConditionOr condition) { this->prepare_level_0(condition->get_conditions()); }
    void prepare_level_2(loki::ConditionNot condition) { this->prepare_level_0(condition->get_condition()); }
    void prepare_level_2(loki::ConditionImply condition)
    {
        this->prepare_level_0(condition->get_condition_left());
        this->prepare_level_0(condition->get_condition_right());
    }
    void prepare_level_2(loki::ConditionExists condition)
    {
        this->prepare_level_0(condition->get_parameters());
        this->prepare_level_0(condition->get_condition());
    }
    void prepare_level_2(loki::ConditionForall condition)
    {
        this->prepare_level_0(condition->get_parameters());
        this->prepare_level_0(condition->get_condition());
    }
    void prepare_level_2(loki::ConditionNumericConstraint condition)
    {
        this->prepare_level_0(condition->get_function_expression_left());
        this->prepare_level_0(condition->get_function_expression_right());
    }
    void prepare_level_2(loki::Condition condition)
    {
        std::visit([this](auto&& arg) { return this->prepare_level_0(arg); }, condition->get_condition());
    }
    void prepare_level_2(loki::EffectLiteral effect) { this->prepare_level_0(effect->get_literal()); }
    void prepare_level_2(loki::EffectAnd effect) { this->prepare_level_0(effect->get_effects()); }
    void prepare_level_2(loki::EffectNumeric effect)
    {
        this->prepare_level_0(effect->get_function());
        this->prepare_level_0(effect->get_function_expression());
    }
    void prepare_level_2(loki::EffectCompositeForall effect)
    {
        this->prepare_level_0(effect->get_parameters());
        this->prepare_level_0(effect->get_effect());
    }
    void prepare_level_2(loki::EffectCompositeWhen effect)
    {
        this->prepare_level_0(effect->get_condition());
        this->prepare_level_0(effect->get_effect());
    }
    void prepare_level_2(loki::EffectCompositeOneof effect) { this->prepare_level_0(effect->get_effects()); }
    void prepare_level_2(loki::EffectCompositeProbabilistic effect)
    {
        for (const auto& [probability, effect] : effect->get_effect_distribution())
        {
            this->prepare_level_0(effect);
        }
    }
    void prepare_level_2(loki::Effect effect)
    {
        std::visit([this](auto&& arg) { return this->prepare_level_0(arg); }, effect->get_effect());
    }
    void prepare_level_2(loki::FunctionExpressionNumber) {}
    void prepare_level_2(loki::FunctionExpressionBinaryOperator function_expression)
    {
        this->prepare_level_0(function_expression->get_left_function_expression());
        this->prepare_level_0(function_expression->get_right_function_expression());
    }
    void prepare_level_2(loki::FunctionExpressionMultiOperator function_expression) { this->prepare_level_0(function_expression->get_function_expressions()); }
    void prepare_level_2(loki::FunctionExpressionMinus function_expression) { this->prepare_level_0(function_expression->get_function_expression()); }
    void prepare_level_2(loki::FunctionExpressionFunction function_expression) { this->prepare_level_0(function_expression->get_function()); }
    void prepare_level_2(loki::FunctionExpression function_expression)
    {
        std::visit([this](auto&& arg) { return this->prepare_level_0(arg); }, function_expression->get_function_expression());
    }
    void prepare_level_2(loki::FunctionSkeleton function_skeleton)
    {
        this->prepare_level_0(function_skeleton->get_parameters());
        this->prepare_level_0(function_skeleton->get_type());
    }
    void prepare_level_2(loki::Function function)
    {
        this->prepare_level_0(function->get_function_skeleton());
        this->prepare_level_0(function->get_terms());
    }
    void prepare_level_2(loki::Action action)
    {
        this->prepare_level_0(action->get_parameters());
        this->prepare_level_0(action->get_condition());
        this->prepare_level_0(action->get_effect());
    }
    void prepare_level_2(loki::Axiom axiom)
    {
        this->prepare_level_0(axiom->get_parameters());
        this->prepare_level_0(axiom->get_literal());
        this->prepare_level_0(axiom->get_condition());
    }
    void prepare_level_2(loki::OptimizationMetric metric) { this->prepare_level_0(metric->get_function_expression()); }

    ///////////////////////////////////////////////////////
    /// Translate
    ///////////////////////////////////////////////////////

    template<IsBackInsertibleRange Range>
    auto translate_level_1(const Range& input, Repositories& repositories)
    {
        return self().translate_level_2(input, repositories);
    }
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
    auto translate_level_1(const std::optional<T>& element, Repositories& repositories)
    {
        return element.has_value() ? this->translate_level_0(element.value(), repositories) : std::optional<T> { std::nullopt };
    }
    template<typename T>
    auto translate_level_1(const T& element, Repositories& repositories)
    {
        return self().translate_level_2(element, repositories);
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
        return repositories.get_or_create_action(action->get_name(),
                                                 action->get_original_arity(),
                                                 this->translate_level_0(action->get_parameters(), repositories),
                                                 this->translate_level_0(action->get_condition(), repositories),
                                                 this->translate_level_0(action->get_effect(), repositories));
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

    auto translate_level_1(const Domain& domain, DomainBuilder& builder) { return self().translate_level_2(domain, builder); }

    auto translate_level_2(const Domain& domain, DomainBuilder& builder)
    {
        auto& repositories = builder.get_repositories();

        builder.get_name() = domain->get_name();
        builder.get_filepath() = domain->get_filepath();
        builder.get_requirements() = this->translate_level_0(domain->get_requirements(), repositories);
        builder.get_types() = this->translate_level_0(domain->get_types(), repositories);
        builder.get_constants() = this->translate_level_0(domain->get_constants(), repositories);
        builder.get_static_initial_literals() = this->translate_level_0(domain->get_static_initial_literals(), repositories);
        builder.get_predicates() = this->translate_level_0(domain->get_predicates(), repositories);
        builder.get_function_skeletons() = this->translate_level_0(domain->get_function_skeletons(), repositories);
        builder.get_actions() = this->translate_level_0(domain->get_actions(), repositories);
        builder.get_axioms() = this->translate_level_0(domain->get_axioms(), repositories);

        return builder.get_result();
    }

    auto translate_level_1(const Problem& problem, ProblemBuilder& builder) { return self().translate_level_2(problem, builder); }

    auto translate_level_2(const Problem& problem, ProblemBuilder& builder)
    {
        auto& repositories = builder.get_repositories();

        builder.get_filepath() = problem->get_filepath();
        builder.get_name() = problem->get_name();
        builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
        builder.get_objects() = this->translate_level_0(problem->get_objects(), repositories);
        builder.get_predicates() = this->translate_level_0(problem->get_predicates(), repositories);
        builder.get_initial_literals() = this->translate_level_0(problem->get_initial_literals(), repositories);
        builder.get_initial_function_values() = this->translate_level_0(problem->get_initial_function_values(), repositories);
        builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition(), repositories);
        builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric(), repositories);
        builder.get_axioms() = this->translate_level_0(problem->get_axioms(), repositories);

        return builder.get_result(problem->get_index());
    }
};
}

#endif
