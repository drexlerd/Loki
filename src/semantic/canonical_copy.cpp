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

formalism::TaskView CanonicalCopyTranslator::copy_task(formalism::TaskView task)
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

formalism::EffectLiteralView CanonicalCopyTranslator::copy(formalism::EffectLiteralView source)
{
    if (auto mapped = find_mapped(m_storage->effect_literals, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::EffectLiteral>(m_storage->repository, as_index(copy(source.get_literal())));
    remember(m_storage->effect_literals, source, out);
    return out;
}

formalism::EffectAndView CanonicalCopyTranslator::copy(formalism::EffectAndView source)
{
    if (auto mapped = find_mapped(m_storage->effect_ands, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::EffectAnd>(m_storage->repository, copy_list(source.get_effects()));
    remember(m_storage->effect_ands, source, out);
    return out;
}

formalism::EffectNumericView CanonicalCopyTranslator::copy(formalism::EffectNumericView source)
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

formalism::EffectForallView CanonicalCopyTranslator::copy(formalism::EffectForallView source)
{
    if (auto mapped = find_mapped(m_storage->effect_foralls, source))
        return *mapped;
    auto out =
        formalism::get_or_create<formalism::EffectForall>(m_storage->repository, copy_list(source.get_parameters()), as_index(copy(source.get_effect())));
    remember(m_storage->effect_foralls, source, out);
    return out;
}

formalism::EffectWhenView CanonicalCopyTranslator::copy(formalism::EffectWhenView source)
{
    if (auto mapped = find_mapped(m_storage->effect_whens, source))
        return *mapped;
    auto out =
        formalism::get_or_create<formalism::EffectWhen>(m_storage->repository, as_index(copy(source.get_condition())), as_index(copy(source.get_effect())));
    remember(m_storage->effect_whens, source, out);
    return out;
}

formalism::EffectOneOfView CanonicalCopyTranslator::copy(formalism::EffectOneOfView source)
{
    if (auto mapped = find_mapped(m_storage->effect_one_ofs, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::EffectOneOf>(m_storage->repository, copy_list(source.get_effects()));
    remember(m_storage->effect_one_ofs, source, out);
    return out;
}

formalism::EffectProbabilisticAlternativeView CanonicalCopyTranslator::copy(formalism::EffectProbabilisticAlternativeView source)
{
    if (auto mapped = find_mapped(m_storage->effect_probabilistic_alternatives, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::EffectProbabilisticAlternative>(m_storage->repository,
                                                                                   source.get_data().probability,
                                                                                   as_index(copy(source.get_effect())));
    remember(m_storage->effect_probabilistic_alternatives, source, out);
    return out;
}

formalism::EffectProbabilisticView CanonicalCopyTranslator::copy(formalism::EffectProbabilisticView source)
{
    if (auto mapped = find_mapped(m_storage->effect_probabilistics, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::EffectProbabilistic>(m_storage->repository, copy_list(source.get_alternatives()));
    remember(m_storage->effect_probabilistics, source, out);
    return out;
}

formalism::EffectView CanonicalCopyTranslator::copy(formalism::EffectView source)
{
    if (auto mapped = find_mapped(m_storage->effects, source))
        return *mapped;
    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return as_index(copy(arg)); }, source.get_value());
    auto out = formalism::get_or_create<formalism::Effect>(m_storage->repository, std::move(value));
    remember(m_storage->effects, source, out);
    return out;
}

formalism::ActionView CanonicalCopyTranslator::copy(formalism::ActionView source)
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

formalism::AxiomView CanonicalCopyTranslator::copy(formalism::AxiomView source)
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

formalism::MetricView CanonicalCopyTranslator::copy(formalism::MetricView source)
{
    if (auto mapped = find_mapped(m_storage->metrics, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::Metric>(
        m_storage->repository,
        source.get_optimization_direction(),
        as_index(copy(source.get_expression())));
    remember(m_storage->metrics, source, out);
    return out;
}

formalism::InitialFunctionValueView CanonicalCopyTranslator::copy(formalism::InitialFunctionValueView source)
{
    if (auto mapped = find_mapped(m_storage->initial_function_values, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::InitialFunctionValue>(m_storage->repository,
                                                                         as_index(copy(source.get_function())),
                                                                         as_index(copy(source.get_value())));
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
