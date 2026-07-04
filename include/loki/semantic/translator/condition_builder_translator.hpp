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

#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_BUILDER_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_BUILDER_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionBuilderTranslator : public CopyTranslatorComponent<Derived, ConditionBuilderTranslator<Derived>>
{
public:
    explicit ConditionBuilderTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionBuilderTranslator<Derived>>(context) {}

    formalism::ConditionView wrap_condition(ygg::Data<formalism::Condition>::Variant value);
    template<typename T>
    formalism::ConditionView wrap_condition(formalism::EntityView<T> value);
    template<typename T>
    formalism::ConditionView wrap_condition(ygg::Index<T> value);
    std::optional<formalism::ConditionOrView> as_or(formalism::ConditionView condition) const;
    formalism::ConditionView flatten_condition(formalism::ConditionView condition);
    formalism::ConditionView make_conjunction(ygg::IndexList<formalism::Condition> conditions);
    formalism::ConditionView make_disjunction(ygg::IndexList<formalism::Condition> conditions);
};

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Data<formalism::Condition>::Variant value)
{
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(formalism::EntityView<T> value)
{
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository,
                                                          typename ygg::Data<formalism::Condition>::template ViewVariant<formalism::Repository>(value));
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<T> value)
{
    return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value));
}

template<typename Derived>
std::optional<formalism::ConditionOrView> ConditionBuilderTranslator<Derived>::as_or(formalism::ConditionView condition) const
{
    auto result = std::optional<formalism::ConditionOrView> {};
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionOrView>)
                result = node;
        },
        condition.get_value());
    return result;
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::flatten_condition(formalism::ConditionView condition)
{
    return ygg::visit(
        [&](const auto& node) -> formalism::ConditionView
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionAndView>)
            {
                auto parts = std::vector<formalism::ConditionView> {};
                for (auto child : node.get_conditions())
                {
                    const auto flat = this->self().flatten_condition(child);
                    ygg::visit(
                        [&](const auto& flat_node)
                        {
                            using FlatNode = std::decay_t<decltype(flat_node)>;
                            if constexpr (std::is_same_v<FlatNode, formalism::ConditionAndView>)
                            {
                                for (auto part : flat_node.get_conditions())
                                    parts.push_back(part);
                            }
                            else
                            {
                                parts.push_back(flat);
                            }
                        },
                        flat.get_value());
                }
                return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(parts)));
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionOrView>)
            {
                auto parts = std::vector<formalism::ConditionView> {};
                for (auto child : node.get_conditions())
                {
                    const auto flat = this->self().flatten_condition(child);
                    ygg::visit(
                        [&](const auto& flat_node)
                        {
                            using FlatNode = std::decay_t<decltype(flat_node)>;
                            if constexpr (std::is_same_v<FlatNode, formalism::ConditionOrView>)
                            {
                                for (auto part : flat_node.get_conditions())
                                    parts.push_back(part);
                            }
                            else
                            {
                                parts.push_back(flat);
                            }
                        },
                        flat.get_value());
                }
                return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(parts)));
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionExistsView>)
            {
                const auto flat = this->self().flatten_condition(node.get_condition());
                return ygg::visit(
                    [&](const auto& flat_node) -> formalism::ConditionView
                    {
                        using FlatNode = std::decay_t<decltype(flat_node)>;
                        if constexpr (std::is_same_v<FlatNode, formalism::ConditionExistsView>)
                        {
                            auto parameters = std::vector<formalism::ParameterView> {};
                            for (auto parameter : node.get_parameters())
                                parameters.push_back(parameter);
                            for (auto parameter : flat_node.get_parameters())
                                parameters.push_back(parameter);
                            return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository,
                                                                                                                    std::move(parameters),
                                                                                                                    flat_node.get_condition()));
                        }
                        else
                        {
                            auto parameters = std::vector<formalism::ParameterView> {};
                            for (auto parameter : node.get_parameters())
                                parameters.push_back(parameter);
                            return this->self().wrap_condition(
                                formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), flat));
                        }
                    },
                    flat.get_value());
            }
            else if constexpr (std::is_same_v<Node, formalism::ConditionForallView>)
            {
                const auto flat = this->self().flatten_condition(node.get_condition());
                return ygg::visit(
                    [&](const auto& flat_node) -> formalism::ConditionView
                    {
                        using FlatNode = std::decay_t<decltype(flat_node)>;
                        if constexpr (std::is_same_v<FlatNode, formalism::ConditionForallView>)
                        {
                            auto parameters = std::vector<formalism::ParameterView> {};
                            for (auto parameter : node.get_parameters())
                                parameters.push_back(parameter);
                            for (auto parameter : flat_node.get_parameters())
                                parameters.push_back(parameter);
                            return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository,
                                                                                                                    std::move(parameters),
                                                                                                                    flat_node.get_condition()));
                        }
                        else
                        {
                            auto parameters = std::vector<formalism::ParameterView> {};
                            for (auto parameter : node.get_parameters())
                                parameters.push_back(parameter);
                            return this->self().wrap_condition(
                                formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), flat));
                        }
                    },
                    flat.get_value());
            }
            else
            {
                return condition;
            }
        },
        condition.get_value());
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::make_conjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions))));
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::make_disjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions))));
}

}  // namespace loki::semantic::detail

#endif
