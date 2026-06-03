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

    formalism::EffectLiteralView copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository);

    formalism::EffectAndView copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository);

    formalism::EffectNumericView copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository);

    formalism::EffectForallView copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository);

    formalism::EffectWhenView copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository);

    formalism::EffectOneOfView copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository);

    formalism::EffectProbabilisticAlternativeView copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository);

    formalism::EffectProbabilisticView copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository);

    formalism::EffectView copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository);

};

template<typename Derived>
formalism::EffectLiteralView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectLiteral> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectLiteral>(this->m_storage->repository, as_index(this->self().copy(repository[source].literal, repository)));
}

template<typename Derived>
formalism::EffectAndView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectAnd> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, this->self().template copy_list<formalism::Effect>(repository[source].effects, repository));
}

template<typename Derived>
formalism::EffectNumericView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectNumeric> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository, data.op, as_index(this->self().copy(data.function, repository)), this->self().template copy_list<formalism::Term>(data.terms, repository), as_index(this->self().copy(data.expression, repository)));
}

template<typename Derived>
formalism::EffectForallView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectForall> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto effect = as_index(this->self().copy(data.effect, repository));
    if (this->m_phase == TranslationPhase::AddTypePredicates)
    {
        auto guard = this->self().type_conditions_for_parameters(parameters);
        const auto condition = this->self().make_conjunction(std::move(guard));
        effect = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, as_index(condition), effect).get_index())));
    }
    const auto out_parameters = this->m_phase == TranslationPhase::AddTypePredicates ? this->self().maybe_strip_parameters(parameters) : parameters;
    const auto out = formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, out_parameters, effect);
    this->self().leave_scope();
    return out;
}

template<typename Derived>
formalism::EffectWhenView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectWhen> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, as_index(this->self().copy(data.condition, repository)), as_index(this->self().copy(data.effect, repository)));
}

template<typename Derived>
formalism::EffectOneOfView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectOneOf> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectOneOf>(this->m_storage->repository, this->self().template copy_list<formalism::Effect>(repository[source].effects, repository));
}

template<typename Derived>
formalism::EffectProbabilisticAlternativeView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectProbabilisticAlternative> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::EffectProbabilisticAlternative>(this->m_storage->repository, data.probability, as_index(this->self().copy(data.effect, repository)));
}

template<typename Derived>
formalism::EffectProbabilisticView EffectTranslator<Derived>::copy(ygg::Index<formalism::EffectProbabilistic> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectProbabilistic>(this->m_storage->repository, this->self().template copy_list<formalism::EffectProbabilisticAlternative>(repository[source].alternatives, repository));
}

template<typename Derived>
formalism::EffectView EffectTranslator<Derived>::copy(ygg::Index<formalism::Effect> source, const formalism::Repository& repository)
{
    if (this->m_phase == TranslationPhase::SplitDisjunctiveConditions)
    {
        auto split = cista::optional<ygg::Index<formalism::Effect>> {};
        std::visit(
            [&](const auto& arg)
            {
                using Node = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Node, ygg::Index<formalism::EffectWhen>>)
                {
                    const auto& data = repository[arg];
                    const auto condition = as_index(this->self().copy(data.condition, repository));
                    const auto effect = as_index(this->self().copy(data.effect, repository));
                    if (const auto condition_or = this->self().as_or(condition))
                    {
                        auto effects = ygg::IndexList<formalism::Effect> {};
                        for (auto part : condition_or->get_data().conditions)
                        {
                            const auto when = formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, part, effect).get_index();
                            effects.push_back(as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(when))));
                        }
                        split = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)).get_index())));
                    }
                    else
                    {
                        split = as_index(this->self().wrap_effect(ygg::Data<formalism::Effect>::Variant(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition, effect).get_index())));
                    }
                }
            },
            repository[source].value);
        if (split)
            return ygg::make_view(*split, this->m_storage->repository);
    }

    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return ygg::Data<formalism::Effect>::Variant(as_index(this->self().copy(arg, repository))); }, repository[source].value);
    auto copied = formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
    if (this->m_phase == TranslationPhase::ToEffectNormalForm)
        return this->self().normalize_effect(copied.get_index());
    return copied;
}

} // namespace loki::semantic::detail

#endif
