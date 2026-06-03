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

    formalism::ConditionView to_dnf(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionOr> node);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionAnd> node);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionExists> node);
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionForall> node);
    template<typename T>
    formalism::ConditionView to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>);
};

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf(ygg::Index<formalism::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().to_dnf_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>) { return ygg::make_view(condition, this->m_storage->repository); }

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>) { return ygg::make_view(condition, this->m_storage->repository); }

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionOr> node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto dnf = as_index(this->self().to_dnf(child));
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : child_or->get_data().conditions)
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
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionAnd> node)
{
    auto combinations = std::vector<ygg::IndexList<formalism::Condition>> { ygg::IndexList<formalism::Condition> {} };
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto dnf = as_index(this->self().to_dnf(child));
        auto alternatives = ygg::IndexList<formalism::Condition> {};
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : child_or->get_data().conditions)
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
        disjuncts.push_back(as_index(this->self().make_conjunction(std::move(combination))));
    return this->self().make_disjunction(std::move(disjuncts));
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionExists> node)
{
    const auto& data = this->m_storage->repository[node];
    const auto child = as_index(this->self().to_dnf(data.condition));
    if (const auto child_or = this->self().as_or(child))
    {
        auto parts = ygg::IndexList<formalism::Condition> {};
        for (auto nested : child_or->get_data().conditions)
        {
            const auto exists = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(
                this->m_storage->repository, data.parameters, nested).get_index()));
            parts.push_back(as_index(this->self().flatten_condition(exists)));
        }
        return this->self().to_dnf(as_index(this->self().make_disjunction(std::move(parts))));
    }
    const auto exists = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(
        this->m_storage->repository, data.parameters, child).get_index()));
    return this->self().flatten_condition(exists);
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionForall> node)
{
    const auto& data = this->m_storage->repository[node];
    const auto child = as_index(this->self().to_dnf(data.condition));
    if (const auto child_or = this->self().as_or(child))
    {
        auto parts = ygg::IndexList<formalism::Condition> {};
        for (auto nested : child_or->get_data().conditions)
        {
            const auto forall = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(
                this->m_storage->repository, data.parameters, nested).get_index()));
            parts.push_back(as_index(this->self().flatten_condition(forall)));
        }
        return this->self().to_dnf(as_index(this->self().make_disjunction(std::move(parts))));
    }
    const auto forall = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(
        this->m_storage->repository, data.parameters, child).get_index()));
    return this->self().flatten_condition(forall);
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>)
{
    return ygg::make_view(condition, this->m_storage->repository);
}

} // namespace loki::semantic::detail

#endif
