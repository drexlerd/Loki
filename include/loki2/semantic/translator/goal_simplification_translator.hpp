/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_GOAL_SIMPLIFICATION_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_GOAL_SIMPLIFICATION_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class GoalSimplificationTranslator : public CopyTranslatorComponent<Derived, GoalSimplificationTranslator<Derived>>
{
public:
    explicit GoalSimplificationTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, GoalSimplificationTranslator<Derived>>(context) {}

    ygg::Index<pddl::Condition> make_generated_goal_condition(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> simplify_goal_condition(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node);
    template<typename T>
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>);
};

template<typename Derived>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::make_generated_goal_condition(ygg::Index<pddl::Condition> condition)
{
    const auto name = cista::offset::string("_goal_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate = this->m_storage->builder.predicate(this->m_storage->repository, name, {}).get_index();
    const auto atom = this->m_storage->builder.atom(this->m_storage->repository, predicate, {}).get_index();
    const auto literal = this->m_storage->builder.literal(this->m_storage->repository, true, atom).get_index();
    const auto axiom = this->m_storage->builder.axiom(this->m_storage->repository, {}, literal, condition).get_index();
    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    return this->self().wrap_condition(this->m_storage->builder.condition_literal(this->m_storage->repository, literal).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition(ygg::Index<pddl::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().simplify_goal_condition_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
{
    auto conditions = ygg::IndexList<pddl::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
        conditions.push_back(this->self().simplify_goal_condition(child));
    return this->self().wrap_condition(this->m_storage->builder.condition_and(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
template<typename T>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
{
    return this->self().make_generated_goal_condition(condition);
}

} // namespace loki2::semantic::detail

#endif
