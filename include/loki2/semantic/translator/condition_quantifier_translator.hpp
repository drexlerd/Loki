/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_CONDITION_QUANTIFIER_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_CONDITION_QUANTIFIER_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class ConditionQuantifierTranslator : public CopyTranslatorComponent<Derived, ConditionQuantifierTranslator<Derived>>
{
public:
    explicit ConditionQuantifierTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionQuantifierTranslator<Derived>>(context) {}

    ygg::Index<pddl::Condition> make_generated_axiom_condition(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> make_generated_goal_condition(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> simplify_goal_condition(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>);
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node);
    template<typename T>
    ygg::Index<pddl::Condition> simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>);
    cista::optional<ygg::Index<pddl::ConditionExists>> as_exists(ygg::Index<pddl::Condition> condition) const;
    ygg::Index<pddl::Condition> move_existentials(ygg::Index<pddl::Condition> condition);
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>);
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>);
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node);
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionExists> node);
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionOr> node);
    template<typename T>
    ygg::Index<pddl::Condition> move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>);
    void lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, cista::optional<ygg::Index<pddl::Condition>>& condition);
    void lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, ygg::Index<pddl::Condition>& condition);
};

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::make_generated_axiom_condition(ygg::Index<pddl::Condition> condition)
{
    const auto free_parameters = this->self().free_parameters_in_scope(condition);
    auto predicate_parameters = ygg::IndexList<pddl::Parameter> {};
    auto terms = ygg::IndexList<pddl::Term> {};
    for (auto parameter : free_parameters)
    {
        predicate_parameters.push_back(parameter);
        terms.push_back(this->self().term_from_variable(this->m_storage->repository[parameter].variable));
    }

    const auto name = cista::offset::string("_universal_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate = this->m_storage->builder.predicate(this->m_storage->repository, name, predicate_parameters).get_index();
    const auto atom = this->m_storage->builder.atom(this->m_storage->repository, predicate, terms).get_index();
    const auto positive_head = this->m_storage->builder.literal(this->m_storage->repository, true, atom).get_index();
    const auto negative_literal = this->m_storage->builder.literal(this->m_storage->repository, false, atom).get_index();
    const auto axiom = this->m_storage->builder.axiom(this->m_storage->repository, free_parameters, positive_head, condition).get_index();
    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    return this->self().wrap_condition(this->m_storage->builder.condition_literal(this->m_storage->repository, negative_literal).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::make_generated_goal_condition(ygg::Index<pddl::Condition> condition)
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
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::simplify_goal_condition(ygg::Index<pddl::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().simplify_goal_condition_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
{
    auto conditions = ygg::IndexList<pddl::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
        conditions.push_back(this->self().simplify_goal_condition(child));
    return this->self().wrap_condition(this->m_storage->builder.condition_and(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
template<typename T>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::simplify_goal_condition_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
{
    return this->self().make_generated_goal_condition(condition);
}

template<typename Derived>
cista::optional<ygg::Index<pddl::ConditionExists>> ConditionQuantifierTranslator<Derived>::as_exists(ygg::Index<pddl::Condition> condition) const
{
    auto result = cista::optional<ygg::Index<pddl::ConditionExists>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<pddl::ConditionExists>>)
            result = node;
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials(ygg::Index<pddl::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().move_existentials_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionLiteral>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<pddl::ConditionNumericConstraint>) { return condition; }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionAnd> node)
{
    auto parameters = ygg::IndexList<pddl::Parameter> {};
    auto parts = ygg::IndexList<pddl::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto moved = this->self().move_existentials(child);
        if (const auto exists = this->self().as_exists(moved))
        {
            const auto& exists_data = this->m_storage->repository[*exists];
            for (auto parameter : exists_data.parameters)
                parameters.push_back(parameter);
            parts.push_back(exists_data.condition);
        }
        else
        {
            parts.push_back(moved);
        }
    }
    auto conjunction = this->self().wrap_condition(this->m_storage->builder.condition_and(this->m_storage->repository, std::move(parts)).get_index());
    if (parameters.empty())
        return conjunction;
    return this->self().wrap_condition(this->m_storage->builder.condition_exists(this->m_storage->repository, std::move(parameters), conjunction).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionExists> node)
{
    const auto& data = this->m_storage->repository[node];
    return this->self().wrap_condition(this->m_storage->builder.condition_exists(this->m_storage->repository, data.parameters, this->self().move_existentials(data.condition)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition>, ygg::Index<pddl::ConditionOr> node)
{
    auto parts = ygg::IndexList<pddl::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
        parts.push_back(this->self().move_existentials(child));
    return this->self().wrap_condition(this->m_storage->builder.condition_or(this->m_storage->repository, std::move(parts)).get_index());
}

template<typename Derived>
template<typename T>
ygg::Index<pddl::Condition> ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<pddl::Condition> condition, ygg::Index<T>)
{
    return condition;
}

template<typename Derived>
void ConditionQuantifierTranslator<Derived>::lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, cista::optional<ygg::Index<pddl::Condition>>& condition)
{
    if (!condition)
        return;
    auto moved = this->self().move_existentials(*condition);
    if (const auto exists = this->self().as_exists(moved))
    {
        const auto& data = this->m_storage->repository[*exists];
        for (auto parameter : data.parameters)
            parameters.push_back(parameter);
        moved = data.condition;
    }
    condition = moved;
}

template<typename Derived>
void ConditionQuantifierTranslator<Derived>::lift_top_level_exists(ygg::IndexList<pddl::Parameter>& parameters, ygg::Index<pddl::Condition>& condition)
{
    auto optional = cista::optional<ygg::Index<pddl::Condition>>(condition);
    this->self().lift_top_level_exists(parameters, optional);
    condition = *optional;
}

} // namespace loki2::semantic::detail

#endif
