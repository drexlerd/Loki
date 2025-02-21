/*
 * Copyright (C) 2023 Dominik Drexler
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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DOMAIN_BUILDER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DOMAIN_BUILDER_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/domain_repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class DomainBuilder
{
private:
    HanaDomainRepositories m_repositories;

    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    std::unordered_map<std::string, Type> m_types;
    std::unordered_map<std::string, Object> m_constants;
    std::unordered_map<std::string, Predicate> m_predicates;
    std::unordered_map<std::string, FunctionSkeleton> m_functions;
    ActionSet m_actions;
    AxiomSet m_axioms;

public:
    DomainBuilder() = default;

    /// @brief Finalizes the `Domain` and returns it.
    /// The `DomainBuilder` is in an invalid state afterwards.
    /// @return the resulting `Domain`.
    Domain get_result();

    Requirements get_or_create_requirements(RequirementEnumSet requirement_set);

    Type get_or_create_type(std::string name, TypeList bases);

    Variable get_or_create_variable(std::string name);

    Object get_or_create_object(std::string name, TypeList types);

    Term get_or_create_term(Variable variable);
    Term get_or_create_term(Object object);

    Parameter get_or_create_parameter(Variable variable, TypeList types);

    Predicate get_or_create_predicate(std::string name, ParameterList parameters);

    Atom get_or_create_atom(Predicate predicate, TermList terms);

    Literal get_or_create_literal(bool is_negated, Atom atom);

    FunctionExpressionNumber get_or_create_function_expression_number(double number);
    FunctionExpressionBinaryOperator get_or_create_function_expression_binary_operator(BinaryOperatorEnum binary_operator,
                                                                                       FunctionExpression left_function_expression,
                                                                                       FunctionExpression right_function_expression);
    FunctionExpressionMultiOperator get_or_create_function_expression_multi_operator(MultiOperatorEnum multi_operator,
                                                                                     FunctionExpressionList function_expressions_);
    FunctionExpressionMinus get_or_create_function_expression_minus(FunctionExpression function_expression);
    FunctionExpressionFunction get_or_create_function_expression_function(Function function);
    FunctionExpression get_or_create_function_expression(FunctionExpressionNumber fexpr_number);
    FunctionExpression get_or_create_function_expression(FunctionExpressionBinaryOperator fexpr_binary_op);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMultiOperator fexpr_multi_op);
    FunctionExpression get_or_create_function_expression(FunctionExpressionMinus fexpr_minus);
    FunctionExpression get_or_create_function_expression(FunctionExpressionFunction fexpr_function);

    Function get_or_create_function(FunctionSkeleton function_skeleton, TermList terms);

    FunctionSkeleton get_or_create_function_skeleton(std::string name, ParameterList parameters, Type type);

    ConditionLiteral get_or_create_condition_literal(Literal literal);
    ConditionAnd get_or_create_condition_and(ConditionList conditions_);
    ConditionOr get_or_create_condition_or(ConditionList conditions_);
    ConditionNot get_or_create_condition_not(Condition condition);
    ConditionImply get_or_create_condition_imply(Condition condition_left, Condition condition_right);
    ConditionExists get_or_create_condition_exists(ParameterList parameters, Condition condition);

    ConditionForall get_or_create_condition_forall(ParameterList parameters, Condition condition);
    ConditionNumericConstraint get_or_create_condition_numeric_constraint(BinaryComparatorEnum binary_comparator,
                                                                          FunctionExpression function_expression_left,
                                                                          FunctionExpression function_expression_right);
    Condition get_or_create_condition(ConditionLiteral condition);
    Condition get_or_create_condition(ConditionAnd condition);
    Condition get_or_create_condition(ConditionOr condition);
    Condition get_or_create_condition(ConditionNot condition);
    Condition get_or_create_condition(ConditionImply condition);
    Condition get_or_create_condition(ConditionExists condition);
    Condition get_or_create_condition(ConditionForall condition);
    Condition get_or_create_condition(ConditionNumericConstraint condition);

    EffectLiteral get_or_create_effect_literal(Literal literal);
    EffectAnd get_or_create_effect_and(EffectList effects);
    EffectNumeric get_or_create_effect_numeric(AssignOperatorEnum assign_operator, Function function, FunctionExpression function_expression);
    EffectCompositeForall get_or_create_effect_composite_forall(ParameterList parameters, Effect effect);
    EffectCompositeWhen get_or_create_effect_composite_when(Condition condition, Effect effect);
    EffectCompositeOneof get_or_create_effect_composite_oneof(EffectList effects);
    EffectCompositeProbabilistic get_or_create_effect_composite_probabilistic(EffectDistribution effects);
    Effect get_or_create_effect(EffectLiteral effect);
    Effect get_or_create_effect(EffectAnd effect);
    Effect get_or_create_effect(EffectNumeric effect);
    Effect get_or_create_effect(EffectCompositeForall effect);
    Effect get_or_create_effect(EffectCompositeWhen effect);
    Effect get_or_create_effect(EffectCompositeOneof effect);
    Effect get_or_create_effect(EffectCompositeProbabilistic effect);

    Action
    get_or_create_action(std::string name, size_t original_arity, ParameterList parameters, std::optional<Condition> condition, std::optional<Effect> effect);

    Axiom get_or_create_axiom(ParameterList parameters, Literal subtyped_literal, Condition condition);

    /**
     * Domain-specific modifiers.
     * Return true iff the element was newly inserted into the domain and false otherwise.
     */

    void set_requirements(Requirements requirements);
    void insert_type(Type type);
    void insert_constant(Object constant);
    void insert_predicate(Predicate predicate);
    void insert_function_skeleton(FunctionSkeleton function);
    void insert_action(Action action);
    void insert_axiom(Axiom axiom);

    /**
     * Domain-specific accessors.
     * Return true iff the element exist in the domain and false otherwise.
     */

    void contains_requirements(RequirementEnum requirement) const;
    void contains_type(const std::string& name) const;
    void contains_constant(const std::string& name) const;
    void contains_predicate(const std::string& name) const;
    void contains_function_skeleton(const std::string& name) const;
    void contains_action(Action action) const;
    void contains_axiom(Axiom axiom) const;
};
}

#endif
