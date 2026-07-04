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
        auto data = domain.get_data();
        data.index = {};
        data.requirements = copy_list(domain.get_requirements());
        data.types = copy_list(domain.get_types());
        data.constants = copy_list(domain.get_constants());
        data.predicates = copy_list(domain.get_predicates());
        data.functions = copy_list(domain.get_functions());
        data.actions = copy_list(domain.get_actions());
        data.axioms = copy_list(domain.get_axioms());
        auto view = formalism::get_or_create<formalism::Domain>(m_storage->repository, std::move(data));
        m_storage->translated_domain = view;
        remember(m_storage->domains, domain, view);
        return view;
    }

    formalism::TaskView copy_task(formalism::TaskView task)
    {
        auto data = task.get_data();
        data.index = {};
        data.domain = m_storage->translated_domain ? m_storage->translated_domain->get_index() : as_index(copy_domain(task.get_domain()));
        data.requirements = copy_list(task.get_requirements());
        data.objects = copy_list(task.get_objects());
        data.initial_literals = copy_list(task.get_initial_literals());
        data.initial_function_values = copy_list(task.get_initial_function_values());
        if (const auto goal = task.get_goal())
            data.goal = as_index(copy(goal.value()));
        else
            data.goal = {};
        if (const auto metric = task.get_metric())
            data.metric = as_index(copy(metric.value()));
        else
            data.metric = {};
        data.predicates = copy_list(task.get_predicates());
        data.axioms = copy_list(task.get_axioms());
        auto view = formalism::get_or_create<formalism::Task>(m_storage->repository, std::move(data));
        remember(m_storage->tasks, task, view);
        return view;
    }

private:
    std::shared_ptr<TranslationStorage> m_storage;

    template<typename T>
    ygg::IndexList<T> copy_list(formalism::EntityListView<T> source)
    {
        auto result = ygg::IndexList<T> {};
        for (auto view : source)
            result.push_back(as_index(copy(view)));
        return result;
    }

    formalism::RequirementView copy(formalism::RequirementView source)
    {
        if (auto mapped = find_mapped(m_storage->requirements, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Requirement>(m_storage->repository, source.get_kind());
        remember(m_storage->requirements, source, out);
        return out;
    }

    formalism::TypeView copy(formalism::TypeView source)
    {
        if (auto mapped = find_mapped(m_storage->types, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Type>(m_storage->repository, source.get_name(), copy_list(source.get_bases()));
        remember(m_storage->types, source, out);
        return out;
    }

    formalism::ObjectView copy(formalism::ObjectView source)
    {
        if (auto mapped = find_mapped(m_storage->objects, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Object>(m_storage->repository, source.get_name(), copy_list(source.get_types()));
        remember(m_storage->objects, source, out);
        return out;
    }

    formalism::VariableView copy(formalism::VariableView source)
    {
        if (auto mapped = find_mapped(m_storage->variables, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Variable>(m_storage->repository, source.get_name());
        remember(m_storage->variables, source, out);
        return out;
    }

    formalism::ParameterView copy(formalism::ParameterView source)
    {
        if (auto mapped = find_mapped(m_storage->parameters, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Parameter>(m_storage->repository, as_index(copy(source.get_variable())), copy_list(source.get_types()));
        remember(m_storage->parameters, source, out);
        return out;
    }

    formalism::PredicateView copy(formalism::PredicateView source)
    {
        if (auto mapped = find_mapped(m_storage->predicates, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Predicate>(m_storage->repository, source.get_name(), copy_list(source.get_parameters()));
        remember(m_storage->predicates, source, out);
        return out;
    }

    formalism::FunctionSkeletonView copy(formalism::FunctionSkeletonView source)
    {
        if (auto mapped = find_mapped(m_storage->functions, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::FunctionSkeleton>(m_storage->repository,
                                                                         source.get_name(),
                                                                         copy_list(source.get_parameters()),
                                                                         as_index(copy(source.get_type())));
        remember(m_storage->functions, source, out);
        return out;
    }

    formalism::TermView copy(formalism::TermView source)
    {
        if (auto mapped = find_mapped(m_storage->terms, source))
            return *mapped;
        auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant { return as_index(copy(arg)); }, source.get_value());
        auto out = formalism::get_or_create<formalism::Term>(m_storage->repository, std::move(value));
        remember(m_storage->terms, source, out);
        return out;
    }

    formalism::AtomView copy(formalism::AtomView source)
    {
        if (auto mapped = find_mapped(m_storage->atoms, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Atom>(m_storage->repository, as_index(copy(source.get_predicate())), copy_list(source.get_terms()));
        remember(m_storage->atoms, source, out);
        return out;
    }

    formalism::LiteralView copy(formalism::LiteralView source)
    {
        if (auto mapped = find_mapped(m_storage->literals, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Literal>(m_storage->repository, as_index(copy(source.get_atom())), source.get_polarity());
        remember(m_storage->literals, source, out);
        return out;
    }

    formalism::FunctionExpressionNumberView copy(formalism::FunctionExpressionNumberView source)
    {
        if (auto mapped = find_mapped(m_storage->numbers, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::FunctionExpressionNumber>(m_storage->repository, source.get_value());
        remember(m_storage->numbers, source, out);
        return out;
    }

    formalism::FunctionTermView copy(formalism::FunctionTermView source)
    {
        if (auto mapped = find_mapped(m_storage->function_terms, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::FunctionTerm>(m_storage->repository, as_index(copy(source.get_function())), copy_list(source.get_terms()));
        remember(m_storage->function_terms, source, out);
        return out;
    }

    formalism::UnaryFunctionExpressionView copy(formalism::UnaryFunctionExpressionView source)
    {
        if (auto mapped = find_mapped(m_storage->unary_expressions, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::UnaryFunctionExpression>(m_storage->repository, source.get_data().op, as_index(copy(source.get_expression())));
        remember(m_storage->unary_expressions, source, out);
        return out;
    }

    formalism::BinaryFunctionExpressionView copy(formalism::BinaryFunctionExpressionView source)
    {
        if (auto mapped = find_mapped(m_storage->binary_expressions, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::BinaryFunctionExpression>(m_storage->repository,
                                                                                 source.get_data().op,
                                                                                 as_index(copy(source.get_left())),
                                                                                 as_index(copy(source.get_right())));
        remember(m_storage->binary_expressions, source, out);
        return out;
    }

    formalism::MultiFunctionExpressionView copy(formalism::MultiFunctionExpressionView source)
    {
        if (auto mapped = find_mapped(m_storage->multi_expressions, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::MultiFunctionExpression>(m_storage->repository, source.get_data().op, copy_list(source.get_expressions()));
        remember(m_storage->multi_expressions, source, out);
        return out;
    }

    formalism::FunctionExpressionView copy(formalism::FunctionExpressionView source)
    {
        if (auto mapped = find_mapped(m_storage->function_expressions, source))
            return *mapped;
        auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant { return as_index(copy(arg)); }, source.get_value());
        auto out = formalism::get_or_create<formalism::FunctionExpression>(m_storage->repository, std::move(value));
        remember(m_storage->function_expressions, source, out);
        return out;
    }

    formalism::ConditionLiteralView copy(formalism::ConditionLiteralView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_literals, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionLiteral>(m_storage->repository, as_index(copy(source.get_literal())));
        remember(m_storage->condition_literals, source, out);
        return out;
    }

    formalism::ConditionAndView copy(formalism::ConditionAndView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_ands, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionAnd>(m_storage->repository, copy_list(source.get_conditions()));
        remember(m_storage->condition_ands, source, out);
        return out;
    }

    formalism::ConditionOrView copy(formalism::ConditionOrView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_ors, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionOr>(m_storage->repository, copy_list(source.get_conditions()));
        remember(m_storage->condition_ors, source, out);
        return out;
    }

    formalism::ConditionNotView copy(formalism::ConditionNotView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_nots, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionNot>(m_storage->repository, as_index(copy(source.get_condition())));
        remember(m_storage->condition_nots, source, out);
        return out;
    }

    formalism::ConditionImplyView copy(formalism::ConditionImplyView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_implies, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::ConditionImply>(m_storage->repository, as_index(copy(source.get_left())), as_index(copy(source.get_right())));
        remember(m_storage->condition_implies, source, out);
        return out;
    }

    formalism::ConditionExistsView copy(formalism::ConditionExistsView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_exists, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionExists>(m_storage->repository,
                                                                        copy_list(source.get_parameters()),
                                                                        as_index(copy(source.get_condition())));
        remember(m_storage->condition_exists, source, out);
        return out;
    }

    formalism::ConditionForallView copy(formalism::ConditionForallView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_foralls, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionForall>(m_storage->repository,
                                                                        copy_list(source.get_parameters()),
                                                                        as_index(copy(source.get_condition())));
        remember(m_storage->condition_foralls, source, out);
        return out;
    }

    formalism::ConditionNumericConstraintView copy(formalism::ConditionNumericConstraintView source)
    {
        if (auto mapped = find_mapped(m_storage->condition_numeric_constraints, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::ConditionNumericConstraint>(m_storage->repository,
                                                                                   source.get_data().comparator,
                                                                                   as_index(copy(source.get_left())),
                                                                                   as_index(copy(source.get_right())));
        remember(m_storage->condition_numeric_constraints, source, out);
        return out;
    }

    formalism::ConditionView copy(formalism::ConditionView source)
    {
        if (auto mapped = find_mapped(m_storage->conditions, source))
            return *mapped;
        auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Condition>::Variant { return as_index(copy(arg)); }, source.get_value());
        auto out = formalism::get_or_create<formalism::Condition>(m_storage->repository, std::move(value));
        remember(m_storage->conditions, source, out);
        return out;
    }

    formalism::EffectLiteralView copy(formalism::EffectLiteralView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_literals, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectLiteral>(m_storage->repository, as_index(copy(source.get_literal())));
        remember(m_storage->effect_literals, source, out);
        return out;
    }

    formalism::EffectAndView copy(formalism::EffectAndView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_ands, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectAnd>(m_storage->repository, copy_list(source.get_effects()));
        remember(m_storage->effect_ands, source, out);
        return out;
    }

    formalism::EffectNumericView copy(formalism::EffectNumericView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_numerics, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectNumeric>(m_storage->repository,
                                                                      source.get_data().op,
                                                                      as_index(copy(source.get_function())),
                                                                      copy_list(source.get_terms()),
                                                                      as_index(copy(source.get_expression())));
        remember(m_storage->effect_numerics, source, out);
        return out;
    }

    formalism::EffectForallView copy(formalism::EffectForallView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_foralls, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::EffectForall>(m_storage->repository, copy_list(source.get_parameters()), as_index(copy(source.get_effect())));
        remember(m_storage->effect_foralls, source, out);
        return out;
    }

    formalism::EffectWhenView copy(formalism::EffectWhenView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_whens, source))
            return *mapped;
        auto out =
            formalism::get_or_create<formalism::EffectWhen>(m_storage->repository, as_index(copy(source.get_condition())), as_index(copy(source.get_effect())));
        remember(m_storage->effect_whens, source, out);
        return out;
    }

    formalism::EffectOneOfView copy(formalism::EffectOneOfView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_one_ofs, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectOneOf>(m_storage->repository, copy_list(source.get_effects()));
        remember(m_storage->effect_one_ofs, source, out);
        return out;
    }

    formalism::EffectProbabilisticAlternativeView copy(formalism::EffectProbabilisticAlternativeView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_probabilistic_alternatives, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectProbabilisticAlternative>(m_storage->repository,
                                                                                       source.get_data().probability,
                                                                                       as_index(copy(source.get_effect())));
        remember(m_storage->effect_probabilistic_alternatives, source, out);
        return out;
    }

    formalism::EffectProbabilisticView copy(formalism::EffectProbabilisticView source)
    {
        if (auto mapped = find_mapped(m_storage->effect_probabilistics, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::EffectProbabilistic>(m_storage->repository, copy_list(source.get_alternatives()));
        remember(m_storage->effect_probabilistics, source, out);
        return out;
    }

    formalism::EffectView copy(formalism::EffectView source)
    {
        if (auto mapped = find_mapped(m_storage->effects, source))
            return *mapped;
        auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return as_index(copy(arg)); }, source.get_value());
        auto out = formalism::get_or_create<formalism::Effect>(m_storage->repository, std::move(value));
        remember(m_storage->effects, source, out);
        return out;
    }

    formalism::ActionView copy(formalism::ActionView source)
    {
        if (auto mapped = find_mapped(m_storage->actions, source))
            return *mapped;
        const auto& data = source.get_data();
        auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
        if (const auto condition = source.get_precondition())
            precondition = as_index(copy(condition.value()));
        auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
        if (const auto effect_view = source.get_effect())
            effect = as_index(copy(effect_view.value()));
        auto out = formalism::get_or_create<formalism::Action>(m_storage->repository,
                                                               data.name,
                                                               data.original_name,
                                                               copy_list(source.get_parameters()),
                                                               data.original_arity,
                                                               precondition,
                                                               effect);
        remember(m_storage->actions, source, out);
        return out;
    }

    formalism::AxiomView copy(formalism::AxiomView source)
    {
        if (auto mapped = find_mapped(m_storage->axioms, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Axiom>(m_storage->repository,
                                                              copy_list(source.get_parameters()),
                                                              source.get_data().original_arity,
                                                              as_index(copy(source.get_head())),
                                                              as_index(copy(source.get_condition())));
        remember(m_storage->axioms, source, out);
        return out;
    }

    formalism::MetricView copy(formalism::MetricView source)
    {
        if (auto mapped = find_mapped(m_storage->metrics, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::Metric>(m_storage->repository, source.is_minimize(), as_index(copy(source.get_expression())));
        remember(m_storage->metrics, source, out);
        return out;
    }

    formalism::InitialFunctionValueView copy(formalism::InitialFunctionValueView source)
    {
        if (auto mapped = find_mapped(m_storage->initial_function_values, source))
            return *mapped;
        auto out = formalism::get_or_create<formalism::InitialFunctionValue>(m_storage->repository,
                                                                             as_index(copy(source.get_function())),
                                                                             as_index(copy(source.get_value())));
        remember(m_storage->initial_function_values, source, out);
        return out;
    }
};

}  // namespace loki::semantic::detail

#endif
