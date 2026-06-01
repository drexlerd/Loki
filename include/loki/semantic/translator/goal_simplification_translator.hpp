/*
 * Copyright (C) 2026 Dominik Drexler
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
    const auto predicate = pddl::get_or_create<pddl::Predicate>(this->m_storage->repository, name, ygg::IndexList<pddl::Parameter> {}).get_index();
    const auto atom = pddl::get_or_create<pddl::Atom>(this->m_storage->repository, predicate, ygg::IndexList<pddl::Term> {}).get_index();
    const auto literal = pddl::get_or_create<pddl::Literal>(this->m_storage->repository, true, atom).get_index();
    const auto axiom = pddl::get_or_create<pddl::Axiom>(this->m_storage->repository, ygg::IndexList<pddl::Parameter> {}, literal, condition).get_index();
    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    return this->self().wrap_condition(pddl::get_or_create<pddl::ConditionLiteral>(this->m_storage->repository, literal).get_index());
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
    return this->self().wrap_condition(pddl::get_or_create<pddl::ConditionAnd>(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
template<typename T>
ygg::Index<pddl::Condition> GoalSimplificationTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
{
    return this->self().make_generated_goal_condition(condition);
}

} // namespace loki::semantic::detail

#endif
