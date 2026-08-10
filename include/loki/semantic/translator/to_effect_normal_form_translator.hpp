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

#ifndef LOKI_SEMANTIC_TRANSLATOR_TO_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_TO_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <algorithm>
#include <optional>

namespace loki::semantic::detail
{

template<typename Derived>
class ToEffectNormalFormTranslator : public CopyTranslatorComponent<Derived, ToEffectNormalFormTranslator<Derived>>
{
public:
    explicit ToEffectNormalFormTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ToEffectNormalFormTranslator<Derived>>(context) {}

    formalism::EffectView wrap_effect(ygg::Data<formalism::Effect>::Variant value);
    template<typename T>
    formalism::EffectView wrap_effect(formalism::EntityView<T> value);
    template<typename T>
    std::optional<formalism::EntityView<T>> as_effect(formalism::EffectView effect) const;
    formalism::EffectView normalize_effect(formalism::EffectView effect);
    template<typename T>
    formalism::EffectView normalize_effect_node(formalism::EffectView effect, formalism::EntityView<T>);
    formalism::EffectView normalize_effect_node(formalism::EffectView, formalism::EffectAndView node);
    formalism::EffectView normalize_effect_node(formalism::EffectView, formalism::EffectForallView node);
    formalism::EffectView normalize_effect_node(formalism::EffectView, formalism::EffectWhenView node);
};

template<typename Derived>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::wrap_effect(ygg::Data<formalism::Effect>::Variant value)
{
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
template<typename T>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::wrap_effect(formalism::EntityView<T> value)
{
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository,
                                                       typename ygg::Data<formalism::Effect>::template ViewVariant<formalism::Repository>(value));
}

template<typename Derived>
template<typename T>
std::optional<formalism::EntityView<T>> ToEffectNormalFormTranslator<Derived>::as_effect(formalism::EffectView effect) const
{
    auto result = std::optional<formalism::EntityView<T>> {};
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::EntityView<T>>)
                result = node;
        },
        effect.get_value());
    return result;
}

template<typename Derived>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::normalize_effect(formalism::EffectView effect)
{
    return ygg::visit([&](const auto& node) { return this->self().normalize_effect_node(effect, node); }, effect.get_value());
}

template<typename Derived>
template<typename T>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView effect, formalism::EntityView<T>)
{
    return effect;
}

template<typename Derived>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, formalism::EffectAndView node)
{
    struct NumericGroup
    {
        formalism::NumericEffectOperator op {};
        ygg::Index<formalism::FunctionSkeleton> function;
        ygg::IndexList<formalism::Term> terms;
        ygg::IndexList<formalism::FunctionExpression> expressions;
    };

    auto effects = std::vector<formalism::EffectView> {};
    auto numeric_groups = std::vector<NumericGroup> {};

    auto push_effect = [&](formalism::EffectView effect)
    {
        if (const auto numeric = this->self().template as_effect<formalism::EffectNumeric>(effect))
        {
            const auto& data = numeric->get_data();
            auto it =
                std::find_if(numeric_groups.begin(),
                             numeric_groups.end(),
                             [&](const NumericGroup& group) { return group.op == data.op && group.function == data.function && group.terms == data.terms; });
            if (it == numeric_groups.end())
            {
                auto group = NumericGroup {};
                group.op = data.op;
                group.function = data.function;
                group.terms = data.terms;
                group.expressions.push_back(data.expression);
                numeric_groups.push_back(std::move(group));
            }
            else
            {
                it->expressions.push_back(data.expression);
            }
            return;
        }
        effects.push_back(effect);
    };

    for (auto child : node.get_effects())
    {
        const auto normalized = this->self().normalize_effect(child);
        if (const auto nested = this->self().template as_effect<formalism::EffectAnd>(normalized))
        {
            for (auto part : nested->get_effects())
                push_effect(part);
        }
        else
        {
            push_effect(normalized);
        }
    }

    for (const auto& group : numeric_groups)
    {
        // A single expression stays as-is; only genuine aggregations get a sum, keeping
        // normalization idempotent across repeated translations.
        auto sum = group.expressions.front();
        if (group.expressions.size() > 1)
        {
            auto remaining = ygg::IndexList<formalism::FunctionExpression> {};
            for (std::size_t i = 2; i < group.expressions.size(); ++i)
                remaining.push_back(group.expressions[i]);
            sum = formalism::get_or_create<formalism::FunctionExpression>(
                      this->m_storage->repository,
                      ygg::Data<formalism::FunctionExpression>::Variant(
                          formalism::get_or_create<formalism::MultiFunctionExpression>(this->m_storage->repository,
                                                                                       formalism::MultiArithmeticOperator::Add,
                                                                                       group.expressions[0],
                                                                                       group.expressions[1],
                                                                                       std::move(remaining))
                              .get_index()))
                      .get_index();
        }
        effects.push_back(this->self().wrap_effect(
            formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository, group.op, group.function, group.terms, sum)));
    }

    return this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
}

template<typename Derived>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, formalism::EffectForallView node)
{
    const auto& data = node.get_data();
    const auto nested = this->self().normalize_effect(node.get_effect());
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : nested_and->get_effects())
        {
            const auto wrapped =
                this->self().wrap_effect(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, part.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        auto parameters = data.parameters;
        for (auto parameter : nested_forall->get_parameters())
            parameters.push_back(parameter.get_index());
        const auto wrapped = this->self().wrap_effect(
            formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), nested_forall->get_effect().get_index()));
        return this->self().normalize_effect(wrapped);
    }
    return this->self().wrap_effect(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, nested.get_index()));
}

template<typename Derived>
formalism::EffectView ToEffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, formalism::EffectWhenView node)
{
    const auto moved_condition = this->self().move_existentials(node.get_condition());
    auto condition = this->self().flatten_condition(this->self().to_dnf(moved_condition));
    const auto nested = this->self().normalize_effect(node.get_effect());

    if (const auto condition_or = this->self().as_or(condition))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : condition_or->get_conditions())
        {
            const auto wrapped =
                this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, part.get_index(), nested.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }

    if (const auto nested_when = this->self().template as_effect<formalism::EffectWhen>(nested))
    {
        auto conditions = ygg::IndexList<formalism::Condition> {};
        conditions.push_back(condition.get_index());
        conditions.push_back(nested_when->get_condition().get_index());
        const auto combined_condition = as_index(this->self().make_conjunction(std::move(conditions)));
        const auto wrapped = this->self().wrap_effect(
            formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, combined_condition, nested_when->get_effect().get_index()));
        return this->self().normalize_effect(wrapped);
    }
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : nested_and->get_effects())
        {
            const auto wrapped =
                this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), part.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        auto parameters = ygg::IndexList<formalism::Parameter> {};
        for (auto parameter : nested_forall->get_parameters())
            parameters.push_back(parameter.get_index());
        const auto pushed_when = this->self()
                                     .wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository,
                                                                                                  condition.get_index(),
                                                                                                  nested_forall->get_effect().get_index()))
                                     .get_index();
        const auto wrapped =
            this->self().wrap_effect(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), pushed_when));
        return this->self().normalize_effect(wrapped);
    }
    if (const auto exists = this->self().as_exists(condition))
    {
        auto parameters = ygg::IndexList<formalism::Parameter> {};
        for (auto parameter : exists->get_parameters())
            parameters.push_back(parameter.get_index());
        const auto pushed_when =
            this->self()
                .wrap_effect(
                    formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, exists->get_condition().get_index(), nested.get_index()))
                .get_index();
        const auto wrapped =
            this->self().wrap_effect(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), pushed_when));
        return this->self().normalize_effect(wrapped);
    }

    return this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), nested.get_index()));
}

}  // namespace loki::semantic::detail

#endif
