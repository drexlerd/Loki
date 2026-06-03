/*
 * Copyright (C) 2026 Dominik Drexler
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
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionLiteral> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionAnd> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionOr> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionNot> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionImply> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionExists> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionForall> value);
    formalism::ConditionView wrap_condition(ygg::Index<formalism::ConditionNumericConstraint> value);
    std::optional<formalism::ConditionOrView> as_or(ygg::Index<formalism::Condition> condition) const;
    formalism::ConditionView flatten_condition(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView make_conjunction(ygg::IndexList<formalism::Condition> conditions);
    formalism::ConditionView make_disjunction(ygg::IndexList<formalism::Condition> conditions);
};

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Data<formalism::Condition>::Variant value)
{
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionLiteral> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionAnd> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionOr> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionNot> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionImply> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionExists> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionForall> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionNumericConstraint> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
std::optional<formalism::ConditionOrView> ConditionBuilderTranslator<Derived>::as_or(ygg::Index<formalism::Condition> condition) const
{
    auto result = std::optional<formalism::ConditionOrView> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
            result = ygg::make_view(node, this->m_storage->repository);
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::flatten_condition(ygg::Index<formalism::Condition> condition)
{
    return ygg::make_view(std::visit(
        [&](const auto& node) -> ygg::Index<formalism::Condition>
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionAnd>>)
            {
                auto parts = ygg::IndexList<formalism::Condition> {};
                for (auto child : this->m_storage->repository[node].conditions)
                {
                    const auto flat = as_index(this->self().flatten_condition(child));
                    std::visit(
                        [&](const auto& flat_node)
                        {
                            using FlatNode = std::decay_t<decltype(flat_node)>;
                            if constexpr (std::is_same_v<FlatNode, ygg::Index<formalism::ConditionAnd>>)
                            {
                                const auto& nested = this->m_storage->repository[flat_node].conditions;
                                parts.insert(parts.end(), nested.begin(), nested.end());
                            }
                            else
                            {
                                parts.push_back(flat);
                            }
                        },
                        this->m_storage->repository[flat].value);
                }
                return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(parts)).get_index()));
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
            {
                auto parts = ygg::IndexList<formalism::Condition> {};
                for (auto child : this->m_storage->repository[node].conditions)
                {
                    const auto flat = as_index(this->self().flatten_condition(child));
                    std::visit(
                        [&](const auto& flat_node)
                        {
                            using FlatNode = std::decay_t<decltype(flat_node)>;
                            if constexpr (std::is_same_v<FlatNode, ygg::Index<formalism::ConditionOr>>)
                            {
                                const auto& nested = this->m_storage->repository[flat_node].conditions;
                                parts.insert(parts.end(), nested.begin(), nested.end());
                            }
                            else
                            {
                                parts.push_back(flat);
                            }
                        },
                        this->m_storage->repository[flat].value);
                }
                return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(parts)).get_index()));
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionExists>>)
            {
                const auto& data = this->m_storage->repository[node];
                const auto flat = as_index(this->self().flatten_condition(data.condition));
                return std::visit(
                    [&](const auto& flat_node) -> ygg::Index<formalism::Condition>
                    {
                        using FlatNode = std::decay_t<decltype(flat_node)>;
                        if constexpr (std::is_same_v<FlatNode, ygg::Index<formalism::ConditionExists>>)
                        {
                            auto parameters = data.parameters;
                            const auto& nested = this->m_storage->repository[flat_node];
                            parameters.insert(parameters.end(), nested.parameters.begin(), nested.parameters.end());
                            return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), nested.condition).get_index()));
                        }
                        else
                        {
                            return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, flat).get_index()));
                        }
                    },
                    this->m_storage->repository[flat].value);
            }
            else if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionForall>>)
            {
                const auto& data = this->m_storage->repository[node];
                const auto flat = as_index(this->self().flatten_condition(data.condition));
                return std::visit(
                    [&](const auto& flat_node) -> ygg::Index<formalism::Condition>
                    {
                        using FlatNode = std::decay_t<decltype(flat_node)>;
                        if constexpr (std::is_same_v<FlatNode, ygg::Index<formalism::ConditionForall>>)
                        {
                            auto parameters = data.parameters;
                            const auto& nested = this->m_storage->repository[flat_node];
                            parameters.insert(parameters.end(), nested.parameters.begin(), nested.parameters.end());
                            return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), nested.condition).get_index()));
                        }
                        else
                        {
                            return as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, data.parameters, flat).get_index()));
                        }
                    },
                    this->m_storage->repository[flat].value);
            }
            else
            {
                return condition;
            }
        },
        this->m_storage->repository[condition].value), this->m_storage->repository);
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::make_conjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions)).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionBuilderTranslator<Derived>::make_disjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions)).get_index())));
}

} // namespace loki::semantic::detail

#endif
