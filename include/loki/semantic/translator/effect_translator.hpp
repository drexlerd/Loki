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

#ifndef LOKI_SEMANTIC_TRANSLATOR_EFFECT_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_EFFECT_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <optional>

namespace loki::semantic::detail
{

template<typename Derived>
class EffectTranslator : public CopyTranslatorComponent<Derived, EffectTranslator<Derived>>
{
public:
    explicit EffectTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EffectTranslator<Derived>>(context) {}

    formalism::EffectLiteralView copy(formalism::EffectLiteralView source);

    formalism::EffectAndView copy(formalism::EffectAndView source);

    formalism::EffectNumericView copy(formalism::EffectNumericView source);

    formalism::EffectForallView copy(formalism::EffectForallView source);

    formalism::EffectWhenView copy(formalism::EffectWhenView source);

    formalism::EffectOneOfView copy(formalism::EffectOneOfView source);

    formalism::EffectProbabilisticAlternativeView copy(formalism::EffectProbabilisticAlternativeView source);

    formalism::EffectProbabilisticView copy(formalism::EffectProbabilisticView source);

    formalism::EffectView copy(formalism::EffectView source);
};

template<typename Derived>
formalism::EffectLiteralView EffectTranslator<Derived>::copy(formalism::EffectLiteralView source)
{
    return formalism::get_or_create<formalism::EffectLiteral>(this->m_storage->repository, as_index(this->self().copy(source.get_literal())));
}

template<typename Derived>
formalism::EffectAndView EffectTranslator<Derived>::copy(formalism::EffectAndView source)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().copy(effect)));
    return formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectNumericView EffectTranslator<Derived>::copy(formalism::EffectNumericView source)
{
    const auto& data = source.get_data();
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().copy(term)));
    return formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository,
                                                              data.op,
                                                              as_index(this->self().copy(source.get_function())),
                                                              std::move(terms),
                                                              as_index(this->self().copy(source.get_expression())));
}

template<typename Derived>
formalism::EffectForallView EffectTranslator<Derived>::copy(formalism::EffectForallView source)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameters = this->self().copy_parameters(source.get_parameters());
    this->self().enter_scope(parameters, source.get_parameters());
    auto effect = as_index(this->self().copy(source.get_effect()));
    if (this->m_phase == TranslationPhase::AddTypePredicates)
    {
        auto guard = this->self().type_conditions_for_parameters(source.get_parameters());
        const auto condition = this->self().make_conjunction(std::move(guard));
        effect =
            this->self().wrap_effect(formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), effect)).get_index();
    }
    const auto out_parameters = this->self().removes_typing_now() ? this->self().copy_parameters_without_types(source.get_parameters()) : parameters;
    const auto out = formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, out_parameters, effect);
    this->self().leave_scope();
    return out;
}

template<typename Derived>
formalism::EffectWhenView EffectTranslator<Derived>::copy(formalism::EffectWhenView source)
{
    return formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository,
                                                           as_index(this->self().copy(source.get_condition())),
                                                           as_index(this->self().copy(source.get_effect())));
}

template<typename Derived>
formalism::EffectOneOfView EffectTranslator<Derived>::copy(formalism::EffectOneOfView source)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().copy(effect)));
    return formalism::get_or_create<formalism::EffectOneOf>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectProbabilisticAlternativeView EffectTranslator<Derived>::copy(formalism::EffectProbabilisticAlternativeView source)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::EffectProbabilisticAlternative>(this->m_storage->repository,
                                                                               data.probability,
                                                                               as_index(this->self().copy(source.get_effect())));
}

template<typename Derived>
formalism::EffectProbabilisticView EffectTranslator<Derived>::copy(formalism::EffectProbabilisticView source)
{
    auto alternatives = ygg::IndexList<formalism::EffectProbabilisticAlternative> {};
    for (auto alternative : source.get_alternatives())
        alternatives.push_back(as_index(this->self().copy(alternative)));
    return formalism::get_or_create<formalism::EffectProbabilistic>(this->m_storage->repository, std::move(alternatives));
}

template<typename Derived>
formalism::EffectView EffectTranslator<Derived>::copy(formalism::EffectView source)
{
    if (this->m_phase == TranslationPhase::SplitDisjunctiveConditions)
    {
        auto split = std::optional<formalism::EffectView> {};
        ygg::visit(
            [&](const auto& arg)
            {
                using Node = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<Node, formalism::EffectWhenView>)
                {
                    const auto condition = this->self().copy(arg.get_condition());
                    const auto effect = as_index(this->self().copy(arg.get_effect()));
                    if (const auto condition_or = this->self().as_or(condition))
                    {
                        auto effects = ygg::IndexList<formalism::Effect> {};
                        for (auto part : condition_or->get_data().conditions)
                        {
                            const auto when = formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, part, effect);
                            effects.push_back(this->self().wrap_effect(when).get_index());
                        }
                        split = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects)));
                    }
                    else
                    {
                        split = this->self().wrap_effect(
                            formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository, condition.get_index(), effect));
                    }
                }
            },
            source.get_value());
        if (split)
            return *split;
    }

    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant
                            { return ygg::Data<formalism::Effect>::Variant(as_index(this->self().copy(arg))); },
                            source.get_value());
    auto copied = formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
    if (this->m_phase == TranslationPhase::ToEffectNormalForm)
        return this->self().normalize_effect(copied);
    return copied;
}

}  // namespace loki::semantic::detail

#endif
