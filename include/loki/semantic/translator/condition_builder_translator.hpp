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

    ygg::Index<formalism::Condition> wrap_condition(ygg::Data<formalism::Condition>::Variant value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionLiteral> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionAnd> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionOr> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionNot> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionImply> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionExists> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionForall> value);
    ygg::Index<formalism::Condition> wrap_condition(ygg::Index<formalism::ConditionNumericConstraint> value);
    cista::optional<ygg::Index<formalism::ConditionOr>> as_or(ygg::Index<formalism::Condition> condition) const;
    ygg::Index<formalism::Condition> make_conjunction(ygg::IndexList<formalism::Condition> conditions);
    ygg::Index<formalism::Condition> make_disjunction(ygg::IndexList<formalism::Condition> conditions);
};

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Data<formalism::Condition>::Variant value)
{
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository, std::move(value)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionLiteral> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionAnd> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionOr> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionNot> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionImply> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionExists> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionForall> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<formalism::ConditionNumericConstraint> value) { return this->self().wrap_condition(ygg::Data<formalism::Condition>::Variant(value)); }

template<typename Derived>
cista::optional<ygg::Index<formalism::ConditionOr>> ConditionBuilderTranslator<Derived>::as_or(ygg::Index<formalism::Condition> condition) const
{
    auto result = cista::optional<ygg::Index<formalism::ConditionOr>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
            result = node;
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::make_conjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionBuilderTranslator<Derived>::make_disjunction(ygg::IndexList<formalism::Condition> conditions)
{
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions)).get_index());
}

} // namespace loki::semantic::detail

#endif
