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

    formalism::ConditionView to_dnf(formalism::ConditionView condition);
    formalism::ConditionView to_dnf_node(formalism::ConditionView, formalism::ConditionOrView node);
    formalism::ConditionView to_dnf_node(formalism::ConditionView, formalism::ConditionAndView node);
    formalism::ConditionView to_dnf_node(formalism::ConditionView, formalism::ConditionExistsView node);
    formalism::ConditionView to_dnf_node(formalism::ConditionView, formalism::ConditionForallView node);
    template<typename T>
    formalism::ConditionView to_dnf_node(formalism::ConditionView condition, T);
};

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf(formalism::ConditionView condition)
{
    return ygg::visit([&](const auto& node) { return this->self().to_dnf_node(condition, node); }, condition.get_value());
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(formalism::ConditionView, formalism::ConditionOrView node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
    {
        const auto dnf = this->self().to_dnf(child);
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : child_or->get_data().conditions)
                parts.push_back(nested);
        }
        else
        {
            parts.push_back(dnf.get_index());
        }
    }
    return this->self().make_disjunction(std::move(parts));
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(formalism::ConditionView, formalism::ConditionAndView node)
{
    auto combinations = std::vector<ygg::IndexList<formalism::Condition>> { ygg::IndexList<formalism::Condition> {} };
    for (auto child : node.get_conditions())
    {
        const auto dnf = this->self().to_dnf(child);
        auto alternatives = ygg::IndexList<formalism::Condition> {};
        if (const auto child_or = this->self().as_or(dnf))
        {
            for (auto nested : child_or->get_data().conditions)
                alternatives.push_back(nested);
        }
        else
        {
            alternatives.push_back(dnf.get_index());
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
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(formalism::ConditionView, formalism::ConditionExistsView node)
{
    const auto& data = node.get_data();
    const auto child = this->self().to_dnf(node.get_condition());
    if (const auto child_or = this->self().as_or(child))
    {
        auto parts = ygg::IndexList<formalism::Condition> {};
        for (auto nested : child_or->get_data().conditions)
        {
            const auto exists =
                this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, nested));
            parts.push_back(as_index(this->self().flatten_condition(exists)));
        }
        return this->self().to_dnf(this->self().make_disjunction(std::move(parts)));
    }
    const auto exists =
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, child.get_index()));
    return this->self().flatten_condition(exists);
}

template<typename Derived>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(formalism::ConditionView, formalism::ConditionForallView node)
{
    const auto& data = node.get_data();
    const auto child = this->self().to_dnf(node.get_condition());
    if (const auto child_or = this->self().as_or(child))
    {
        auto parts = ygg::IndexList<formalism::Condition> {};
        for (auto nested : child_or->get_data().conditions)
        {
            const auto forall =
                this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, data.parameters, nested));
            parts.push_back(as_index(this->self().flatten_condition(forall)));
        }
        return this->self().to_dnf(this->self().make_disjunction(std::move(parts)));
    }
    const auto forall =
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, data.parameters, child.get_index()));
    return this->self().flatten_condition(forall);
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionDnfTranslator<Derived>::to_dnf_node(formalism::ConditionView condition, T)
{
    return condition;
}

}  // namespace loki::semantic::detail

#endif
