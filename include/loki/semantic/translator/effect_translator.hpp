/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_EFFECT_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_EFFECT_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class EffectTranslator : public CopyTranslatorComponent<Derived, EffectTranslator<Derived>>
{
public:
    explicit EffectTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EffectTranslator<Derived>>(context) {}

    ygg::Index<formalism::EffectLiteral> copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectAnd> copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectNumeric> copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectForall> copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectWhen> copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectOneOf> copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectProbabilisticAlternative> copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository);

    ygg::Index<formalism::EffectProbabilistic> copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository);

    ygg::Index<formalism::Effect> copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository);

};

template<typename Derived>
ygg::Index<formalism::EffectLiteral> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectLiteral>(this->m_storage->repository, this->self().copy(repository[source].literal, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectAnd> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, this->self().template copy_list<formalism::Effect>(repository[source].effects, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectNumeric> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository, data.op, this->self().copy(data.function, repository), this->self().template copy_list<formalism::Term>(data.terms, repository), this->self().copy(data.expression, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectForall> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto effect = this->self().copy(data.effect, repository);
    auto guard = this->self().type_conditions_for_parameters(parameters);
    if (!guard.empty())
    {
        const auto condition = this->self().make_conjunction(std::move(guard));
        effect = this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition, effect).get_index()));
    }
    const auto out = formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), effect).get_index();
    this->self().leave_scope();
    return out;
}

template<typename Derived>
ygg::Index<formalism::EffectWhen> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, this->self().copy(data.condition, repository), this->self().copy(data.effect, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectOneOf> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectOneOf>(this->m_storage->repository, this->self().template copy_list<formalism::Effect>(repository[source].effects, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectProbabilisticAlternative> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectProbabilisticAlternative>(this->m_storage->repository, data.probability, this->self().copy(data.effect, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::EffectProbabilistic> EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectProbabilistic>(this->m_storage->repository, this->self().template copy_list<formalism::EffectProbabilisticAlternative>(repository[source].alternatives, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Effect> EffectTranslator<Derived>::copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return ygg::Data<formalism::Effect>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    return this->self().normalize_effect(formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value)).get_index());
}

} // namespace loki::semantic::detail

#endif
