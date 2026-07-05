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

#ifndef LOKI_SEMANTIC_TRANSLATOR_MOVE_EXISTENTIAL_QUANTIFIERS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_MOVE_EXISTENTIAL_QUANTIFIERS_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class MoveExistentialQuantifiersTranslator : public CopyTranslatorComponent<Derived, MoveExistentialQuantifiersTranslator<Derived>>
{
public:
    explicit MoveExistentialQuantifiersTranslator(CopyContext& context) :
        CopyTranslatorComponent<Derived, MoveExistentialQuantifiersTranslator<Derived>>(context)
    {
    }

    std::optional<formalism::ConditionExistsView> as_exists(formalism::ConditionView condition) const;
    formalism::ConditionView move_existentials(formalism::ConditionView condition);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionAndView node);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionExistsView node);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionOrView node);
    template<typename T>
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::EntityView<T>);
    formalism::ConditionView lift_top_level_exists(std::vector<formalism::ParameterView>& parameters, formalism::ConditionView condition);
};

template<typename Derived>
std::optional<formalism::ConditionExistsView> MoveExistentialQuantifiersTranslator<Derived>::as_exists(formalism::ConditionView condition) const
{
    auto result = std::optional<formalism::ConditionExistsView> {};
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionExistsView>)
                result = node;
        },
        condition.get_value());
    return result;
}

template<typename Derived>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::move_existentials(formalism::ConditionView condition)
{
    return ygg::visit([&](const auto& node) { return this->self().move_existentials_node(condition, node); }, condition.get_value());
}

template<typename Derived>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionAndView node)
{
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    auto seen_parameters = ygg::UnorderedSet<formalism::ParameterView> {};
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
    {
        const auto moved = this->self().move_existentials(child);
        if (const auto exists = this->self().as_exists(moved))
        {
            for (auto parameter : exists->get_parameters())
            {
                if (seen_parameters.insert(parameter).second)
                    parameters.push_back(parameter.get_index());
            }
            parts.push_back(exists->get_condition().get_index());
        }
        else
        {
            parts.push_back(moved.get_index());
        }
    }
    auto conjunction = this->self().make_conjunction(std::move(parts));
    if (parameters.empty())
        return conjunction;
    return this->self().flatten_condition(this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), conjunction.get_index())));
}

template<typename Derived>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionExistsView node)
{
    const auto& data = node.get_data();
    const auto condition = this->self().move_existentials(node.get_condition());
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, condition.get_index())));
}

template<typename Derived>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionOrView node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
        parts.push_back(this->self().move_existentials(child).get_index());
    return this->self().make_disjunction(std::move(parts));
}

template<typename Derived>
template<typename T>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::move_existentials_node(formalism::ConditionView condition, formalism::EntityView<T>)
{
    return condition;
}

template<typename Derived>
formalism::ConditionView MoveExistentialQuantifiersTranslator<Derived>::lift_top_level_exists(std::vector<formalism::ParameterView>& parameters,
                                                                                              formalism::ConditionView condition)
{
    auto moved = this->self().move_existentials(condition);
    if (const auto exists = this->self().as_exists(moved))
    {
        auto seen_parameters = ygg::UnorderedSet<formalism::ParameterView> {};
        for (auto parameter : parameters)
            seen_parameters.insert(parameter);

        for (auto parameter : exists->get_parameters())
        {
            if (seen_parameters.insert(parameter).second)
                parameters.push_back(parameter);
        }
        moved = exists->get_condition();
    }
    return this->self().flatten_condition(moved);
}

}  // namespace loki::semantic::detail

#endif
