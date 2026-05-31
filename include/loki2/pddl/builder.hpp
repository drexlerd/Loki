/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI2_PDDL_BUILDER_HPP_
#define LOKI2_PDDL_BUILDER_HPP_

#include <cista/containers/optional.h>
#include <cista/containers/string.h>
#include <yggdrasil/formalism/builder.hpp>

#include "loki2/pddl/repository.hpp"

#include <utility>

namespace loki2::pddl
{

class Builder
{
private:
    ygg::formalism::BuilderStorage<Requirement,
                   Type,
                   Object,
                   Variable,
                   Parameter,
                   Predicate,
                   FunctionSkeleton,
                   Term,
                   Atom,
                   Literal,
                   FunctionExpressionNumber,
                   FunctionTerm,
                   UnaryFunctionExpression,
                   BinaryFunctionExpression,
                   MultiFunctionExpression,
                   FunctionExpression,
                   ConditionLiteral,
                   ConditionAnd,
                   ConditionOr,
                   ConditionNot,
                   ConditionImply,
                   ConditionExists,
                   ConditionForall,
                   ConditionNumericConstraint,
                   Condition,
                   EffectLiteral,
                   EffectAnd,
                   EffectNumeric,
                   EffectForall,
                   EffectWhen,
                   EffectOneOf,
                   EffectProbabilisticAlternative,
                   EffectProbabilistic,
                   Effect,
                   Action,
                   Axiom,
                   Metric,
                   InitialFunctionValue,
                   Domain,
                   Task> m_storage;

    template<typename T, typename... Args>
    [[nodiscard]] EntityView<T> emplace(Repository& repository, Args&&... args)
    {
        auto data_ptr = get_builder<T>();
        auto& data = *data_ptr;
        data.clear();
        data = ygg::Data<T>(std::forward<Args>(args)...);
        canonicalize(repository, data);
        return repository.get_or_create(data).first;
    }

public:
    Builder() = default;

    template<typename T>
    [[nodiscard]] auto get_builder()
    {
        return m_storage.template get_builder<T>();
    }

    template<typename T>
    [[nodiscard]] auto get_or_create(Repository& repository, ygg::Data<T>& data)
    {
        canonicalize(repository, data);
        return repository.get_or_create(data);
    }

    [[nodiscard]] RequirementView requirement(Repository& repository, RequirementKind kind) { return emplace<Requirement>(repository, kind); }
    [[nodiscard]] TypeView type(Repository& repository, ::cista::offset::string name, ygg::IndexList<Type> bases = {}) { return emplace<Type>(repository, std::move(name), std::move(bases)); }
    [[nodiscard]] ObjectView object(Repository& repository, ::cista::offset::string name, ygg::IndexList<Type> types = {}) { return emplace<Object>(repository, std::move(name), std::move(types)); }
    [[nodiscard]] VariableView variable(Repository& repository, ::cista::offset::string name) { return emplace<Variable>(repository, std::move(name)); }
    [[nodiscard]] ParameterView parameter(Repository& repository, ygg::Index<Variable> variable, ygg::IndexList<Type> types = {}) { return emplace<Parameter>(repository, variable, std::move(types)); }
    [[nodiscard]] PredicateView predicate(Repository& repository, ::cista::offset::string name, ygg::IndexList<Parameter> parameters = {}) { return emplace<Predicate>(repository, std::move(name), std::move(parameters)); }
    [[nodiscard]] FunctionSkeletonView function_skeleton(Repository& repository, ::cista::offset::string name, ygg::IndexList<Parameter> parameters, ygg::Index<Type> type) { return emplace<FunctionSkeleton>(repository, std::move(name), std::move(parameters), type); }
    [[nodiscard]] TermView term(Repository& repository, ygg::Data<Term>::Variant value) { return emplace<Term>(repository, std::move(value)); }
    [[nodiscard]] AtomView atom(Repository& repository, ygg::Index<Predicate> predicate, ygg::IndexList<Term> terms = {}) { return emplace<Atom>(repository, predicate, std::move(terms)); }
    [[nodiscard]] LiteralView literal(Repository& repository, bool positive, ygg::Index<Atom> atom) { return emplace<Literal>(repository, positive, atom); }

    [[nodiscard]] FunctionExpressionNumberView number(Repository& repository, double value) { return emplace<FunctionExpressionNumber>(repository, value); }
    [[nodiscard]] FunctionTermView function_term(Repository& repository, ygg::Index<FunctionSkeleton> function, ygg::IndexList<Term> terms = {}) { return emplace<FunctionTerm>(repository, function, std::move(terms)); }
    [[nodiscard]] UnaryFunctionExpressionView unary_function_expression(Repository& repository, UnaryArithmeticOperator op, ygg::Index<FunctionExpression> expression) { return emplace<UnaryFunctionExpression>(repository, op, expression); }
    [[nodiscard]] BinaryFunctionExpressionView binary_function_expression(Repository& repository, BinaryArithmeticOperator op, ygg::Index<FunctionExpression> left, ygg::Index<FunctionExpression> right) { return emplace<BinaryFunctionExpression>(repository, op, left, right); }
    [[nodiscard]] MultiFunctionExpressionView multi_function_expression(Repository& repository, MultiArithmeticOperator op, ygg::IndexList<FunctionExpression> expressions) { return emplace<MultiFunctionExpression>(repository, op, std::move(expressions)); }
    [[nodiscard]] FunctionExpressionView function_expression(Repository& repository, ygg::Data<FunctionExpression>::Variant value) { return emplace<FunctionExpression>(repository, std::move(value)); }

    [[nodiscard]] ConditionLiteralView condition_literal(Repository& repository, ygg::Index<Literal> literal) { return emplace<ConditionLiteral>(repository, literal); }
    [[nodiscard]] ConditionAndView condition_and(Repository& repository, ygg::IndexList<Condition> conditions) { return emplace<ConditionAnd>(repository, std::move(conditions)); }
    [[nodiscard]] ConditionOrView condition_or(Repository& repository, ygg::IndexList<Condition> conditions) { return emplace<ConditionOr>(repository, std::move(conditions)); }
    [[nodiscard]] ConditionNotView condition_not(Repository& repository, ygg::Index<Condition> condition) { return emplace<ConditionNot>(repository, condition); }
    [[nodiscard]] ConditionImplyView condition_imply(Repository& repository, ygg::Index<Condition> left, ygg::Index<Condition> right) { return emplace<ConditionImply>(repository, left, right); }
    [[nodiscard]] ConditionExistsView condition_exists(Repository& repository, ygg::IndexList<Parameter> parameters, ygg::Index<Condition> condition) { return emplace<ConditionExists>(repository, std::move(parameters), condition); }
    [[nodiscard]] ConditionForallView condition_forall(Repository& repository, ygg::IndexList<Parameter> parameters, ygg::Index<Condition> condition) { return emplace<ConditionForall>(repository, std::move(parameters), condition); }
    [[nodiscard]] ConditionNumericConstraintView condition_numeric_constraint(Repository& repository, BinaryComparator comparator, ygg::Index<FunctionExpression> left, ygg::Index<FunctionExpression> right) { return emplace<ConditionNumericConstraint>(repository, comparator, left, right); }
    [[nodiscard]] ConditionView condition(Repository& repository, ygg::Data<Condition>::Variant value) { return emplace<Condition>(repository, std::move(value)); }

    [[nodiscard]] EffectLiteralView effect_literal(Repository& repository, ygg::Index<Literal> literal) { return emplace<EffectLiteral>(repository, literal); }
    [[nodiscard]] EffectAndView effect_and(Repository& repository, ygg::IndexList<Effect> effects) { return emplace<EffectAnd>(repository, std::move(effects)); }
    [[nodiscard]] EffectNumericView effect_numeric(Repository& repository, NumericEffectOperator op, ygg::Index<FunctionSkeleton> function, ygg::IndexList<Term> terms, ygg::Index<FunctionExpression> expression) { return emplace<EffectNumeric>(repository, op, function, std::move(terms), expression); }
    [[nodiscard]] EffectForallView effect_forall(Repository& repository, ygg::IndexList<Parameter> parameters, ygg::Index<Effect> effect) { return emplace<EffectForall>(repository, std::move(parameters), effect); }
    [[nodiscard]] EffectWhenView effect_when(Repository& repository, ygg::Index<Condition> condition, ygg::Index<Effect> effect) { return emplace<EffectWhen>(repository, condition, effect); }
    [[nodiscard]] EffectOneOfView effect_one_of(Repository& repository, ygg::IndexList<Effect> effects) { return emplace<EffectOneOf>(repository, std::move(effects)); }
    [[nodiscard]] EffectProbabilisticAlternativeView effect_probabilistic_alternative(Repository& repository, double probability, ygg::Index<Effect> effect) { return emplace<EffectProbabilisticAlternative>(repository, probability, effect); }
    [[nodiscard]] EffectProbabilisticView effect_probabilistic(Repository& repository, ygg::IndexList<EffectProbabilisticAlternative> alternatives) { return emplace<EffectProbabilistic>(repository, std::move(alternatives)); }
    [[nodiscard]] EffectView effect(Repository& repository, ygg::Data<Effect>::Variant value) { return emplace<Effect>(repository, std::move(value)); }

    [[nodiscard]] ActionView action(Repository& repository, ::cista::offset::string name, ygg::IndexList<Parameter> parameters, ::cista::optional<ygg::Index<Condition>> precondition, ::cista::optional<ygg::Index<Effect>> effect) { return emplace<Action>(repository, std::move(name), std::move(parameters), precondition, effect); }
    [[nodiscard]] AxiomView axiom(Repository& repository, ygg::IndexList<Parameter> parameters, ygg::Index<Literal> head, ygg::Index<Condition> condition) { return emplace<Axiom>(repository, std::move(parameters), head, condition); }
    [[nodiscard]] MetricView metric(Repository& repository, bool minimize, ygg::Index<FunctionExpression> expression) { return emplace<Metric>(repository, minimize, expression); }
    [[nodiscard]] InitialFunctionValueView initial_function_value(Repository& repository, ygg::Index<FunctionTerm> function, ygg::Index<FunctionExpression> value) { return emplace<InitialFunctionValue>(repository, function, value); }

    [[nodiscard]] DomainView domain(Repository& repository, ygg::Data<Domain> data) { auto p = get_builder<Domain>(); auto& stored = *p; stored.clear(); stored = std::move(data); canonicalize(repository, stored); return repository.get_or_create(stored).first; }
    [[nodiscard]] TaskView task(Repository& repository, ygg::Data<Task> data) { auto p = get_builder<Task>(); auto& stored = *p; stored.clear(); stored = std::move(data); canonicalize(repository, stored); return repository.get_or_create(stored).first; }
};

}

#endif
