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

#ifndef LOKI_SEMANTIC_TRANSLATOR_COMPILE_CONDITIONAL_EFFECTS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COMPILE_CONDITIONAL_EFFECTS_TRANSLATOR_HPP_

#include "loki/semantic/errors.hpp"
#include "loki/semantic/translator/copy_translator_component.hpp"

#include <limits>
#include <string>
#include <vector>
#include <yggdrasil/containers/unordered_set.hpp>

namespace loki::semantic::detail
{

template<typename Derived>
class CompileConditionalEffectsTranslator : public CopyTranslatorComponent<Derived, CompileConditionalEffectsTranslator<Derived>>
{
public:
    explicit CompileConditionalEffectsTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, CompileConditionalEffectsTranslator<Derived>>(context)
    {
    }

    void compile_conditional_effect_actions(formalism::EntityListView<formalism::Action> actions, ygg::IndexList<formalism::Action>& result);
};

template<typename Derived>
void CompileConditionalEffectsTranslator<Derived>::compile_conditional_effect_actions(formalism::EntityListView<formalism::Action> actions,
                                                                                       ygg::IndexList<formalism::Action>& result)
{
    struct ConditionalEffect
    {
        formalism::EffectWhenView when;
        formalism::ConditionView condition;
        formalism::EffectView effect;
    };

    result.clear();
    auto seen = ygg::UnorderedSet<formalism::ActionView> {};

    for (auto action : actions)
    {
        const auto& data = action.get_data();
        auto unconditional = ygg::IndexList<formalism::Effect> {};
        auto conditional = std::vector<ConditionalEffect> {};

        if (const auto effect_view = action.get_effect())
        {
            auto effects = std::vector<formalism::EffectView> { effect_view.value() };
            if (const auto effect_and = this->self().template as_effect<formalism::EffectAnd>(effect_view.value()))
            {
                effects.clear();
                for (auto effect : effect_and->get_effects())
                    effects.push_back(effect);
            }

            for (auto effect : effects)
            {
                if (const auto effect_when = this->self().template as_effect<formalism::EffectWhen>(effect))
                    conditional.push_back({ *effect_when, effect_when->get_condition(), effect_when->get_effect() });
                else
                    unconditional.push_back(effect.get_index());
            }
        }

        if (conditional.empty())
        {
            this->self().push_unique(result, seen, action);
            continue;
        }

        if (conditional.size() >= std::numeric_limits<size_t>::digits)
            throw SemanticError("Too many conditional effects to compile.");

        const auto num_variants = size_t { 1 } << conditional.size();
        for (auto mask = size_t { 0 }; mask < num_variants; ++mask)
        {
            auto conditions = ygg::IndexList<formalism::Condition> {};
            if (data.precondition)
                conditions.push_back(*data.precondition);

            auto effects = unconditional;
            for (auto i = size_t { 0 }; i < conditional.size(); ++i)
            {
                const auto selected = (mask & (size_t { 1 } << i)) != 0;
                if (selected)
                {
                    conditions.push_back(conditional[i].condition.get_index());
                    effects.push_back(conditional[i].effect.get_index());
                }
                else
                {
                    conditions.push_back(as_index(this->self().negate_condition(conditional[i].condition)));
                }
            }

            auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
            if (conditions.size() == 1)
                precondition = conditions.front();
            else if (!conditions.empty())
                precondition = as_index(this->self().make_conjunction(std::move(conditions)));

            auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
            if (effects.size() == 1)
                effect = effects.front();
            else if (!effects.empty())
            {
                auto effect_data = this->template checkout<formalism::EffectAnd>();
                for (auto item : effects)
                    effect_data->effects.push_back(item);
                effect = this->self().wrap_effect(formalism::get_or_create(this->m_storage->repository, *effect_data)).get_index();
            }

            auto name = std::string(data.name) + "_" + std::to_string(action.get_index().get_value());
            for (const auto& item : conditional)
                name += "_" + std::to_string(item.when.get_index().get_value());
            name += "_" + std::to_string(mask);

            auto action_data = this->template checkout<formalism::Action>();
            action_data->name = cista::offset::string(name);
            action_data->original_name = data.original_name;
            for (auto parameter : data.parameters)
                action_data->parameters.push_back(parameter);
            action_data->original_arity = data.original_arity;
            action_data->precondition = precondition;
            action_data->effect = effect;
            this->self().push_unique(result, seen, formalism::get_or_create(this->m_storage->repository, *action_data));
        }
    }
}

}  // namespace loki::semantic::detail

#endif
