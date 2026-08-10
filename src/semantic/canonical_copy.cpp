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

#include "loki/formalism/builder.hpp"
#include "loki/semantic/translator/canonical_copy_translator.hpp"
#include "loki/semantic/translator/common.hpp"

#include <utility>

namespace loki::semantic::detail
{

CanonicalCopyTranslator::CanonicalCopyTranslator(std::shared_ptr<TranslationStorage> storage) : m_storage(std::move(storage)) {}

formalism::DomainView CanonicalCopyTranslator::copy_domain(formalism::DomainView domain)
{
    auto data = checkout<formalism::Domain>();
    data->name = domain.get_data().name;
    copy_list(domain.get_requirements(), data->requirements);
    copy_list(domain.get_types(), data->types);
    copy_list(domain.get_constants(), data->constants);
    copy_list(domain.get_predicates(), data->predicates);
    copy_list(domain.get_functions(), data->functions);
    copy_list(domain.get_actions(), data->actions);
    copy_list(domain.get_axioms(), data->axioms);
    auto view = formalism::get_or_create(m_storage->repository, *data).first;
    m_storage->translated_domain = view;
    remember(m_storage->domains, domain, view);
    return view;
}

formalism::TaskView CanonicalCopyTranslator::copy_task(formalism::TaskView task)
{
    auto data = checkout<formalism::Task>();
    data->name = task.get_data().name;
    data->domain = m_storage->translated_domain ? m_storage->translated_domain->get_index() : as_index(copy_domain(task.get_domain()));
    copy_list(task.get_requirements(), data->requirements);
    copy_list(task.get_objects(), data->objects);
    copy_list(task.get_initial_literals(), data->initial_literals);
    copy_list(task.get_initial_function_values(), data->initial_function_values);
    if (const auto source_goal = task.get_goal())
        data->goal = as_index(copy(source_goal.value()));
    if (const auto source_metric = task.get_metric())
        data->metric = as_index(copy(source_metric.value()));
    copy_list(task.get_predicates(), data->predicates);
    copy_list(task.get_axioms(), data->axioms);
    auto view = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->tasks, task, view);
    return view;
}

formalism::EffectLiteralView CanonicalCopyTranslator::copy(formalism::EffectLiteralView source)
{
    if (auto mapped = find_mapped(m_storage->effect_literals, source))
        return *mapped;
    auto data = checkout<formalism::EffectLiteral>();
    data->literal = as_index(copy(source.get_literal()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_literals, source, out);
    return out;
}

formalism::EffectAndView CanonicalCopyTranslator::copy(formalism::EffectAndView source)
{
    if (auto mapped = find_mapped(m_storage->effect_ands, source))
        return *mapped;
    auto data = checkout<formalism::EffectAnd>();
    copy_list(source.get_effects(), data->effects);
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_ands, source, out);
    return out;
}

formalism::EffectNumericView CanonicalCopyTranslator::copy(formalism::EffectNumericView source)
{
    if (auto mapped = find_mapped(m_storage->effect_numerics, source))
        return *mapped;
    auto data = checkout<formalism::EffectNumeric>();
    data->op = source.get_data().op;
    data->function = as_index(copy(source.get_function()));
    data->expression = as_index(copy(source.get_expression()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_numerics, source, out);
    return out;
}

formalism::EffectForallView CanonicalCopyTranslator::copy(formalism::EffectForallView source)
{
    if (auto mapped = find_mapped(m_storage->effect_foralls, source))
        return *mapped;
    auto data = checkout<formalism::EffectForall>();
    copy_list(source.get_parameters(), data->parameters);
    data->effect = as_index(copy(source.get_effect()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_foralls, source, out);
    return out;
}

formalism::EffectWhenView CanonicalCopyTranslator::copy(formalism::EffectWhenView source)
{
    if (auto mapped = find_mapped(m_storage->effect_whens, source))
        return *mapped;
    auto data = checkout<formalism::EffectWhen>();
    data->condition = as_index(copy(source.get_condition()));
    data->effect = as_index(copy(source.get_effect()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_whens, source, out);
    return out;
}

formalism::EffectOneOfView CanonicalCopyTranslator::copy(formalism::EffectOneOfView source)
{
    if (auto mapped = find_mapped(m_storage->effect_one_ofs, source))
        return *mapped;
    auto data = checkout<formalism::EffectOneOf>();
    copy_list(source.get_effects(), data->effects);
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_one_ofs, source, out);
    return out;
}

formalism::EffectProbabilisticAlternativeView CanonicalCopyTranslator::copy(formalism::EffectProbabilisticAlternativeView source)
{
    if (auto mapped = find_mapped(m_storage->effect_probabilistic_alternatives, source))
        return *mapped;
    auto data = checkout<formalism::EffectProbabilisticAlternative>();
    data->probability = source.get_data().probability;
    data->effect = as_index(copy(source.get_effect()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_probabilistic_alternatives, source, out);
    return out;
}

formalism::EffectProbabilisticView CanonicalCopyTranslator::copy(formalism::EffectProbabilisticView source)
{
    if (auto mapped = find_mapped(m_storage->effect_probabilistics, source))
        return *mapped;
    auto data = checkout<formalism::EffectProbabilistic>();
    copy_list(source.get_alternatives(), data->alternatives);
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effect_probabilistics, source, out);
    return out;
}

formalism::EffectView CanonicalCopyTranslator::copy(formalism::EffectView source)
{
    if (auto mapped = find_mapped(m_storage->effects, source))
        return *mapped;
    auto data = checkout<formalism::Effect>();
    data->value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return as_index(copy(arg)); }, source.get_value());
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->effects, source, out);
    return out;
}

formalism::ActionView CanonicalCopyTranslator::copy(formalism::ActionView source)
{
    if (auto mapped = find_mapped(m_storage->actions, source))
        return *mapped;
    const auto& data = source.get_data();
    auto out_data = checkout<formalism::Action>();
    out_data->name = data.name;
    out_data->original_name = data.original_name;
    out_data->original_arity = data.original_arity;
    if (const auto condition = source.get_precondition())
        out_data->precondition = as_index(copy(condition.value()));
    if (const auto effect_view = source.get_effect())
        out_data->effect = as_index(copy(effect_view.value()));
    copy_list(source.get_parameters(), out_data->parameters);
    auto out = formalism::get_or_create(m_storage->repository, *out_data).first;
    remember(m_storage->actions, source, out);
    return out;
}

formalism::AxiomView CanonicalCopyTranslator::copy(formalism::AxiomView source)
{
    if (auto mapped = find_mapped(m_storage->axioms, source))
        return *mapped;
    auto data = checkout<formalism::Axiom>();
    copy_list(source.get_parameters(), data->parameters);
    data->original_arity = source.get_data().original_arity;
    data->head = as_index(copy(source.get_head()));
    data->condition = as_index(copy(source.get_condition()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->axioms, source, out);
    return out;
}

formalism::MetricView CanonicalCopyTranslator::copy(formalism::MetricView source)
{
    if (auto mapped = find_mapped(m_storage->metrics, source))
        return *mapped;
    auto data = checkout<formalism::Metric>();
    data->optimization_direction = source.get_optimization_direction();
    data->expression = as_index(copy(source.get_expression()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->metrics, source, out);
    return out;
}

formalism::InitialFunctionValueView CanonicalCopyTranslator::copy(formalism::InitialFunctionValueView source)
{
    if (auto mapped = find_mapped(m_storage->initial_function_values, source))
        return *mapped;
    auto data = checkout<formalism::InitialFunctionValue>();
    data->function = as_index(copy(source.get_function()));
    data->value = as_index(copy(source.get_value()));
    auto out = formalism::get_or_create(m_storage->repository, *data).first;
    remember(m_storage->initial_function_values, source, out);
    return out;
}

formalism::DomainView canonical_copy(std::shared_ptr<TranslationStorage> storage, formalism::DomainView source)
{
    return CanonicalCopyTranslator(std::move(storage)).copy_domain(source);
}

formalism::TaskView canonical_copy(std::shared_ptr<TranslationStorage> storage, formalism::TaskView source)
{
    return CanonicalCopyTranslator(std::move(storage)).copy_task(source);
}

}  // namespace loki::semantic::detail
