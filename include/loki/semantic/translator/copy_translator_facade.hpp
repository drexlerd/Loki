/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_FACADE_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <algorithm>
#include <array>

namespace loki::semantic::detail
{

template<typename Derived>
class CopyTranslatorFacade : public CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>
{
public:
    explicit CopyTranslatorFacade(CopyContext& context) : CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>(context) {}

    cista::optional<ygg::Index<formalism::ConditionOr>> public_as_or(ygg::Index<formalism::Condition> condition) const;

    template<typename T>
    void push_unique(ygg::IndexList<T>& list, std::unordered_set<ygg::uint_t>& seen, ygg::Index<T> value);

    size_t next_generated_axiom_index() const;

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
template<typename T>
void CopyTranslatorFacade<Derived>::push_unique(ygg::IndexList<T>& list, std::unordered_set<ygg::uint_t>& seen, ygg::Index<T> value)
{
    if (seen.insert(value.get_value()).second)
        list.push_back(value);
}

template<typename Derived>
size_t CopyTranslatorFacade<Derived>::next_generated_axiom_index() const
{
    auto next = this->m_num_generated_axioms;
    const auto prefixes = std::array<std::string_view, 3> { "_universal_", "_condition_", "_goal_" };

    for (auto i = ygg::uint_t { 0 }; i < this->m_storage->repository.template size<formalism::Predicate>(); ++i)
    {
        const auto name = std::string_view(this->m_storage->repository[ygg::Index<formalism::Predicate>(i)].name);
        for (const auto prefix : prefixes)
        {
            if (!name.starts_with(prefix))
                continue;

            auto value = size_t { 0 };
            auto valid = name.size() > prefix.size();
            for (auto c : name.substr(prefix.size()))
            {
                if (c < '0' || c > '9')
                {
                    valid = false;
                    break;
                }
                value = value * 10 + static_cast<size_t>(c - '0');
            }
            if (valid)
                next = std::max(next, value + 1);
        }
    }

    return next;
}

template<typename Derived>
ygg::IndexList<formalism::Action> CopyTranslatorFacade<Derived>::split_disjunctive_actions(const ygg::IndexList<formalism::Action>& actions)
{
    auto result = ygg::IndexList<formalism::Action> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    for (auto action : actions)
    {
        const auto data = this->m_storage->repository[action];
        auto precondition = data.precondition;
        if (precondition)
        {
            precondition = as_index(this->self().flatten_condition(as_index(this->self().to_dnf(*precondition))));
            if (const auto condition_or = this->self().public_as_or(*precondition))
            {
                for (auto part : this->m_storage->repository[*condition_or].conditions)
                {
                    auto parameters = data.parameters;
                    auto condition = part;
                    this->self().lift_top_level_exists(parameters, condition);
                    this->self().push_unique(result, seen, formalism::get_or_create<formalism::Action>(this->m_storage->repository, data.name, this->self().maybe_strip_parameters(parameters), condition, data.effect).get_index());
                }
                continue;
            }
        }
        if (precondition)
        {
            auto parameters = data.parameters;
            this->self().lift_top_level_exists(parameters, precondition);
            this->self().push_unique(result, seen, formalism::get_or_create<formalism::Action>(this->m_storage->repository, data.name, this->self().maybe_strip_parameters(parameters), precondition, data.effect).get_index());
        }
        else
        {
            this->self().push_unique(result, seen, action);
        }
    }
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Axiom> CopyTranslatorFacade<Derived>::split_disjunctive_axioms(const ygg::IndexList<formalism::Axiom>& axioms)
{
    auto result = ygg::IndexList<formalism::Axiom> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    for (auto axiom : axioms)
    {
        const auto data = this->m_storage->repository[axiom];
        const auto condition = as_index(this->self().flatten_condition(as_index(this->self().to_dnf(data.condition))));
        if (const auto condition_or = this->self().public_as_or(condition))
        {
            for (auto part : this->m_storage->repository[*condition_or].conditions)
            {
                auto parameters = data.parameters;
                auto part_condition = part;
                this->self().lift_top_level_exists(parameters, part_condition);
                this->self().push_unique(result, seen, formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), data.head, part_condition).get_index());
            }
        }
        else
        {
            auto parameters = data.parameters;
            auto lifted_condition = condition;
            this->self().lift_top_level_exists(parameters, lifted_condition);
            this->self().push_unique(result, seen, formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), data.head, lifted_condition).get_index());
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
    this->self().add_equality_predicate_to_domain(data);
    this->self().append_generated_domain_objects(data);
    auto view = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view.get_index();
    return view;
}

template<typename Derived>
formalism::TaskView CopyTranslatorFacade<Derived>::copy_task(formalism::TaskView task)
{
    auto data = task.get_data();
    const auto first_task_generated_predicate = this->m_generated_predicates.size();
    const auto first_task_generated_axiom = this->m_generated_axioms.size();
    this->m_append_generated_axioms_to_domain = false;
    this->m_num_generated_axioms = this->self().next_generated_axiom_index();
    data.index = {};
    data.domain = this->m_storage->translated_domain;
    data.requirements = this->self().template copy_list<formalism::Requirement>(data.requirements, task.get_context());
    data.objects = this->self().template copy_list<formalism::Object>(data.objects, task.get_context());
    data.initial_literals = this->self().template copy_list<formalism::Literal>(data.initial_literals, task.get_context());
    data.initial_function_values = this->self().template copy_list<formalism::InitialFunctionValue>(data.initial_function_values, task.get_context());
    data.goal = this->self().template copy_optional<formalism::Condition>(data.goal, task.get_context());
    data.metric = this->self().template copy_optional<formalism::Metric>(data.metric, task.get_context());
    data.predicates = this->self().template copy_list<formalism::Predicate>(data.predicates, task.get_context());
    data.axioms = this->self().split_disjunctive_axioms(this->self().template copy_list<formalism::Axiom>(data.axioms, task.get_context()));
    if (data.goal)
        data.goal = as_index(this->self().simplify_goal_condition(*data.goal));
    auto existing_predicates = std::unordered_set<ygg::uint_t> {};
    for (auto predicate : data.predicates)
        existing_predicates.insert(predicate.get_value());
    for (auto i = first_task_generated_predicate; i < this->m_generated_predicates.size(); ++i)
    {
        auto predicate = this->m_generated_predicates[i];
        if (existing_predicates.insert(predicate.get_value()).second)
            data.predicates.push_back(predicate);
    }

    auto existing_axioms = std::unordered_set<ygg::uint_t> {};
    for (auto axiom : data.axioms)
        existing_axioms.insert(axiom.get_value());
    for (auto i = first_task_generated_axiom; i < this->m_generated_axioms.size(); ++i)
    {
        auto axiom = this->m_generated_axioms[i];
        if (existing_axioms.insert(axiom.get_value()).second)
            data.axioms.push_back(axiom);
    }
    data.axioms = this->self().split_disjunctive_axioms(data.axioms);
    if (!data.predicates.empty() || !data.axioms.empty())
    {
        auto has_derived_requirement = false;
        for (auto requirement : data.requirements)
        {
            if (this->m_storage->repository[requirement].kind == formalism::RequirementKind::DerivedPredicates)
            {
                has_derived_requirement = true;
                break;
            }
        }
        if (!has_derived_requirement)
            data.requirements.push_back(formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, formalism::RequirementKind::DerivedPredicates).get_index());
    }
    this->self().initialize_type_literals(data);
    this->self().initialize_equality(data);
    auto view = formalism::get_or_create<formalism::Task>(this->m_storage->repository, std::move(data));
    remember(this->m_storage->tasks, task.get_index(), view.get_index());
    return view;
}

} // namespace loki::semantic::detail

#endif
