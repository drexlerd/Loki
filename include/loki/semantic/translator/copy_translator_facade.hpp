/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class CopyTranslatorFacade : public CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>
{
public:
    explicit CopyTranslatorFacade(CopyContext& context) : CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>(context) {}

    cista::optional<ygg::Index<formalism::ConditionOr>> public_as_or(ygg::Index<formalism::Condition> condition) const;

    ygg::IndexList<formalism::Action> split_disjunctive_actions(const ygg::IndexList<formalism::Action>& actions);

    ygg::IndexList<formalism::Axiom> split_disjunctive_axioms(const ygg::IndexList<formalism::Axiom>& axioms);








    formalism::DomainView copy_domain(formalism::DomainView domain);

    formalism::TaskView copy_task(formalism::TaskView task);};

template<typename Derived>
cista::optional<ygg::Index<formalism::ConditionOr>> CopyTranslatorFacade<Derived>::public_as_or(ygg::Index<formalism::Condition> condition) const
{
    auto result = cista::optional<ygg::Index<formalism::ConditionOr>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
            result = node;
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Action> CopyTranslatorFacade<Derived>::split_disjunctive_actions(const ygg::IndexList<formalism::Action>& actions)
{
    auto result = ygg::IndexList<formalism::Action> {};
    for (auto action : actions)
    {
        const auto data = this->m_storage->repository[action];
        if (data.precondition)
        {
            if (const auto condition_or = this->self().public_as_or(*data.precondition))
            {
                for (auto part : this->m_storage->repository[*condition_or].conditions)
                    result.push_back(formalism::get_or_create<formalism::Action>(this->m_storage->repository, data.name, data.parameters, part, data.effect).get_index());
                continue;
            }
        }
        result.push_back(action);
    }
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Axiom> CopyTranslatorFacade<Derived>::split_disjunctive_axioms(const ygg::IndexList<formalism::Axiom>& axioms)
{
    auto result = ygg::IndexList<formalism::Axiom> {};
    for (auto axiom : axioms)
    {
        const auto data = this->m_storage->repository[axiom];
        if (const auto condition_or = this->self().public_as_or(data.condition))
        {
            for (auto part : this->m_storage->repository[*condition_or].conditions)
                result.push_back(formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, data.parameters, data.head, part).get_index());
        }
        else
        {
            result.push_back(axiom);
        }
    }
    return result;
}

template<typename Derived>
formalism::DomainView CopyTranslatorFacade<Derived>::copy_domain(formalism::DomainView domain)
{
    this->m_storage->original_domain = domain.get_index();
    auto data = domain.get_data();
    data.index = {};
    data.requirements = this->self().template copy_list<formalism::Requirement>(data.requirements, domain.get_context());
    data.types = this->self().template copy_list<formalism::Type>(data.types, domain.get_context());
    data.constants = this->self().template copy_list<formalism::Object>(data.constants, domain.get_context());
    data.predicates = this->self().template copy_list<formalism::Predicate>(data.predicates, domain.get_context());
    data.functions = this->self().template copy_list<formalism::FunctionSkeleton>(data.functions, domain.get_context());
    data.actions = this->self().split_disjunctive_actions(this->self().template copy_list<formalism::Action>(data.actions, domain.get_context()));
    data.axioms = this->self().split_disjunctive_axioms(this->self().template copy_list<formalism::Axiom>(data.axioms, domain.get_context()));
    this->self().add_type_predicates_to_domain(data);
    this->self().append_generated_domain_objects(data);
    auto view = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view.get_index();
    return view;
}

template<typename Derived>
formalism::TaskView CopyTranslatorFacade<Derived>::copy_task(formalism::TaskView task)
{
    auto data = task.get_data();
    this->m_append_generated_axioms_to_domain = false;
    data.index = {};
    data.domain = this->m_storage->translated_domain;
    data.requirements = this->self().template copy_list<formalism::Requirement>(data.requirements, task.get_context());
    data.objects = this->self().template copy_list<formalism::Object>(data.objects, task.get_context());
    data.initial_literals = this->self().template copy_list<formalism::Literal>(data.initial_literals, task.get_context());
    data.initial_function_values = this->self().template copy_list<formalism::InitialFunctionValue>(data.initial_function_values, task.get_context());
    data.goal = this->self().template copy_optional<formalism::Condition>(data.goal, task.get_context());
    data.metric = this->self().template copy_optional<formalism::Metric>(data.metric, task.get_context());
    data.axioms = this->self().split_disjunctive_axioms(this->self().template copy_list<formalism::Axiom>(data.axioms, task.get_context()));
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
    auto view = formalism::get_or_create<formalism::Task>(this->m_storage->repository, std::move(data));
    remember(this->m_storage->tasks, task.get_index(), view.get_index());
    return view;
}

} // namespace loki::semantic::detail

#endif
