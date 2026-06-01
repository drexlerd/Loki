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

    pddl::DomainView copy_domain(pddl::DomainView domain)
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
        auto view = pddl::get_or_create<pddl::Domain>(m_storage->repository, std::move(data));
        m_storage->translated_domain = view.get_index();
        return view;
    }

    pddl::TaskView copy_task(pddl::TaskView task)
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
        data.axioms = copy_list(data.axioms, source);
        auto view = pddl::get_or_create<pddl::Task>(m_storage->repository, std::move(data));
        remember(m_storage->tasks, task.get_index(), view.get_index());
        return view;
    }

private:
    std::shared_ptr<TranslationStorage> m_storage;

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const pddl::Repository& repository)
    {
        auto result = ygg::IndexList<T> {};
        for (auto index : source)
            result.push_back(copy(index, repository));
        return result;
    }

    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const pddl::Repository& repository)
    {
        if (!source)
            return {};
        return copy(*source, repository);
    }

    ygg::Index<pddl::Requirement> copy(ygg::Index<pddl::Requirement> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Requirement> out;
        if (find_mapped(m_storage->requirements, source, out)) return out;
        out = pddl::get_or_create<pddl::Requirement>(m_storage->repository, repository[source].kind).get_index();
        remember(m_storage->requirements, source, out);
        return out;
    }

    ygg::Index<pddl::Type> copy(ygg::Index<pddl::Type> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Type> out;
        if (find_mapped(m_storage->types, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Type>(m_storage->repository, data.name, copy_list(data.bases, repository)).get_index();
        remember(m_storage->types, source, out);
        return out;
    }

    ygg::Index<pddl::Object> copy(ygg::Index<pddl::Object> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Object> out;
        if (find_mapped(m_storage->objects, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Object>(m_storage->repository, data.name, copy_list(data.types, repository)).get_index();
        remember(m_storage->objects, source, out);
        return out;
    }

    ygg::Index<pddl::Variable> copy(ygg::Index<pddl::Variable> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Variable> out;
        if (find_mapped(m_storage->variables, source, out)) return out;
        out = pddl::get_or_create<pddl::Variable>(m_storage->repository, repository[source].name).get_index();
        remember(m_storage->variables, source, out);
        return out;
    }

    ygg::Index<pddl::Parameter> copy(ygg::Index<pddl::Parameter> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Parameter> out;
        if (find_mapped(m_storage->parameters, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Parameter>(m_storage->repository, copy(data.variable, repository), copy_list(data.types, repository)).get_index();
        remember(m_storage->parameters, source, out);
        return out;
    }

    ygg::Index<pddl::Predicate> copy(ygg::Index<pddl::Predicate> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Predicate> out;
        if (find_mapped(m_storage->predicates, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Predicate>(m_storage->repository, data.name, copy_list(data.parameters, repository)).get_index();
        remember(m_storage->predicates, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionSkeleton> copy(ygg::Index<pddl::FunctionSkeleton> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionSkeleton> out;
        if (find_mapped(m_storage->functions, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::FunctionSkeleton>(m_storage->repository, data.name, copy_list(data.parameters, repository), copy(data.type, repository)).get_index();
        remember(m_storage->functions, source, out);
        return out;
    }

    ygg::Index<pddl::Term> copy(ygg::Index<pddl::Term> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Term> out;
        if (find_mapped(m_storage->terms, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Term>::Variant { return ygg::Data<pddl::Term>::Variant(copy(arg, repository)); }, repository[source].value);
        out = pddl::get_or_create<pddl::Term>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->terms, source, out);
        return out;
    }

    ygg::Index<pddl::Atom> copy(ygg::Index<pddl::Atom> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Atom> out;
        if (find_mapped(m_storage->atoms, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Atom>(m_storage->repository, copy(data.predicate, repository), copy_list(data.terms, repository)).get_index();
        remember(m_storage->atoms, source, out);
        return out;
    }

    ygg::Index<pddl::Literal> copy(ygg::Index<pddl::Literal> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Literal> out;
        if (find_mapped(m_storage->literals, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Literal>(m_storage->repository, data.positive, copy(data.atom, repository)).get_index();
        remember(m_storage->literals, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionExpressionNumber> copy(ygg::Index<pddl::FunctionExpressionNumber> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionExpressionNumber> out;
        if (find_mapped(m_storage->numbers, source, out)) return out;
        out = pddl::get_or_create<pddl::FunctionExpressionNumber>(m_storage->repository, repository[source].value).get_index();
        remember(m_storage->numbers, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionTerm> copy(ygg::Index<pddl::FunctionTerm> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionTerm> out;
        if (find_mapped(m_storage->function_terms, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::FunctionTerm>(m_storage->repository, copy(data.function, repository), copy_list(data.terms, repository)).get_index();
        remember(m_storage->function_terms, source, out);
        return out;
    }

    ygg::Index<pddl::UnaryFunctionExpression> copy(ygg::Index<pddl::UnaryFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::UnaryFunctionExpression> out;
        if (find_mapped(m_storage->unary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::UnaryFunctionExpression>(m_storage->repository, data.op, copy(data.expression, repository)).get_index();
        remember(m_storage->unary_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::BinaryFunctionExpression> copy(ygg::Index<pddl::BinaryFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::BinaryFunctionExpression> out;
        if (find_mapped(m_storage->binary_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::BinaryFunctionExpression>(m_storage->repository, data.op, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->binary_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::MultiFunctionExpression> copy(ygg::Index<pddl::MultiFunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::MultiFunctionExpression> out;
        if (find_mapped(m_storage->multi_expressions, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::MultiFunctionExpression>(m_storage->repository, data.op, copy_list(data.expressions, repository)).get_index();
        remember(m_storage->multi_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::FunctionExpression> copy(ygg::Index<pddl::FunctionExpression> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::FunctionExpression> out;
        if (find_mapped(m_storage->function_expressions, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::FunctionExpression>::Variant { return ygg::Data<pddl::FunctionExpression>::Variant(copy(arg, repository)); }, repository[source].value);
        out = pddl::get_or_create<pddl::FunctionExpression>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->function_expressions, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionLiteral> copy(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionLiteral> out;
        if (find_mapped(m_storage->condition_literals, source, out)) return out;
        out = pddl::get_or_create<pddl::ConditionLiteral>(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->condition_literals, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionAnd> copy(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionAnd> out;
        if (find_mapped(m_storage->condition_ands, source, out)) return out;
        out = pddl::get_or_create<pddl::ConditionAnd>(m_storage->repository, copy_list(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ands, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionOr> copy(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionOr> out;
        if (find_mapped(m_storage->condition_ors, source, out)) return out;
        out = pddl::get_or_create<pddl::ConditionOr>(m_storage->repository, copy_list(repository[source].conditions, repository)).get_index();
        remember(m_storage->condition_ors, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionNot> copy(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionNot> out;
        if (find_mapped(m_storage->condition_nots, source, out)) return out;
        out = pddl::get_or_create<pddl::ConditionNot>(m_storage->repository, copy(repository[source].condition, repository)).get_index();
        remember(m_storage->condition_nots, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionImply> copy(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionImply> out;
        if (find_mapped(m_storage->condition_implies, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::ConditionImply>(m_storage->repository, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_implies, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionExists> copy(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionExists> out;
        if (find_mapped(m_storage->condition_exists, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::ConditionExists>(m_storage->repository, copy_list(data.parameters, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->condition_exists, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionForall> copy(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionForall> out;
        if (find_mapped(m_storage->condition_foralls, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::ConditionForall>(m_storage->repository, copy_list(data.parameters, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->condition_foralls, source, out);
        return out;
    }

    ygg::Index<pddl::ConditionNumericConstraint> copy(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::ConditionNumericConstraint> out;
        if (find_mapped(m_storage->condition_numeric_constraints, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::ConditionNumericConstraint>(m_storage->repository, data.comparator, copy(data.left, repository), copy(data.right, repository)).get_index();
        remember(m_storage->condition_numeric_constraints, source, out);
        return out;
    }

    ygg::Index<pddl::Condition> copy(ygg::Index<pddl::Condition> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Condition> out;
        if (find_mapped(m_storage->conditions, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Condition>::Variant { return ygg::Data<pddl::Condition>::Variant(copy(arg, repository)); }, repository[source].value);
        out = pddl::get_or_create<pddl::Condition>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->conditions, source, out);
        return out;
    }

    ygg::Index<pddl::EffectLiteral> copy(ygg::Index<pddl::EffectLiteral> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectLiteral> out;
        if (find_mapped(m_storage->effect_literals, source, out)) return out;
        out = pddl::get_or_create<pddl::EffectLiteral>(m_storage->repository, copy(repository[source].literal, repository)).get_index();
        remember(m_storage->effect_literals, source, out);
        return out;
    }

    ygg::Index<pddl::EffectAnd> copy(ygg::Index<pddl::EffectAnd> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectAnd> out;
        if (find_mapped(m_storage->effect_ands, source, out)) return out;
        out = pddl::get_or_create<pddl::EffectAnd>(m_storage->repository, copy_list(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_ands, source, out);
        return out;
    }

    ygg::Index<pddl::EffectNumeric> copy(ygg::Index<pddl::EffectNumeric> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectNumeric> out;
        if (find_mapped(m_storage->effect_numerics, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::EffectNumeric>(m_storage->repository, data.op, copy(data.function, repository), copy_list(data.terms, repository), copy(data.expression, repository)).get_index();
        remember(m_storage->effect_numerics, source, out);
        return out;
    }

    ygg::Index<pddl::EffectForall> copy(ygg::Index<pddl::EffectForall> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectForall> out;
        if (find_mapped(m_storage->effect_foralls, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::EffectForall>(m_storage->repository, copy_list(data.parameters, repository), copy(data.effect, repository)).get_index();
        remember(m_storage->effect_foralls, source, out);
        return out;
    }

    ygg::Index<pddl::EffectWhen> copy(ygg::Index<pddl::EffectWhen> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectWhen> out;
        if (find_mapped(m_storage->effect_whens, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::EffectWhen>(m_storage->repository, copy(data.condition, repository), copy(data.effect, repository)).get_index();
        remember(m_storage->effect_whens, source, out);
        return out;
    }

    ygg::Index<pddl::EffectOneOf> copy(ygg::Index<pddl::EffectOneOf> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectOneOf> out;
        if (find_mapped(m_storage->effect_one_ofs, source, out)) return out;
        out = pddl::get_or_create<pddl::EffectOneOf>(m_storage->repository, copy_list(repository[source].effects, repository)).get_index();
        remember(m_storage->effect_one_ofs, source, out);
        return out;
    }

    ygg::Index<pddl::EffectProbabilisticAlternative> copy(ygg::Index<pddl::EffectProbabilisticAlternative> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectProbabilisticAlternative> out;
        if (find_mapped(m_storage->effect_probabilistic_alternatives, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::EffectProbabilisticAlternative>(m_storage->repository, data.probability, copy(data.effect, repository)).get_index();
        remember(m_storage->effect_probabilistic_alternatives, source, out);
        return out;
    }

    ygg::Index<pddl::EffectProbabilistic> copy(ygg::Index<pddl::EffectProbabilistic> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::EffectProbabilistic> out;
        if (find_mapped(m_storage->effect_probabilistics, source, out)) return out;
        out = pddl::get_or_create<pddl::EffectProbabilistic>(m_storage->repository, copy_list(repository[source].alternatives, repository)).get_index();
        remember(m_storage->effect_probabilistics, source, out);
        return out;
    }

    ygg::Index<pddl::Effect> copy(ygg::Index<pddl::Effect> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Effect> out;
        if (find_mapped(m_storage->effects, source, out)) return out;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Effect>::Variant { return ygg::Data<pddl::Effect>::Variant(copy(arg, repository)); }, repository[source].value);
        out = pddl::get_or_create<pddl::Effect>(m_storage->repository, std::move(value)).get_index();
        remember(m_storage->effects, source, out);
        return out;
    }

    ygg::Index<pddl::Action> copy(ygg::Index<pddl::Action> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Action> out;
        if (find_mapped(m_storage->actions, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Action>(m_storage->repository, data.name, copy_list(data.parameters, repository), copy_optional(data.precondition, repository), copy_optional(data.effect, repository)).get_index();
        remember(m_storage->actions, source, out);
        return out;
    }

    ygg::Index<pddl::Axiom> copy(ygg::Index<pddl::Axiom> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Axiom> out;
        if (find_mapped(m_storage->axioms, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Axiom>(m_storage->repository, copy_list(data.parameters, repository), copy(data.head, repository), copy(data.condition, repository)).get_index();
        remember(m_storage->axioms, source, out);
        return out;
    }

    ygg::Index<pddl::Metric> copy(ygg::Index<pddl::Metric> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Metric> out;
        if (find_mapped(m_storage->metrics, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::Metric>(m_storage->repository, data.minimize, copy(data.expression, repository)).get_index();
        remember(m_storage->metrics, source, out);
        return out;
    }

    ygg::Index<pddl::InitialFunctionValue> copy(ygg::Index<pddl::InitialFunctionValue> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::InitialFunctionValue> out;
        if (find_mapped(m_storage->initial_function_values, source, out)) return out;
        const auto& data = repository[source];
        out = pddl::get_or_create<pddl::InitialFunctionValue>(m_storage->repository, copy(data.function, repository), copy(data.value, repository)).get_index();
        remember(m_storage->initial_function_values, source, out);
        return out;
    }

    ygg::Index<pddl::Domain> copy(ygg::Index<pddl::Domain> source, const pddl::Repository& repository)
    {
        ygg::Index<pddl::Domain> out;
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
        out = pddl::get_or_create<pddl::Domain>(m_storage->repository, std::move(copied)).get_index();
        remember(m_storage->domains, source, out);
        m_storage->translated_domain = out;
        return out;
    }
};

} // namespace loki::semantic::detail

#endif
