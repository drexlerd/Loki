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

    std::optional<formalism::ConditionOrView> public_as_or(ygg::Index<formalism::Condition> condition) const;

    template<typename T>
    void push_unique(ygg::IndexList<T>& list, std::unordered_set<ygg::uint_t>& seen, ygg::Index<T> value);

    size_t next_generated_axiom_index() const;

    ygg::IndexList<formalism::Action> split_disjunctive_actions(const ygg::IndexList<formalism::Action>& actions);

    ygg::IndexList<formalism::Axiom> split_disjunctive_axioms(const ygg::IndexList<formalism::Axiom>& axioms);

    formalism::DomainView copy_domain(formalism::DomainView domain);

    formalism::TaskView copy_task(formalism::TaskView task);
};

template<typename Derived>
std::optional<formalism::ConditionOrView> CopyTranslatorFacade<Derived>::public_as_or(ygg::Index<formalism::Condition> condition) const
{
    auto result = std::optional<formalism::ConditionOrView> {};
    std::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionOr>>)
                result = ygg::make_view(node, this->m_storage->repository);
        },
        this->m_storage->repository[condition].value);
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
    const auto prefixes = std::array<std::string_view, 2> { "_universal_", "_goal_" };

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
        if (data.precondition)
        {
            const auto precondition = as_index(this->self().flatten_condition(*data.precondition));
            if (const auto condition_or = this->self().public_as_or(precondition))
            {
                for (auto part : condition_or->get_data().conditions)
                    this->self().push_unique(result,
                                             seen,
                                             formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                                                         data.name,
                                                                                         data.parameters,
                                                                                         data.original_arity,
                                                                                         part,
                                                                                         data.effect)
                                                 .get_index());
                continue;
            }
        }
        this->self().push_unique(result, seen, action);
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
        const auto condition = as_index(this->self().flatten_condition(data.condition));
        if (const auto condition_or = this->self().public_as_or(condition))
        {
            for (auto part : condition_or->get_data().conditions)
                this->self().push_unique(
                    result,
                    seen,
                    formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, data.parameters, data.original_arity, data.head, part).get_index());
        }
        else
        {
            this->self().push_unique(result, seen, axiom);
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
    this->m_num_generated_axioms = this->self().next_generated_axiom_index();
    data.functions = this->self().template copy_list<formalism::FunctionSkeleton>(data.functions, domain.get_context());
    data.actions = this->self().template copy_list<formalism::Action>(data.actions, domain.get_context());
    data.axioms = this->self().template copy_list<formalism::Axiom>(data.axioms, domain.get_context());

    switch (this->m_phase)
    {
        case TranslationPhase::RemoveUniversalQuantifiers:
            this->self().append_generated_domain_objects(data);
            if (!data.axioms.empty())
                this->self().ensure_derived_predicates_requirement(data.requirements);
            break;
        case TranslationPhase::SplitDisjunctiveConditions:
            data.actions = this->self().split_disjunctive_actions(data.actions);
            data.axioms = this->self().split_disjunctive_axioms(data.axioms);
            break;
        case TranslationPhase::AddTypePredicates:
            this->self().add_type_predicates_to_domain(data);
            break;
        case TranslationPhase::InitializeEquality:
            this->self().add_equality_predicate_to_domain(data);
            break;
        default:
            break;
    }

    auto view = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view;
    remember(this->m_storage->domains, domain.get_index(), view);
    return view;
}

template<typename Derived>
formalism::TaskView CopyTranslatorFacade<Derived>::copy_task(formalism::TaskView task)
{
    auto data = task.get_data();
    this->m_append_generated_axioms_to_domain = false;
    this->m_num_generated_axioms = this->self().next_generated_axiom_index();
    data.index = {};
    data.domain = this->m_storage->translated_domain->get_index();
    data.requirements = this->self().template copy_list<formalism::Requirement>(data.requirements, task.get_context());
    data.objects = this->self().template copy_list<formalism::Object>(data.objects, task.get_context());
    data.initial_literals = this->self().template copy_list<formalism::Literal>(data.initial_literals, task.get_context());
    data.initial_function_values = this->self().template copy_list<formalism::InitialFunctionValue>(data.initial_function_values, task.get_context());

    if (data.goal)
    {
        if (this->m_phase == TranslationPhase::RenameQuantifiedVariables)
        {
            this->m_num_quantifications.clear();
            this->self().enter_variable_scope();
            const auto renamed_goal = as_index(this->self().rename_variables(*data.goal, task.get_context()));
            this->self().leave_variable_scope();
            const auto previous = this->m_renaming_enabled;
            this->m_renaming_enabled = false;
            data.goal = as_index(this->self().copy(renamed_goal, this->m_storage->repository));
            this->m_renaming_enabled = previous;
        }
        else
        {
            data.goal = as_index(this->self().copy(*data.goal, task.get_context()));
        }
    }

    data.metric = this->self().template copy_optional<formalism::Metric>(data.metric, task.get_context());
    data.predicates = this->self().template copy_list<formalism::Predicate>(data.predicates, task.get_context());
    data.axioms = this->self().template copy_list<formalism::Axiom>(data.axioms, task.get_context());

    switch (this->m_phase)
    {
        case TranslationPhase::RemoveUniversalQuantifiers:
        case TranslationPhase::SimplifyGoal:
        {
            if (this->m_phase == TranslationPhase::SimplifyGoal && data.goal)
                data.goal = as_index(this->self().simplify_goal_condition(*data.goal));

            auto existing_predicates = std::unordered_set<ygg::uint_t> {};
            for (auto predicate : data.predicates)
                existing_predicates.insert(predicate.get_value());
            for (auto predicate : this->m_generated_predicates)
                if (existing_predicates.insert(predicate.get_value()).second)
                    data.predicates.push_back(predicate);

            auto existing_axioms = std::unordered_set<ygg::uint_t> {};
            for (auto axiom : data.axioms)
                existing_axioms.insert(axiom.get_value());
            for (auto axiom : this->m_generated_axioms)
                if (existing_axioms.insert(axiom.get_value()).second)
                    data.axioms.push_back(axiom);

            if (!data.axioms.empty())
                this->self().ensure_derived_predicates_requirement(data.requirements);
            break;
        }
        case TranslationPhase::SplitDisjunctiveConditions:
            data.axioms = this->self().split_disjunctive_axioms(data.axioms);
            break;
        case TranslationPhase::InitializeEquality:
            this->self().initialize_equality(data);
            break;
        case TranslationPhase::AddTypePredicates:
            this->self().initialize_type_literals(data);
            break;
        default:
            break;
    }

    auto view = formalism::get_or_create<formalism::Task>(this->m_storage->repository, std::move(data));
    remember(this->m_storage->tasks, task.get_index(), view);
    return view;
}

}  // namespace loki::semantic::detail

#endif
