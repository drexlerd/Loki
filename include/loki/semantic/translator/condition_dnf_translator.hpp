/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_DNF_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_DNF_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionDnfTranslator : public CopyTranslatorComponent<Derived, ConditionDnfTranslator<Derived>>
{
public:
    explicit ConditionDnfTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionDnfTranslator<Derived>>(context) {}

    ygg::Index<formalism::Condition> to_dnf(ygg::Index<formalism::Condition> condition);
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>);
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>);
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionOr> node);
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionAnd> node);
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionExists> node);
    template<typename T>
    ygg::Index<formalism::Condition> to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>);
};

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf(ygg::Index<formalism::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().to_dnf_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>) { return condition; }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>) { return condition; }

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionOr> node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto dnf = this->self().to_dnf(child);
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : this->m_storage->repository[*child_or].conditions)
                parts.push_back(nested);
        }
        else
        {
            parts.push_back(dnf);
        }
    }
    return this->self().make_disjunction(std::move(parts));
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionAnd> node)
{
    auto combinations = std::vector<ygg::IndexList<formalism::Condition>> { ygg::IndexList<formalism::Condition> {} };
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto dnf = this->self().to_dnf(child);
        auto alternatives = ygg::IndexList<formalism::Condition> {};
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : this->m_storage->repository[*child_or].conditions)
                alternatives.push_back(nested);
        }
        else
        {
            alternatives.push_back(dnf);
        }
        auto next = std::vector<ygg::IndexList<formalism::Condition>> {};
        for (const auto& combination : combinations)
        {
            for (auto alternative : alternatives)
            {
                auto extended = combination;
                extended.push_back(alternative);
                next.push_back(std::move(extended));
            }
        }
        combinations = std::move(next);
    }
    if (combinations.size() == 1)
        return this->self().make_conjunction(std::move(combinations.front()));
    auto disjuncts = ygg::IndexList<formalism::Condition> {};
    for (auto& combination : combinations)
        disjuncts.push_back(this->self().make_conjunction(std::move(combination)));
    return this->self().make_disjunction(std::move(disjuncts));
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionExists> node)
{
    const auto& data = this->m_storage->repository[node];
    const auto child = this->self().to_dnf(data.condition);
    if (const auto child_or = this->self().as_or(child))
    {
        auto parts = ygg::IndexList<formalism::Condition> {};
        for (auto nested : this->m_storage->repository[*child_or].conditions)
            parts.push_back(this->self().flatten_condition(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, nested).get_index())));
        return this->self().to_dnf(this->self().make_disjunction(std::move(parts)));
    }
    return this->self().flatten_condition(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, child).get_index()));
}

template<typename Derived>
template<typename T>
ygg::Index<formalism::Condition> ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>)
{
    return condition;
}

} // namespace loki::semantic::detail

#endif
