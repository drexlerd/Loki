/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_EFFECT_NORMAL_FORM_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class EffectNormalFormTranslator : public CopyTranslatorComponent<Derived, EffectNormalFormTranslator<Derived>>
{
public:
    explicit EffectNormalFormTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EffectNormalFormTranslator<Derived>>(context) {}

    ygg::Index<formalism::Effect> wrap_effect(ygg::Data<formalism::Effect>::Variant value);
    template<typename T>
    cista::optional<ygg::Index<T>> as_effect(ygg::Index<formalism::Effect> effect) const;
    ygg::Index<formalism::Effect> normalize_effect(ygg::Index<formalism::Effect> effect);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectLiteral>);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectNumeric>);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectOneOf>);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectProbabilistic>);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectAnd> node);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectForall> node);
    ygg::Index<formalism::Effect> normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectWhen> node);
};

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::wrap_effect(ygg::Data<formalism::Effect>::Variant value)
{
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value)).get_index();
}

template<typename Derived>
template<typename T>
cista::optional<ygg::Index<T>> EffectNormalFormTranslator<Derived>::as_effect(ygg::Index<formalism::Effect> effect) const
{
    auto result = cista::optional<ygg::Index<T>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<T>>)
            result = node;
    }, this->m_storage->repository[effect].value);
    return result;
}

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect(ygg::Index<formalism::Effect> effect)
{
    return std::visit([&](const auto& node) { return this->self().normalize_effect_node(effect, node); }, this->m_storage->repository[effect].value);
}

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectLiteral>) { return effect; }

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectNumeric>) { return effect; }

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectOneOf>) { return effect; }

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect> effect, ygg::Index<formalism::EffectProbabilistic>) { return effect; }

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectAnd> node)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto child : this->m_storage->repository[node].effects)
    {
        const auto normalized = this->self().normalize_effect(child);
        if (const auto nested = this->self().template as_effect<formalism::EffectAnd>(normalized))
        {
            for (auto part : this->m_storage->repository[*nested].effects)
                effects.push_back(part);
        }
        else
        {
            effects.push_back(normalized);
        }
    }
    return this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)).get_index()));
}

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectForall> node)
{
    const auto& data = this->m_storage->repository[node];
    const auto nested = this->self().normalize_effect(data.effect);
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = ygg::IndexList<formalism::Effect> {};
        for (auto part : this->m_storage->repository[*nested_and].effects)
            effects.push_back(this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, part).get_index()))));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)).get_index())));
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        auto parameters = data.parameters;
        for (auto parameter : this->m_storage->repository[*nested_forall].parameters)
            parameters.push_back(parameter);
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), this->m_storage->repository[*nested_forall].effect).get_index())));
    }
    return this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, data.parameters, nested).get_index()));
}

template<typename Derived>
ygg::Index<formalism::Effect> EffectNormalFormTranslator<Derived>::normalize_effect_node(ygg::Index<formalism::Effect>, ygg::Index<formalism::EffectWhen> node)
{
    const auto& data = this->m_storage->repository[node];
    auto condition = this->self().flatten_condition(this->self().to_dnf(this->self().move_existentials(data.condition)));
    const auto nested = this->self().normalize_effect(data.effect);

    if (const auto condition_or = this->self().as_or(condition))
    {
        auto effects = ygg::IndexList<formalism::Effect> {};
        for (auto part : this->m_storage->repository[*condition_or].conditions)
            effects.push_back(this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, part, nested).get_index()))));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)).get_index())));
    }

    if (this->self().as_exists(condition))
        condition = this->self().make_generated_positive_condition(condition);

    if (const auto nested_when = this->self().template as_effect<formalism::EffectWhen>(nested))
    {
        auto conditions = ygg::IndexList<formalism::Condition> {};
        conditions.push_back(condition);
        conditions.push_back(this->m_storage->repository[*nested_when].condition);
        const auto combined_condition = this->self().make_conjunction(std::move(conditions));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, combined_condition, this->m_storage->repository[*nested_when].effect).get_index())));
    }
    if (const auto nested_and = this->self().template as_effect<formalism::EffectAnd>(nested))
    {
        auto effects = ygg::IndexList<formalism::Effect> {};
        for (auto part : this->m_storage->repository[*nested_and].effects)
            effects.push_back(this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition, part).get_index()))));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)).get_index())));
    }
    if (const auto nested_forall = this->self().template as_effect<formalism::EffectForall>(nested))
    {
        const auto pushed_when = this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition, this->m_storage->repository[*nested_forall].effect).get_index()));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, this->m_storage->repository[*nested_forall].parameters, pushed_when).get_index())));
    }
    if (const auto exists = this->self().as_exists(condition))
    {
        const auto pushed_when = this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, this->m_storage->repository[*exists].condition, nested).get_index()));
        return this->self().normalize_effect(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, this->m_storage->repository[*exists].parameters, pushed_when).get_index())));
    }

    return this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition, nested).get_index()));
}

} // namespace loki::semantic::detail

#endif
