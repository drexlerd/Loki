/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <algorithm>
#include <optional>

namespace loki::semantic::detail
{

template<typename Derived>
class EffectNormalFormTranslator : public CopyTranslatorComponent<Derived, EffectNormalFormTranslator<Derived>>
{
public:
    explicit EffectNormalFormTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EffectNormalFormTranslator<Derived>>(context) {}

    formalism::EffectView wrap_effect(ygg::Data<formalism::Effect>::Variant value);
    template<typename T>
    formalism::EffectView wrap_effect(formalism::EntityView<T> value);
    template<typename T>
    std::optional<formalism::EntityView<T>> as_effect(formalism::EffectView effect) const;
    template<typename T>
    std::optional<formalism::EntityView<T>> as_effect(ygg::Index<formalism::Effect> effect) const;
    formalism::EffectView normalize_effect(formalism::EffectView effect);
    formalism::EffectView normalize_effect(ygg::Index<formalism::Effect> effect);
    formalism::EffectView normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectLiteral>);
    formalism::EffectView normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectNumeric>);
    formalism::EffectView normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectOneOf>);
    formalism::EffectView normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectProbabilistic>);
    formalism::EffectView normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectAnd> node);
    formalism::EffectView normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectForall> node);
    formalism::EffectView normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectWhen> node);
};

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::wrap_effect(ygg::Data<formalism::Effect>::Variant value)
{
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
template<typename T>
formalism::EffectView EffectNormalFormTranslator<Derived>::wrap_effect(formalism::EntityView<T> value)
{
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, typename ygg::Data<formalism::Effect>::template ViewVariant<formalism::Repository>(value));
}

template<typename Derived>
template<typename T>
std::optional<formalism::EntityView<T>> EffectNormalFormTranslator<Derived>::as_effect(formalism::EffectView effect) const
{
    auto result = std::optional<formalism::EntityView<T>> {};
    ygg::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, formalism::EntityView<T>>)
            result = node;
    }, effect.get_value());
    return result;
}

template<typename Derived>
template<typename T>
std::optional<formalism::EntityView<T>> EffectNormalFormTranslator<Derived>::as_effect(ygg::Index<formalism::Effect> effect) const
{
    return as_effect<T>(ygg::make_view(effect, this->m_storage->repository));
}

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect(formalism::EffectView effect)
{
    return std::visit([&](const auto& node) { return this->self().normalize_effect_node(effect, node); }, effect.get_data().value);
}

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect(ygg::Index<formalism::Effect> effect)
{
    return normalize_effect(ygg::make_view(effect, this->m_storage->repository));
}

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectLiteral>) { return effect; }

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectNumeric>) { return effect; }

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectOneOf>) { return effect; }

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView effect, ygg::Index<formalism::EffectProbabilistic>) { return effect; }

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectAnd> node)
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
            auto it = std::find_if(numeric_groups.begin(), numeric_groups.end(), [&](const NumericGroup& group)
            {
                return group.op == data.op && group.function == data.function && group.terms == data.terms;
            });
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

    for (auto child : this->m_storage->repository[node].effects)
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
        const auto sum = formalism::get_or_create<formalism::FunctionExpression>(
            this->m_storage->repository,
            ygg::Data<formalism::FunctionExpression>::Variant(formalism::get_or_create<formalism::MultiFunctionExpression>(
                this->m_storage->repository,
                formalism::MultiArithmeticOperator::Add,
                group.expressions).get_index())).get_index();
        effects.push_back(this->self().wrap_effect(formalism::get_or_create<formalism::EffectNumeric>(
            this->m_storage->repository, group.op, group.function, group.terms, sum)));
    }

    return this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
}

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectForall> node)
{
    const auto& data = this->m_storage->repository[node];
    const auto nested = this->self().normalize_effect(data.effect);
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : nested_and->get_effects())
        {
            const auto wrapped = this->self().wrap_effect(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, part.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        auto parameters = data.parameters;
        for (auto parameter : nested_forall->get_data().parameters)
            parameters.push_back(parameter);
        const auto wrapped = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), nested_forall->get_data().effect).get_index())));
        return this->self().normalize_effect(wrapped);
    }
    return this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
        formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, nested.get_index()).get_index()));
}

template<typename Derived>
formalism::EffectView EffectNormalFormTranslator<Derived>::normalize_effect_node(formalism::EffectView, ygg::Index<formalism::EffectWhen> node)
{
    const auto& data = this->m_storage->repository[node];
    auto condition = this->self().flatten_condition(as_index(this->self().to_dnf(as_index(this->self().move_existentials(data.condition)))));
    const auto nested = this->self().normalize_effect(data.effect);

    if (const auto condition_or = this->self().as_or(condition))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : condition_or->get_conditions())
        {
            const auto wrapped = this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, part.get_index(), nested.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }

    if (const auto nested_when = this->self().template as_effect<formalism::EffectWhen>(nested))
    {
        auto conditions = ygg::IndexList<formalism::Condition> {};
        conditions.push_back(condition.get_index());
        conditions.push_back(nested_when->get_data().condition);
        const auto combined_condition = as_index(this->self().make_conjunction(std::move(conditions)));
        const auto wrapped = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, combined_condition, nested_when->get_data().effect).get_index())));
        return this->self().normalize_effect(wrapped);
    }
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = std::vector<formalism::EffectView> {};
        for (auto part : nested_and->get_effects())
        {
            const auto wrapped = this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), part.get_index()));
            effects.push_back(this->self().normalize_effect(wrapped));
        }
        const auto conjunction = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
        return this->self().normalize_effect(conjunction);
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        const auto pushed_when = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), nested_forall->get_data().effect).get_index())));
        const auto wrapped = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, nested_forall->get_data().parameters, pushed_when).get_index())));
        return this->self().normalize_effect(wrapped);
    }
    if (const auto exists = this->self().as_exists(condition.get_index()))
    {
        const auto pushed_when = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, exists->get_data().condition, nested.get_index()).get_index())));
        const auto wrapped = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
            formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, exists->get_data().parameters, pushed_when).get_index())));
        return this->self().normalize_effect(wrapped);
    }

    return this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(
        formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), nested.get_index()).get_index()));
}

} // namespace loki::semantic::detail

#endif
