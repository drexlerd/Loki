/*
 * Copyright (C) 2024-2026 Dominik Drexler
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
        m_storage->translated_domain = view;
        return view;
    }

    formalism::TaskView copy_task(formalism::TaskView task)
    {
        const auto& source = task.get_context();
        auto data = task.get_data();
        data.index = {};
        data.domain = as_index(copy(data.domain, source));
        data.requirements = copy_list(data.requirements, source);
        data.objects = copy_list(data.objects, source);
        data.initial_literals = copy_list(data.initial_literals, source);
        data.initial_function_values = copy_list(data.initial_function_values, source);
        data.goal = copy_optional(data.goal, source);
        data.metric = copy_optional(data.metric, source);
        data.predicates = copy_list(data.predicates, source);
        data.axioms = copy_list(data.axioms, source);
        auto view = formalism::get_or_create<formalism::Task>(m_storage->repository, std::move(data));
        remember(m_storage->tasks, task.get_index(), view);
        return view;
    }

private:
    std::shared_ptr<TranslationStorage> m_storage;

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const formalism::Repository& repository)
    {
        auto result = ygg::IndexList<T> {};
        for (auto index : source)
            result.push_back(as_index(copy(index, repository)));
        return result;
    }

    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const formalism::Repository& repository)
    {
        if (!source)
            return {};
        return as_index(copy(*source, repository));
    }

    formalism::RequirementView copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->requirements, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Requirement>(m_storage->repository, repository[source].kind);
        remember(m_storage->requirements, source, out);
        return out;
    }

    formalism::TypeView copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->types, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Type>(m_storage->repository, data.name, copy_list(data.bases, repository));
        remember(m_storage->types, source, out);
        return out;
    }

    formalism::ObjectView copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->objects, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Object>(m_storage->repository, data.name, copy_list(data.types, repository));
        remember(m_storage->objects, source, out);
        return out;
    }

    formalism::VariableView copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->variables, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Variable>(m_storage->repository, repository[source].name);
        remember(m_storage->variables, source, out);
        return out;
    }

    formalism::ParameterView copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->parameters, source))
            return *mapped;
        const auto& data = repository[source];
        auto out =
            formalism::get_or_create<formalism::Parameter>(m_storage->repository, as_index(copy(data.variable, repository)), copy_list(data.types, repository));
        remember(m_storage->parameters, source, out);
        return out;
    }

    formalism::PredicateView copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->predicates, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Predicate>(m_storage->repository, data.name, copy_list(data.parameters, repository));
        remember(m_storage->predicates, source, out);
        return out;
    }

    formalism::FunctionSkeletonView copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->functions, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::FunctionSkeleton>(m_storage->repository,
                                                                         data.name,
                                                                         copy_list(data.parameters, repository),
                                                                         as_index(copy(data.type, repository)));
        remember(m_storage->functions, source, out);
        return out;
    }

    formalism::TermView copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->terms, source))
            return *mapped;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant
                                { return ygg::Data<formalism::Term>::Variant(as_index(copy(arg, repository))); },
                                repository[source].value);
        auto out = formalism::get_or_create<formalism::Term>(m_storage->repository, std::move(value));
        remember(m_storage->terms, source, out);
        return out;
    }

    formalism::AtomView copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->atoms, source))
            return *mapped;
        const auto& data = repository[source];
        auto out =
            formalism::get_or_create<formalism::Atom>(m_storage->repository, as_index(copy(data.predicate, repository)), copy_list(data.terms, repository));
        remember(m_storage->atoms, source, out);
        return out;
    }

    formalism::LiteralView copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->literals, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Literal>(m_storage->repository, as_index(copy(data.atom, repository)), data.m_polarity);
        remember(m_storage->literals, source, out);
        return out;
    }

    formalism::FunctionExpressionNumberView copy(ygg::Index<formalism::FunctionExpressionNumber> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->numbers, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::FunctionExpressionNumber>(m_storage->repository, repository[source].value);
        remember(m_storage->numbers, source, out);
        return out;
    }

    formalism::FunctionTermView copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->function_terms, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::FunctionTerm>(m_storage->repository,
                                                                     as_index(copy(data.function, repository)),
                                                                     copy_list(data.terms, repository));
        remember(m_storage->function_terms, source, out);
        return out;
    }

    formalism::UnaryFunctionExpressionView copy(ygg::Index<formalism::UnaryFunctionExpression> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->unary_expressions, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::UnaryFunctionExpression>(m_storage->repository, data.op, as_index(copy(data.expression, repository)));
        remember(m_storage->unary_expressions, source, out);
        return out;
    }

    formalism::BinaryFunctionExpressionView copy(ygg::Index<formalism::BinaryFunctionExpression> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->binary_expressions, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::BinaryFunctionExpression>(m_storage->repository,
                                                                                 data.op,
                                                                                 as_index(copy(data.left, repository)),
                                                                                 as_index(copy(data.right, repository)));
        remember(m_storage->binary_expressions, source, out);
        return out;
    }

    formalism::MultiFunctionExpressionView copy(ygg::Index<formalism::MultiFunctionExpression> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->multi_expressions, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::MultiFunctionExpression>(m_storage->repository, data.op, copy_list(data.expressions, repository));
        remember(m_storage->multi_expressions, source, out);
        return out;
    }

    formalism::FunctionExpressionView copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->function_expressions, source))
            return *mapped;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant
                                { return ygg::Data<formalism::FunctionExpression>::Variant(as_index(copy(arg, repository))); },
                                repository[source].value);
        auto out = formalism::get_or_create<formalism::FunctionExpression>(m_storage->repository, std::move(value));
        remember(m_storage->function_expressions, source, out);
        return out;
    }

    formalism::ConditionLiteralView copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_literals, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionLiteral>(m_storage->repository, as_index(copy(repository[source].literal, repository)));
        remember(m_storage->condition_literals, source, out);
        return out;
    }

    formalism::ConditionAndView copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_ands, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionAnd>(m_storage->repository, copy_list(repository[source].conditions, repository));
        remember(m_storage->condition_ands, source, out);
        return out;
    }

    formalism::ConditionOrView copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_ors, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionOr>(m_storage->repository, copy_list(repository[source].conditions, repository));
        remember(m_storage->condition_ors, source, out);
        return out;
    }

    formalism::ConditionNotView copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_nots, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionNot>(m_storage->repository, as_index(copy(repository[source].condition, repository)));
        remember(m_storage->condition_nots, source, out);
        return out;
    }

    formalism::ConditionImplyView copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_implies, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::ConditionImply>(m_storage->repository,
                                                                       as_index(copy(data.left, repository)),
                                                                       as_index(copy(data.right, repository)));
        remember(m_storage->condition_implies, source, out);
        return out;
    }

    formalism::ConditionExistsView copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_exists, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::ConditionExists>(m_storage->repository,
                                                                        copy_list(data.parameters, repository),
                                                                        as_index(copy(data.condition, repository)));
        remember(m_storage->condition_exists, source, out);
        return out;
    }

    formalism::ConditionForallView copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_foralls, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::ConditionForall>(m_storage->repository,
                                                                        copy_list(data.parameters, repository),
                                                                        as_index(copy(data.condition, repository)));
        remember(m_storage->condition_foralls, source, out);
        return out;
    }

    formalism::ConditionNumericConstraintView copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->condition_numeric_constraints, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::ConditionNumericConstraint>(m_storage->repository,
                                                                                   data.comparator,
                                                                                   as_index(copy(data.left, repository)),
                                                                                   as_index(copy(data.right, repository)));
        remember(m_storage->condition_numeric_constraints, source, out);
        return out;
    }

    formalism::ConditionView copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->conditions, source))
            return *mapped;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Condition>::Variant
                                { return ygg::Data<formalism::Condition>::Variant(as_index(copy(arg, repository))); },
                                repository[source].value);
        auto out = formalism::get_or_create<formalism::Condition>(m_storage->repository, std::move(value));
        remember(m_storage->conditions, source, out);
        return out;
    }

    formalism::EffectLiteralView copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_literals, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectLiteral>(m_storage->repository, as_index(copy(repository[source].literal, repository)));
        remember(m_storage->effect_literals, source, out);
        return out;
    }

    formalism::EffectAndView copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_ands, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectAnd>(m_storage->repository, copy_list(repository[source].effects, repository));
        remember(m_storage->effect_ands, source, out);
        return out;
    }

    formalism::EffectNumericView copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_numerics, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::EffectNumeric>(m_storage->repository,
                                                                      data.op,
                                                                      as_index(copy(data.function, repository)),
                                                                      copy_list(data.terms, repository),
                                                                      as_index(copy(data.expression, repository)));
        remember(m_storage->effect_numerics, source, out);
        return out;
    }

    formalism::EffectForallView copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_foralls, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::EffectForall>(m_storage->repository,
                                                                     copy_list(data.parameters, repository),
                                                                     as_index(copy(data.effect, repository)));
        remember(m_storage->effect_foralls, source, out);
        return out;
    }

    formalism::EffectWhenView copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_whens, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::EffectWhen>(m_storage->repository,
                                                                   as_index(copy(data.condition, repository)),
                                                                   as_index(copy(data.effect, repository)));
        remember(m_storage->effect_whens, source, out);
        return out;
    }

    formalism::EffectOneOfView copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_one_ofs, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectOneOf>(m_storage->repository, copy_list(repository[source].effects, repository));
        remember(m_storage->effect_one_ofs, source, out);
        return out;
    }

    formalism::EffectProbabilisticAlternativeView copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_probabilistic_alternatives, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::EffectProbabilisticAlternative>(m_storage->repository,
                                                                                       data.probability,
                                                                                       as_index(copy(data.effect, repository)));
        remember(m_storage->effect_probabilistic_alternatives, source, out);
        return out;
    }

    formalism::EffectProbabilisticView copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effect_probabilistics, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectProbabilistic>(m_storage->repository, copy_list(repository[source].alternatives, repository));
        remember(m_storage->effect_probabilistics, source, out);
        return out;
    }

    formalism::EffectView copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->effects, source))
            return *mapped;
        auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant
                                { return ygg::Data<formalism::Effect>::Variant(as_index(copy(arg, repository))); },
                                repository[source].value);
        auto out = formalism::get_or_create<formalism::Effect>(m_storage->repository, std::move(value));
        remember(m_storage->effects, source, out);
        return out;
    }

    formalism::ActionView copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->actions, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Action>(m_storage->repository,
                                                               data.name,
                                                               copy_list(data.parameters, repository),
                                                               data.original_arity,
                                                               copy_optional(data.precondition, repository),
                                                               copy_optional(data.effect, repository));
        remember(m_storage->actions, source, out);
        return out;
    }

    formalism::AxiomView copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->axioms, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Axiom>(m_storage->repository,
                                                              copy_list(data.parameters, repository),
                                                              data.original_arity,
                                                              as_index(copy(data.head, repository)),
                                                              as_index(copy(data.condition, repository)));
        remember(m_storage->axioms, source, out);
        return out;
    }

    formalism::MetricView copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->metrics, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::Metric>(m_storage->repository, data.minimize, as_index(copy(data.expression, repository)));
        remember(m_storage->metrics, source, out);
        return out;
    }

    formalism::InitialFunctionValueView copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->initial_function_values, source))
            return *mapped;
        const auto& data = repository[source];
        auto out = formalism::get_or_create<formalism::InitialFunctionValue>(m_storage->repository,
                                                                             as_index(copy(data.function, repository)),
                                                                             as_index(copy(data.value, repository)));
        remember(m_storage->initial_function_values, source, out);
        return out;
    }

    formalism::DomainView copy(ygg::Index<formalism::Domain> source, const formalism::Repository& repository)
    {
        if (auto mapped = find_mapped(m_storage->domains, source))
            return *mapped;
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
        auto out = formalism::get_or_create<formalism::Domain>(m_storage->repository, std::move(copied));
        remember(m_storage->domains, source, out);
        m_storage->translated_domain = out;
        return out;
    }
};

}  // namespace loki::semantic::detail

#endif
