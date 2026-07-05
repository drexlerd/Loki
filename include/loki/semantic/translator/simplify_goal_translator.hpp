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

#ifndef LOKI_SEMANTIC_TRANSLATOR_SIMPLIFY_GOAL_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_SIMPLIFY_GOAL_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class SimplifyGoalTranslator : public CopyTranslatorComponent<Derived, SimplifyGoalTranslator<Derived>>
{
public:
    explicit SimplifyGoalTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, SimplifyGoalTranslator<Derived>>(context) {}

    formalism::ConditionView make_generated_goal_condition(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView simplify_goal_condition(formalism::ConditionView condition);
    formalism::ConditionView simplify_goal_condition_node(formalism::ConditionView condition, formalism::ConditionLiteralView);
    formalism::ConditionView simplify_goal_condition_node(formalism::ConditionView condition, formalism::ConditionNumericConstraintView);
    formalism::ConditionView simplify_goal_condition_node(formalism::ConditionView, formalism::ConditionAndView node);
    template<typename T>
    formalism::ConditionView simplify_goal_condition_node(formalism::ConditionView condition, T);
};

template<typename Derived>
formalism::ConditionView SimplifyGoalTranslator<Derived>::make_generated_goal_condition(ygg::Index<formalism::Condition> condition)
{
    const auto name = cista::offset::string(this->self().next_generated_predicate_name("loki-goal-"));
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, ygg::IndexList<formalism::Parameter> {});
    const auto atom =
        formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate.get_index(), ygg::IndexList<formalism::Term> {}).get_index();
    const auto literal = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, atom, true).get_index();
    const auto axiom = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, ygg::IndexList<formalism::Parameter> {}, literal, condition);
    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, literal));
}

template<typename Derived>
formalism::ConditionView SimplifyGoalTranslator<Derived>::simplify_goal_condition(formalism::ConditionView condition)
{
    return ygg::visit([&](const auto& node) { return this->self().simplify_goal_condition_node(condition, node); }, condition.get_value());
}

template<typename Derived>
formalism::ConditionView SimplifyGoalTranslator<Derived>::simplify_goal_condition_node(formalism::ConditionView condition, formalism::ConditionLiteralView)
{
    return condition;
}

template<typename Derived>
formalism::ConditionView SimplifyGoalTranslator<Derived>::simplify_goal_condition_node(formalism::ConditionView condition,
                                                                                       formalism::ConditionNumericConstraintView)
{
    return condition;
}

template<typename Derived>
formalism::ConditionView SimplifyGoalTranslator<Derived>::simplify_goal_condition_node(formalism::ConditionView, formalism::ConditionAndView node)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
        conditions.push_back(this->self().simplify_goal_condition(child).get_index());
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
template<typename T>
formalism::ConditionView SimplifyGoalTranslator<Derived>::simplify_goal_condition_node(formalism::ConditionView condition, T)
{
    return this->self().make_generated_goal_condition(condition.get_index());
}

}  // namespace loki::semantic::detail

#endif
