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

    ygg::Index<pddl::EffectLiteral> copy(ygg::Index<pddl::EffectLiteral> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectAnd> copy(ygg::Index<pddl::EffectAnd> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectNumeric> copy(ygg::Index<pddl::EffectNumeric> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectForall> copy(ygg::Index<pddl::EffectForall> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectWhen> copy(ygg::Index<pddl::EffectWhen> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectOneOf> copy(ygg::Index<pddl::EffectOneOf> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectProbabilisticAlternative> copy(ygg::Index<pddl::EffectProbabilisticAlternative> source, const pddl::Repository& repository);

    ygg::Index<pddl::EffectProbabilistic> copy(ygg::Index<pddl::EffectProbabilistic> source, const pddl::Repository& repository);

    ygg::Index<pddl::Effect> copy(ygg::Index<pddl::Effect> source, const pddl::Repository& repository);

};

template<typename Derived>
ygg::Index<pddl::EffectLiteral> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectLiteral> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectLiteral> out;
    if (find_mapped(this->m_storage->effect_literals, source, out)) return out;
    out = pddl::get_or_create<pddl::EffectLiteral>(this->m_storage->repository, this->self().copy(repository[source].literal, repository)).get_index();
    remember(this->m_storage->effect_literals, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectAnd> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectAnd> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectAnd> out;
    if (find_mapped(this->m_storage->effect_ands, source, out)) return out;
    out = pddl::get_or_create<pddl::EffectAnd>(this->m_storage->repository, this->self().template copy_list<pddl::Effect>(repository[source].effects, repository)).get_index();
    remember(this->m_storage->effect_ands, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectNumeric> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectNumeric> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectNumeric> out;
    if (find_mapped(this->m_storage->effect_numerics, source, out)) return out;
    const auto& data = repository[source];
    out = pddl::get_or_create<pddl::EffectNumeric>(this->m_storage->repository, data.op, this->self().copy(data.function, repository), this->self().template copy_list<pddl::Term>(data.terms, repository), this->self().copy(data.expression, repository)).get_index();
    remember(this->m_storage->effect_numerics, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectForall> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectForall> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectForall> out;
    if (find_mapped(this->m_storage->effect_foralls, source, out)) return out;
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto effect = this->self().copy(data.effect, repository);
    auto guard = this->self().type_conditions_for_parameters(parameters);
    if (!guard.empty())
    {
        const auto condition = this->self().make_conjunction(std::move(guard));
        effect = this->self().wrap_effect(ygg::Data<pddl::Effect>::Variant(pddl::get_or_create<pddl::EffectWhen>(this->m_storage->repository, condition, effect).get_index()));
    }
    out = pddl::get_or_create<pddl::EffectForall>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), effect).get_index();
    this->self().leave_scope();
    remember(this->m_storage->effect_foralls, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectWhen> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectWhen> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectWhen> out;
    if (find_mapped(this->m_storage->effect_whens, source, out)) return out;
    const auto& data = repository[source];
    out = pddl::get_or_create<pddl::EffectWhen>(this->m_storage->repository, this->self().copy(data.condition, repository), this->self().copy(data.effect, repository)).get_index();
    remember(this->m_storage->effect_whens, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectOneOf> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectOneOf> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectOneOf> out;
    if (find_mapped(this->m_storage->effect_one_ofs, source, out)) return out;
    out = pddl::get_or_create<pddl::EffectOneOf>(this->m_storage->repository, this->self().template copy_list<pddl::Effect>(repository[source].effects, repository)).get_index();
    remember(this->m_storage->effect_one_ofs, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectProbabilisticAlternative> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectProbabilisticAlternative> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectProbabilisticAlternative> out;
    if (find_mapped(this->m_storage->effect_probabilistic_alternatives, source, out)) return out;
    const auto& data = repository[source];
    out = pddl::get_or_create<pddl::EffectProbabilisticAlternative>(this->m_storage->repository, data.probability, this->self().copy(data.effect, repository)).get_index();
    remember(this->m_storage->effect_probabilistic_alternatives, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::EffectProbabilistic> EffectTranslator<Derived>::copy(ygg::Index<pddl::EffectProbabilistic> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::EffectProbabilistic> out;
    if (find_mapped(this->m_storage->effect_probabilistics, source, out)) return out;
    out = pddl::get_or_create<pddl::EffectProbabilistic>(this->m_storage->repository, this->self().template copy_list<pddl::EffectProbabilisticAlternative>(repository[source].alternatives, repository)).get_index();
    remember(this->m_storage->effect_probabilistics, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Effect> EffectTranslator<Derived>::copy(ygg::Index<pddl::Effect> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Effect> out;
    if (find_mapped(this->m_storage->effects, source, out)) return out;
    auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Effect>::Variant { return ygg::Data<pddl::Effect>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    out = this->self().normalize_effect(pddl::get_or_create<pddl::Effect>(this->m_storage->repository, std::move(value)).get_index());
    remember(this->m_storage->effects, source, out);
    return out;
}

} // namespace loki::semantic::detail

#endif
