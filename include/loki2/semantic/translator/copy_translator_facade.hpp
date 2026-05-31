/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class CopyTranslatorFacade : public CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>
{
public:
    explicit CopyTranslatorFacade(CopyContext& context) : CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>(context) {}

    cista::optional<ygg::Index<pddl::ConditionOr>> public_as_or(ygg::Index<pddl::Condition> condition) const;

    ygg::IndexList<pddl::Action> split_disjunctive_actions(const ygg::IndexList<pddl::Action>& actions);

    ygg::IndexList<pddl::Axiom> split_disjunctive_axioms(const ygg::IndexList<pddl::Axiom>& axioms);








    pddl::DomainView copy_domain(pddl::DomainView domain);

    pddl::TaskView copy_task(pddl::TaskView task);};

template<typename Derived>
cista::optional<ygg::Index<pddl::ConditionOr>> CopyTranslatorFacade<Derived>::public_as_or(ygg::Index<pddl::Condition> condition) const
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
ygg::IndexList<pddl::Action> CopyTranslatorFacade<Derived>::split_disjunctive_actions(const ygg::IndexList<pddl::Action>& actions)
{
    auto result = ygg::IndexList<pddl::Action> {};
    for (auto action : actions)
    {
        const auto data = this->m_storage->repository[action];
        if (data.precondition)
        {
            if (const auto condition_or = this->self().public_as_or(*data.precondition))
            {
                for (auto part : this->m_storage->repository[*condition_or].conditions)
                    result.push_back(this->m_storage->builder.action(this->m_storage->repository, data.name, data.parameters, part, data.effect).get_index());
                continue;
            }
        }
        result.push_back(action);
    }
    return result;
}

template<typename Derived>
ygg::IndexList<pddl::Axiom> CopyTranslatorFacade<Derived>::split_disjunctive_axioms(const ygg::IndexList<pddl::Axiom>& axioms)
{
    auto result = ygg::IndexList<pddl::Axiom> {};
    for (auto axiom : axioms)
    {
        const auto data = this->m_storage->repository[axiom];
        if (const auto condition_or = this->self().public_as_or(data.condition))
        {
            for (auto part : this->m_storage->repository[*condition_or].conditions)
                result.push_back(this->m_storage->builder.axiom(this->m_storage->repository, data.parameters, data.head, part).get_index());
        }
        else
        {
            result.push_back(axiom);
        }
    }
    return result;
}

template<typename Derived>
pddl::DomainView CopyTranslatorFacade<Derived>::copy_domain(pddl::DomainView domain)
{
    this->m_storage->original_domain = domain.get_index();
    auto data = domain.get_data();
    data.index = {};
    data.requirements = this->self().template copy_list<pddl::Requirement>(data.requirements, domain.get_context());
    data.types = this->self().template copy_list<pddl::Type>(data.types, domain.get_context());
    data.constants = this->self().template copy_list<pddl::Object>(data.constants, domain.get_context());
    data.predicates = this->self().template copy_list<pddl::Predicate>(data.predicates, domain.get_context());
    data.functions = this->self().template copy_list<pddl::FunctionSkeleton>(data.functions, domain.get_context());
    data.actions = this->self().split_disjunctive_actions(this->self().template copy_list<pddl::Action>(data.actions, domain.get_context()));
    data.axioms = this->self().split_disjunctive_axioms(this->self().template copy_list<pddl::Axiom>(data.axioms, domain.get_context()));
    this->self().add_type_predicates_to_domain(data);
    this->self().append_generated_domain_objects(data);
    auto view = this->m_storage->builder.domain(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view.get_index();
    return view;
}

template<typename Derived>
pddl::TaskView CopyTranslatorFacade<Derived>::copy_task(pddl::TaskView task)
{
    auto data = task.get_data();
    this->m_append_generated_axioms_to_domain = false;
    data.index = {};
    data.domain = this->m_storage->translated_domain;
    data.requirements = this->self().template copy_list<pddl::Requirement>(data.requirements, task.get_context());
    data.objects = this->self().template copy_list<pddl::Object>(data.objects, task.get_context());
    data.initial_literals = this->self().template copy_list<pddl::Literal>(data.initial_literals, task.get_context());
    data.initial_function_values = this->self().template copy_list<pddl::InitialFunctionValue>(data.initial_function_values, task.get_context());
    data.goal = this->self().template copy_optional<pddl::Condition>(data.goal, task.get_context());
    data.metric = this->self().template copy_optional<pddl::Metric>(data.metric, task.get_context());
    data.axioms = this->self().split_disjunctive_axioms(this->self().template copy_list<pddl::Axiom>(data.axioms, task.get_context()));
    if (data.goal)
        data.goal = this->self().simplify_goal_condition(*data.goal);
    for (auto axiom : this->m_generated_axioms)
        data.axioms.push_back(axiom);
    if (!this->m_generated_predicates.empty() || !this->m_generated_axioms.empty())
    {
        this->self().update_translated_domain();
        data.domain = this->m_storage->translated_domain;
    }
    this->self().initialize_type_literals(data);
    this->self().initialize_equality(data);
    auto view = this->m_storage->builder.task(this->m_storage->repository, std::move(data));
    remember(this->m_storage->tasks, task.get_index(), view.get_index());
    return view;
}

} // namespace loki2::semantic::detail

#endif
