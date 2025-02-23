/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#include "to_effect_normal_form.hpp"

#include "utils.hpp"

namespace loki
{

Effect ToEffectNormalFormTranslator::translate_level_2(EffectAnd effect, Repositories& repositories)
{
    auto translated_nested_effects = this->translate_level_0(effect->get_effects(), repositories);

    // Collect type information of nested effects.
    auto effect_literals = std::unordered_set<Effect> {};
    auto effect_when = EffectList {};
    auto other_effects = EffectList {};
    for (const auto& nested_translated_effect : translated_nested_effects)
    {
        if (const auto nested_translated_effect_literal = std::get_if<EffectLiteral>(&nested_translated_effect->get_effect()))
        {
            effect_literals.emplace(nested_translated_effect);
        }
        else if (const auto nested_translated_effect_when = std::get_if<EffectCompositeWhen>(&nested_translated_effect->get_effect()))
        {
            effect_when.push_back(nested_translated_effect);
        }
        else
        {
            other_effects.push_back(nested_translated_effect);
        }
    }

    // 2. e and (phi > e)  =>  e
    translated_nested_effects = other_effects;
    // Effect literals always make it into the result.
    for (const auto& effect_literal : effect_literals)
    {
        translated_nested_effects.push_back(effect_literal);
    }
    // Remove when effects that are dominated by literal effects
    for (const auto& part : effect_when)
    {
        if (effect_literals.count(std::get<EffectCompositeWhen>(part->get_effect())->get_effect()) == 0)
        {
            translated_nested_effects.push_back(part);
        }
    }

    // 3. e1 and (e2 and e3)  =>  e1 and e2 and e3
    return flatten(repositories.get_or_create_effect_and(translated_nested_effects), repositories);
}

Effect ToEffectNormalFormTranslator::translate_level_2(EffectCompositeForall effect, Repositories& repositories)
{
    auto translated_parameters = this->translate_level_0(effect->get_parameters(), repositories);
    auto translated_nested_effect = this->translate_level_0(effect->get_effect(), repositories);

    if (const auto translated_nested_effect_and = std::get_if<EffectAnd>(&translated_nested_effect->get_effect()))
    {
        // 4. forall(vars, (e1 and e2))  => forall(vars, e1) and forall(vars, e2)
        auto result_parts = EffectList {};
        for (const auto& part : (*translated_nested_effect_and)->get_effects())
        {
            result_parts.push_back(repositories.get_or_create_effect(repositories.get_or_create_effect_composite_forall(translated_parameters, part)));
        }
        return this->translate_level_0(repositories.get_or_create_effect_and(result_parts), repositories);
    }
    // 5. forall(vars1, forall(vars2, e))  =>  forall(vars1+vars2, e)
    return flatten(repositories.get_or_create_effect_composite_forall(translated_parameters, translated_nested_effect), repositories);
}

Effect ToEffectNormalFormTranslator::translate_level_2(EffectCompositeWhen effect, Repositories& repositories)
{
    const auto translated_condition = this->translate_level_0(effect->get_condition(), repositories);
    const auto translated_nested_effect = this->translate_level_0(effect->get_effect(), repositories);

    if (const auto translated_nested_effect_when = std::get_if<EffectCompositeWhen>(&translated_nested_effect->get_effect()))
    {
        // 6. phi > (psi > e)  =>  (phi and psi) > e
        return this->translate_level_0(repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(
                                           repositories.get_or_create_condition(repositories.get_or_create_condition_and(
                                               ConditionList { translated_condition, (*translated_nested_effect_when)->get_condition() })),
                                           (*translated_nested_effect_when)->get_effect())),
                                       repositories);
    }
    else if (const auto translated_nested_effect_and = std::get_if<EffectAnd>(&translated_nested_effect->get_effect()))
    {
        // 7. phi > (e1 and e2)  =>  (phi > e1) and (phi > e2)
        auto parts = EffectList {};
        for (const auto& translated_nested_effect : (*translated_nested_effect_and)->get_effects())
        {
            parts.push_back(
                repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(translated_condition, translated_nested_effect)));
        }
        return this->translate_level_0(repositories.get_or_create_effect_and(parts), repositories);
    }
    else if (const auto translated_nested_effect_forall = std::get_if<EffectCompositeForall>(&translated_nested_effect->get_effect()))
    {
        // 8. phi > forall(vars, e)  => forall(vars, phi > e)
        return this->translate_level_0(
            repositories.get_or_create_effect_composite_forall(
                (*translated_nested_effect_forall)->get_parameters(),
                repositories.get_or_create_effect(
                    repositories.get_or_create_effect_composite_when(translated_condition, (*translated_nested_effect_forall)->get_effect()))),
            repositories);
    }
    else if (const auto translated_condition_exists = std::get_if<ConditionExists>(&translated_condition->get_condition()))
    {
        // 9. exists(vars, phi) > e  => forall(vars, phi > e)
        return this->translate_level_0(
            repositories.get_or_create_effect_composite_forall(
                (*translated_condition_exists)->get_parameters(),
                repositories.get_or_create_effect(
                    repositories.get_or_create_effect_composite_when((*translated_condition_exists)->get_condition(), translated_nested_effect))),
            repositories);
    }

    return repositories.get_or_create_effect(repositories.get_or_create_effect_composite_when(translated_condition, translated_nested_effect));
}

Condition ToEffectNormalFormTranslator::translate_level_2(ConditionAnd condition, Repositories& repositories)
{
    // 10. A and (B and C)  =>  A and B and C
    return flatten(repositories.get_or_create_condition_and(this->translate_level_0(condition->get_conditions(), repositories)), repositories);
}

}