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


#ifndef LOKI_SEMANTIC_TRANSLATOR_GOAL_SIMPLIFICATION_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_GOAL_SIMPLIFICATION_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class GoalSimplificationTranslator : public CopyTranslatorComponent<Derived, GoalSimplificationTranslator<Derived>>
{
public:
    explicit GoalSimplificationTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, GoalSimplificationTranslator<Derived>>(context) {}

    formalism::ConditionView make_generated_goal_condition(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView simplify_goal_condition(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView simplify_goal_condition_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>);
    formalism::ConditionView simplify_goal_condition_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>);
    formalism::ConditionView simplify_goal_condition_node(ygg::Index<formalism::Condition>, ygg::Index<formalism::ConditionAnd> node);
    template<typename T>
    formalism::ConditionView simplify_goal_condition_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>);
};

template<typename Derived>
formalism::ConditionView GoalSimplificationTranslator<Derived>::make_generated_goal_condition(ygg::Index<formalism::Condition> condition)
{
    const auto name = cista::offset::string("_goal_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate =
        formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, ygg::IndexList<formalism::Parameter> {}).get_index();
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate, ygg::IndexList<formalism::Term> {}).get_index();
    const auto literal = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, atom, true).get_index();
    const auto axiom =
        formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, ygg::IndexList<formalism::Parameter> {}, literal, condition).get_index();
    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, literal).get_index());
}

template<typename Derived>
formalism::ConditionView GoalSimplificationTranslator<Derived>::simplify_goal_condition(ygg::Index<formalism::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().simplify_goal_condition_node(condition, node); },
                      this->m_storage->repository[condition].value);
}

template<typename Derived>
formalism::ConditionView GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<formalism::Condition> condition,
                                                                                             ygg::Index<formalism::ConditionLiteral>)
{
    return ygg::make_view(condition, this->m_storage->repository);
}

template<typename Derived>
formalism::ConditionView GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<formalism::Condition> condition,
                                                                                             ygg::Index<formalism::ConditionNumericConstraint>)
{
    return ygg::make_view(condition, this->m_storage->repository);
}

template<typename Derived>
formalism::ConditionView GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<formalism::Condition>,
                                                                                             ygg::Index<formalism::ConditionAnd> node)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
        conditions.push_back(as_index(this->self().simplify_goal_condition(child)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
template<typename T>
formalism::ConditionView GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>)
{
    return this->self().make_generated_goal_condition(condition);
}

}  // namespace loki::semantic::detail

#endif
