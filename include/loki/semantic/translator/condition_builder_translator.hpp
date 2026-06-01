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

    ygg::Index<pddl::Condition> wrap_condition(ygg::Data<pddl::Condition>::Variant value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionLiteral> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionAnd> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionOr> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionNot> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionImply> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionExists> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionForall> value);
    ygg::Index<pddl::Condition> wrap_condition(ygg::Index<pddl::ConditionNumericConstraint> value);
    cista::optional<ygg::Index<pddl::ConditionOr>> as_or(ygg::Index<pddl::Condition> condition) const;
    ygg::Index<pddl::Condition> make_conjunction(ygg::IndexList<pddl::Condition> conditions);
    ygg::Index<pddl::Condition> make_disjunction(ygg::IndexList<pddl::Condition> conditions);
};

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Data<pddl::Condition>::Variant value)
{
    return pddl::get_or_create<pddl::Condition>(this->m_storage->repository, std::move(value)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionLiteral> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionAnd> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionOr> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionNot> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionImply> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionExists> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionForall> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::wrap_condition(ygg::Index<pddl::ConditionNumericConstraint> value) { return this->self().wrap_condition(ygg::Data<pddl::Condition>::Variant(value)); }

template<typename Derived>
cista::optional<ygg::Index<pddl::ConditionOr>> ConditionBuilderTranslator<Derived>::as_or(ygg::Index<pddl::Condition> condition) const
{
    auto result = cista::optional<ygg::Index<pddl::ConditionOr>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<pddl::ConditionOr>>)
            result = node;
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::make_conjunction(ygg::IndexList<pddl::Condition> conditions)
{
    return this->self().wrap_condition(pddl::get_or_create<pddl::ConditionAnd>(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionBuilderTranslator<Derived>::make_disjunction(ygg::IndexList<pddl::Condition> conditions)
{
    return this->self().wrap_condition(pddl::get_or_create<pddl::ConditionOr>(this->m_storage->repository, std::move(conditions)).get_index());
}

} // namespace loki::semantic::detail

#endif
