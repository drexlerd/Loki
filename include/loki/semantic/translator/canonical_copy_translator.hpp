/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_CANONICAL_COPY_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CANONICAL_COPY_TRANSLATOR_HPP_

#include "loki/semantic/translator/common.hpp"

namespace loki::semantic::detail
{

class CanonicalCopyTranslator
{
public:
    explicit CanonicalCopyTranslator(std::shared_ptr<TranslationStorage> storage) : m_storage(std::move(storage)) {}

    formalism::DomainView copy_domain(formalism::DomainView domain)
    {
        m_storage->original_domain = domain.get_index();
        const auto& source = domain.get_context();
        auto data = domain.get_data();
        data.index = {};
        data.requirements = copy_list(data.requirements, source);
        data.types = copy_list(data.types, source);
        data.constants = copy_list(data.constants, source);
        data.predicates = copy_list(data.predicates, source);
        data.functions = copy_list(data.functions, source);
        data.actions = copy_list(data.actions, source);
        data.axioms = copy_list(data.axioms, source);
        auto view = formalism::get_or_create<formalism::Domain>(m_storage->repository, std::move(data));
        m_storage->translated_domain = view.get_index();
        return view;
    }

    formalism::TaskView copy_task(formalism::TaskView task)
    {
        const auto& source = task.get_context();
        auto data = task.get_data();
        data.index = {};
        data.domain = copy(data.domain, source);
        data.requirements = copy_list(data.requirements, source);
        data.objects = copy_list(data.objects, source);
        data.initial_literals = copy_list(data.initial_literals, source);
        data.initial_function_values = copy_list(data.initial_function_values, source);
        data.goal = copy_optional(data.goal, source);
        data.metric = copy_optional(data.metric, source);
        data.predicates = copy_list(data.predicates, source);
        data.axioms = copy_list(data.axioms, source);
        auto view = formalism::get_or_create<formalism::Task>(m_storage->repository, std::move(data));
        remember(m_storage->tasks, task.get_index(), view.get_index());
        return view;
    }

private:
    std::shared_ptr<TranslationStorage> m_storage;

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const formalism::Repository& repository)
    {
        auto result = ygg::IndexList<T> {};
        for (auto index : source)
            result.push_back(copy(index, repository));
        return result;
    }

    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const formalism::Repository& repository)
    {
        if (!source)
            return {};
        return copy(*source, repository);
    }

    ygg::Index<formalism::Requirement> copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Requirement> out;
        if (find_mapped(m_storage->requirements, source, out)) return out;
        out = formalism::get_or_create<formalism::Requirement>(m_storage->repository, repository[source].kind).get_index();
        remember(m_storage->requirements, source, out);
        return out;
    }

    ygg::Index<formalism::Type> copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Type> out;
        if (find_mapped(m_storage->types, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Type>(m_storage->repository, data.name, copy_list(data.bases, repository)).get_index();
        remember(m_storage->types, source, out);
        return out;
    }

    ygg::Index<formalism::Object> copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Object> out;
        if (find_mapped(m_storage->objects, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Object>(m_storage->repository, data.name, copy_list(data.types, repository)).get_index();
        remember(m_storage->objects, source, out);
        return out;
    }

    ygg::Index<formalism::Variable> copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Variable> out;
        if (find_mapped(m_storage->variables, source, out)) return out;
        out = formalism::get_or_create<formalism::Variable>(m_storage->repository, repository[source].name).get_index();
        remember(m_storage->variables, source, out);
        return out;
    }

    ygg::Index<formalism::Parameter> copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Parameter> out;
        if (find_mapped(m_storage->parameters, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Parameter>(m_storage->repository, copy(data.variable, repository), copy_list(data.types, repository)).get_index();
        remember(m_storage->parameters, source, out);
        return out;
    }

    ygg::Index<formalism::Predicate> copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Predicate> out;
        if (find_mapped(m_storage->predicates, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Predicate>(m_storage->repository, data.name, copy_list(data.parameters, repository)).get_index();
        remember(m_storage->predicates, source, out);
        return out;
    }

    ygg::Index<formalism::FunctionSkeleton> copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::FunctionSkeleton> out;
        if (find_mapped(m_storage->functions, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::FunctionSkeleton>(m_storage->repository, data.name, copy_list(data.parameters, repository), copy(data.type, repository)).get_index();
        remember(m_storage->functions, source, out);
        return out;
    }

    ygg::Index<formalism::Term> copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Term> out;
        if (find_mapped(m_storage->terms, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant { return ygg::Data<formalism::Term>::Variant(copy(arg, repository)); }, repository[source].value);
        out = formalism::get_or_create<formalism::Term>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->terms, source, out);
        return out;
    }

    ygg::Index<formalism::Atom> copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Atom> out;
        if (find_mapped(m_storage->atoms, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Atom>(m_storage->repository, copy(data.predicate, repository), copy_list(data.terms, repository)).get_index();
        remember(m_storage->atoms, source, out);
        return out;
    }

    ygg::Index<formalism::Literal> copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Literal> out;
        if (find_mapped(m_storage->literals, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Literal>(m_storage->repository, data.positive, copy(data.atom, repository)).get_index();
        remember(m_storage->literals, source, out);
        return out;
    }

    ygg::Index<formalism::FunctionExpressionNumber> copy(ygg::Index<formalism::FunctionExpressionNumber> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::FunctionExpressionNumber> out;
        if (find_mapped(m_storage->numbers, source, out)) return out;
        out = formalism::get_or_create<formalism::FunctionExpressionNumber>(m_storage->repository, repository[source].value).get_index();
        remember(m_storage->numbers, source, out);
        return out;
    }

    ygg::Index<formalism::FunctionTerm> copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::FunctionTerm> out;
        if (find_mapped(m_storage->function_terms, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::FunctionTerm>(m_storage->repository, copy(data.function, repository), copy_list(data.terms, repository)).get_index();
        remember(m_storage->function_terms, source, out);
        return out;
    }

    ygg::Index<formalism::UnaryFunctionExpression> copy(ygg::Index<formalism::UnaryFunctionExpression> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::UnaryFunctionExpression> out;
        if (find_mapped(m_storage->unary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::UnaryFunctionExpression>(m_storage->repository, data.op, copy(data.expression, repository)).get_index();
        remember(m_storage->unary_expressions, source, out);
        return out;
    }

    ygg::Index<formalism::BinaryFunctionExpression> copy(ygg::Index<formalism::BinaryFunctionExpression> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::BinaryFunctionExpression> out;
        if (find_mapped(m_storage->binary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::BinaryFunctionExpression>(m_storage->repository, data.op, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->binary_expressions, source, out);
        return out;
    }

    ygg::Index<formalism::MultiFunctionExpression> copy(ygg::Index<formalism::MultiFunctionExpression> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::MultiFunctionExpression> out;
        if (find_mapped(m_storage->multi_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::MultiFunctionExpression>(m_storage->repository, data.op, copy_list(data.expressions, repository)).get_index();
        remember(m_storage->multi_expressions, source, out);
        return out;
    }

    ygg::Index<formalism::FunctionExpression> copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::FunctionExpression> out;
        if (find_mapped(m_storage->function_expressions, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant { return ygg::Data<formalism::FunctionExpression>::Variant(copy(arg, repository)); }, repository[source].value);
        out = formalism::get_or_create<formalism::FunctionExpression>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->function_expressions, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionLiteral> copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionLiteral> out;
        if (find_mapped(m_storage->condition_literals, source, out)) return out;
        out = formalism::get_or_create<formalism::ConditionLiteral>(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->condition_literals, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionAnd> copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionAnd> out;
        if (find_mapped(m_storage->condition_ands, source, out)) return out;
        out = formalism::get_or_create<formalism::ConditionAnd>(m_storage->repository, copy_list(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ands, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionOr> copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionOr> out;
        if (find_mapped(m_storage->condition_ors, source, out)) return out;
        out = formalism::get_or_create<formalism::ConditionOr>(m_storage->repository, copy_list(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ors, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionNot> copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionNot> out;
        if (find_mapped(m_storage->condition_nots, source, out)) return out;
        out = formalism::get_or_create<formalism::ConditionNot>(m_storage->repository, copy(repository[source].condition, repository)).get_index();
        remember(m_storage->condition_nots, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionImply> copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionImply> out;
        if (find_mapped(m_storage->condition_implies, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::ConditionImply>(m_storage->repository, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_implies, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionExists> copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionExists> out;
        if (find_mapped(m_storage->condition_exists, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::ConditionExists>(m_storage->repository, copy_list(data.parameters, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->condition_exists, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionForall> copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionForall> out;
        if (find_mapped(m_storage->condition_foralls, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::ConditionForall>(m_storage->repository, copy_list(data.parameters, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->condition_foralls, source, out);
        return out;
    }

    ygg::Index<formalism::ConditionNumericConstraint> copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::ConditionNumericConstraint> out;
        if (find_mapped(m_storage->condition_numeric_constraints, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::ConditionNumericConstraint>(m_storage->repository, data.comparator, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_numeric_constraints, source, out);
        return out;
    }

    ygg::Index<formalism::Condition> copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Condition> out;
        if (find_mapped(m_storage->conditions, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Condition>::Variant { return ygg::Data<formalism::Condition>::Variant(copy(arg, repository)); }, repository[source].value);
        out = formalism::get_or_create<formalism::Condition>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->conditions, source, out);
        return out;
    }

    ygg::Index<formalism::EffectLiteral> copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectLiteral> out;
        if (find_mapped(m_storage->effect_literals, source, out)) return out;
        out = formalism::get_or_create<formalism::EffectLiteral>(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->effect_literals, source, out);
        return out;
    }

    ygg::Index<formalism::EffectAnd> copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectAnd> out;
        if (find_mapped(m_storage->effect_ands, source, out)) return out;
        out = formalism::get_or_create<formalism::EffectAnd>(m_storage->repository, copy_list(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_ands, source, out);
        return out;
    }

    ygg::Index<formalism::EffectNumeric> copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectNumeric> out;
        if (find_mapped(m_storage->effect_numerics, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::EffectNumeric>(m_storage->repository, data.op, copy(data.function, repository), copy_list(data.terms, repository), copy(data.expression, repository)).get_index();
        remember(m_storage->effect_numerics, source, out);
        return out;
    }

    ygg::Index<formalism::EffectForall> copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectForall> out;
        if (find_mapped(m_storage->effect_foralls, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::EffectForall>(m_storage->repository, copy_list(data.parameters, repository), copy(data.effect, repository)).get_index();
        remember(m_storage->effect_foralls, source, out);
        return out;
    }

    ygg::Index<formalism::EffectWhen> copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectWhen> out;
        if (find_mapped(m_storage->effect_whens, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::EffectWhen>(m_storage->repository, copy(data.condition, repository), copy(data.effect, repository)).get_index();
        remember(m_storage->effect_whens, source, out);
        return out;
    }

    ygg::Index<formalism::EffectOneOf> copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectOneOf> out;
        if (find_mapped(m_storage->effect_one_ofs, source, out)) return out;
        out = formalism::get_or_create<formalism::EffectOneOf>(m_storage->repository, copy_list(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_one_ofs, source, out);
        return out;
    }

    ygg::Index<formalism::EffectProbabilisticAlternative> copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectProbabilisticAlternative> out;
        if (find_mapped(m_storage->effect_probabilistic_alternatives, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::EffectProbabilisticAlternative>(m_storage->repository, data.probability, copy(data.effect, repository)).get_index();
        remember(m_storage->effect_probabilistic_alternatives, source, out);
        return out;
    }

    ygg::Index<formalism::EffectProbabilistic> copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::EffectProbabilistic> out;
        if (find_mapped(m_storage->effect_probabilistics, source, out)) return out;
        out = formalism::get_or_create<formalism::EffectProbabilistic>(m_storage->repository, copy_list(repository[source].alternatives, repository)).get_index();
        remember(m_storage->effect_probabilistics, source, out);
        return out;
    }

    ygg::Index<formalism::Effect> copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Effect> out;
        if (find_mapped(m_storage->effects, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return ygg::Data<formalism::Effect>::Variant(copy(arg, repository)); }, repository[source].value);
        out = formalism::get_or_create<formalism::Effect>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->effects, source, out);
        return out;
    }

    ygg::Index<formalism::Action> copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Action> out;
        if (find_mapped(m_storage->actions, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Action>(m_storage->repository, data.name, copy_list(data.parameters, repository), copy_optional(data.precondition, repository), copy_optional(data.effect, repository)).get_index();
        remember(m_storage->actions, source, out);
        return out;
    }

    ygg::Index<formalism::Axiom> copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Axiom> out;
        if (find_mapped(m_storage->axioms, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Axiom>(m_storage->repository, copy_list(data.parameters, repository), copy(data.head, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->axioms, source, out);
        return out;
    }

    ygg::Index<formalism::Metric> copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Metric> out;
        if (find_mapped(m_storage->metrics, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::Metric>(m_storage->repository, data.minimize, copy(data.expression, repository)).get_index();
        remember(m_storage->metrics, source, out);
        return out;
    }

    ygg::Index<formalism::InitialFunctionValue> copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::InitialFunctionValue> out;
        if (find_mapped(m_storage->initial_function_values, source, out)) return out;
        const auto& data = repository[source];
        out = formalism::get_or_create<formalism::InitialFunctionValue>(m_storage->repository, copy(data.function, repository), copy(data.value, repository)).get_index();
        remember(m_storage->initial_function_values, source, out);
        return out;
    }

    ygg::Index<formalism::Domain> copy(ygg::Index<formalism::Domain> source, const formalism::Repository& repository)
    {
        ygg::Index<formalism::Domain> out;
        if (find_mapped(m_storage->domains, source, out)) return out;
        const auto& data = repository[source];
        auto copied = data;
        copied.index = {};
        copied.requirements = copy_list(data.requirements, repository);
        copied.types = copy_list(data.types, repository);
        copied.constants = copy_list(data.constants, repository);
        copied.predicates = copy_list(data.predicates, repository);
        copied.functions = copy_list(data.functions, repository);
        copied.actions = copy_list(data.actions, repository);
        copied.axioms = copy_list(data.axioms, repository);
        out = formalism::get_or_create<formalism::Domain>(m_storage->repository, std::move(copied)).get_index();
        remember(m_storage->domains, source, out);
        m_storage->translated_domain = out;
        return out;
    }
};

} // namespace loki::semantic::detail

#endif
