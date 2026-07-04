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

#include "loki/semantic/errors.hpp"
#include "loki/semantic/translator/copy_translator_component.hpp"

#include <algorithm>
#include <limits>
#include <yggdrasil/containers/unordered_set.hpp>

namespace loki::semantic::detail
{

template<typename Derived>
class CopyTranslatorFacade : public CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>
{
public:
    explicit CopyTranslatorFacade(CopyContext& context) : CopyTranslatorComponent<Derived, CopyTranslatorFacade<Derived>>(context) {}

    template<typename T>
    void push_unique(ygg::IndexList<T>& list, std::unordered_set<ygg::uint_t>& seen, ygg::Index<T> value);

    ygg::UnorderedSet<std::string> used_predicate_names() const;

    std::string next_generated_predicate_name(std::string_view prefix);

    ygg::IndexList<formalism::Action> split_disjunctive_actions(formalism::EntityListView<formalism::Action> actions);

    ygg::IndexList<formalism::Action> multiply_conditional_effect_actions(formalism::EntityListView<formalism::Action> actions);

    ygg::IndexList<formalism::Axiom> split_disjunctive_axioms(formalism::EntityListView<formalism::Axiom> axioms);

    formalism::DomainView copy_domain(formalism::DomainView domain);

    formalism::TaskView copy_task(formalism::TaskView task);
};

template<typename Derived>
template<typename T>
void CopyTranslatorFacade<Derived>::push_unique(ygg::IndexList<T>& list, std::unordered_set<ygg::uint_t>& seen, ygg::Index<T> value)
{
    if (seen.insert(value.get_value()).second)
        list.push_back(value);
}

template<typename Derived>
ygg::UnorderedSet<std::string> CopyTranslatorFacade<Derived>::used_predicate_names() const
{
    auto names = this->m_used_predicate_names;
    if (this->m_storage->translated_domain)
    {
        for (auto predicate : this->m_storage->translated_domain->get_predicates())
            names.insert(std::string(predicate.get_name()));
    }
    return names;
}

template<typename Derived>
std::string CopyTranslatorFacade<Derived>::next_generated_predicate_name(std::string_view prefix)
{
    auto used = this->self().used_predicate_names();
    while (true)
    {
        auto name = std::string(prefix) + std::to_string(this->m_num_generated_axioms++);
        if (used.insert(name).second)
        {
            this->m_used_predicate_names.insert(name);
            return name;
        }
    }
}

template<typename Derived>
ygg::IndexList<formalism::Action> CopyTranslatorFacade<Derived>::split_disjunctive_actions(formalism::EntityListView<formalism::Action> actions)
{
    auto result = ygg::IndexList<formalism::Action> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    for (auto action : actions)
    {
        const auto data = action.get_data();
        if (const auto precondition_view = action.get_precondition())
        {
            const auto precondition = this->self().flatten_condition(precondition_view.value());
            if (const auto condition_or = this->self().as_or(precondition))
            {
                for (auto part : condition_or->get_data().conditions)
                    this->self().push_unique(result,
                                             seen,
                                             formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                                                         data.name,
                                                                                         data.original_name,
                                                                                         data.parameters,
                                                                                         data.original_arity,
                                                                                         part,
                                                                                         data.effect)
                                                 .get_index());
                continue;
            }
        }
        this->self().push_unique(result, seen, action.get_index());
    }
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Action> CopyTranslatorFacade<Derived>::multiply_conditional_effect_actions(formalism::EntityListView<formalism::Action> actions)
{
    struct ConditionalEffect
    {
        formalism::EffectWhenView when;
        formalism::ConditionView condition;
        formalism::EffectView effect;
    };

    auto result = ygg::IndexList<formalism::Action> {};
    auto seen = std::unordered_set<ygg::uint_t> {};

    for (auto action : actions)
    {
        const auto data = action.get_data();
        auto unconditional = ygg::IndexList<formalism::Effect> {};
        auto conditional = std::vector<ConditionalEffect> {};

        if (const auto effect_view = action.get_effect())
        {
            auto effects = std::vector<formalism::EffectView> { effect_view.value() };
            if (const auto effect_and = this->self().template as_effect<formalism::EffectAnd>(effect_view.value()))
            {
                effects.clear();
                for (auto effect : effect_and->get_effects())
                    effects.push_back(effect);
            }

            for (auto effect : effects)
            {
                if (const auto effect_when = this->self().template as_effect<formalism::EffectWhen>(effect))
                    conditional.push_back({ *effect_when, effect_when->get_condition(), effect_when->get_effect() });
                else
                    unconditional.push_back(effect.get_index());
            }
        }

        if (conditional.empty())
        {
            this->self().push_unique(result, seen, action.get_index());
            continue;
        }

        if (conditional.size() >= std::numeric_limits<size_t>::digits)
            throw SemanticError("Too many conditional effects to multiply.");

        const auto num_variants = size_t { 1 } << conditional.size();
        for (auto mask = size_t { 0 }; mask < num_variants; ++mask)
        {
            auto conditions = ygg::IndexList<formalism::Condition> {};
            if (data.precondition)
                conditions.push_back(*data.precondition);

            auto effects = unconditional;
            for (auto i = size_t { 0 }; i < conditional.size(); ++i)
            {
                const auto selected = (mask & (size_t { 1 } << i)) != 0;
                if (selected)
                {
                    conditions.push_back(conditional[i].condition.get_index());
                    effects.push_back(conditional[i].effect.get_index());
                }
                else
                {
                    conditions.push_back(as_index(this->self().negate_condition(conditional[i].condition, this->m_storage->repository)));
                }
            }

            auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
            if (conditions.size() == 1)
                precondition = conditions.front();
            else if (!conditions.empty())
                precondition = as_index(this->self().make_conjunction(std::move(conditions)));

            auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
            if (effects.size() == 1)
                effect = effects.front();
            else if (!effects.empty())
                effect = this->self().wrap_effect(formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects))).get_index();

            auto name = std::string(data.name) + "_" + std::to_string(action.get_index().get_value());
            for (const auto& item : conditional)
                name += "_" + std::to_string(item.when.get_index().get_value());
            name += "_" + std::to_string(mask);

            this->self().push_unique(result,
                                     seen,
                                     formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                                                 std::move(name),
                                                                                 data.original_name,
                                                                                 data.parameters,
                                                                                 data.original_arity,
                                                                                 precondition,
                                                                                 effect)
                                         .get_index());
        }
    }

    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Axiom> CopyTranslatorFacade<Derived>::split_disjunctive_axioms(formalism::EntityListView<formalism::Axiom> axioms)
{
    auto result = ygg::IndexList<formalism::Axiom> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    for (auto axiom : axioms)
    {
        const auto data = axiom.get_data();
        const auto condition = this->self().flatten_condition(axiom.get_condition());
        if (const auto condition_or = this->self().as_or(condition))
        {
            for (auto part : condition_or->get_data().conditions)
                this->self().push_unique(
                    result,
                    seen,
                    formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, data.parameters, data.original_arity, data.head, part).get_index());
        }
        else
        {
            this->self().push_unique(result, seen, axiom.get_index());
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
    data.requirements = this->self().template copy_list<formalism::Requirement>(domain.get_requirements());
    data.types = this->self().template copy_list<formalism::Type>(domain.get_types());
    data.constants = this->self().template copy_list<formalism::Object>(domain.get_constants());
    data.predicates = this->self().template copy_list<formalism::Predicate>(domain.get_predicates());
    data.functions = this->self().template copy_list<formalism::FunctionSkeleton>(domain.get_functions());
    data.actions = this->self().template copy_list<formalism::Action>(domain.get_actions());
    data.axioms = this->self().template copy_list<formalism::Axiom>(domain.get_axioms());

    auto copied_domain = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, data);
    this->m_storage->translated_domain = copied_domain;
    data = copied_domain.get_data();
    data.index = {};

    switch (this->m_phase)
    {
        case TranslationPhase::RemoveUniversalQuantifiers:
            this->self().append_generated_domain_objects(data, copied_domain.get_requirements());
            if (!data.axioms.empty())
                this->self().ensure_derived_predicates_requirement(copied_domain.get_requirements(), data.requirements);
            break;
        case TranslationPhase::SplitDisjunctiveConditions:
            data.actions = this->self().split_disjunctive_actions(copied_domain.get_actions());
            data.axioms = this->self().split_disjunctive_axioms(copied_domain.get_axioms());
            break;
        case TranslationPhase::MultiplyConditionalEffects:
            data.actions = this->self().multiply_conditional_effect_actions(copied_domain.get_actions());
            data.requirements = this->self().strip_requirement(copied_domain.get_requirements(), formalism::RequirementKind::ConditionalEffects);
            break;
        case TranslationPhase::AddTypePredicates:
            this->self().add_type_predicates_to_domain(data, copied_domain);
            break;
        case TranslationPhase::InitializeEquality:
            this->self().add_equality_predicate_to_domain(data, copied_domain);
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
    data.index = {};
    data.domain = this->m_storage->translated_domain->get_index();
    data.requirements = this->self().template copy_list<formalism::Requirement>(task.get_requirements());
    data.objects = this->self().template copy_list<formalism::Object>(task.get_objects());
    data.initial_literals = this->self().template copy_list<formalism::Literal>(task.get_initial_literals());
    data.initial_function_values = this->self().template copy_list<formalism::InitialFunctionValue>(task.get_initial_function_values());

    if (const auto goal = task.get_goal())
    {
        if (this->m_phase == TranslationPhase::RenameQuantifiedVariables)
        {
            this->m_num_quantifications.clear();
            this->self().enter_variable_scope();
            const auto renamed_goal = this->self().rename_variables(goal.value(), task.get_context());
            this->self().leave_variable_scope();
            const auto previous = this->m_renaming_enabled;
            this->m_renaming_enabled = false;
            data.goal = as_index(this->self().copy(renamed_goal, this->m_storage->repository));
            this->m_renaming_enabled = previous;
        }
        else
        {
            data.goal = as_index(this->self().copy(goal.value(), task.get_context()));
        }
    }
    else
    {
        data.goal = {};
    }

    if (const auto metric = task.get_metric())
        data.metric = as_index(this->self().copy(metric.value(), task.get_context()));
    else
        data.metric = {};
    data.predicates = this->self().template copy_list<formalism::Predicate>(task.get_predicates());
    data.axioms = this->self().template copy_list<formalism::Axiom>(task.get_axioms());

    auto copied_task = formalism::get_or_create<formalism::Task>(this->m_storage->repository, data);
    data = copied_task.get_data();
    data.index = {};

    switch (this->m_phase)
    {
        case TranslationPhase::RemoveUniversalQuantifiers:
        case TranslationPhase::SimplifyGoal:
        {
            if (this->m_phase == TranslationPhase::SimplifyGoal && data.goal)
                data.goal = as_index(this->self().simplify_goal_condition(copied_task.get_goal().value()));

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
                this->self().ensure_derived_predicates_requirement(copied_task.get_requirements(), data.requirements);
            break;
        }
        case TranslationPhase::SplitDisjunctiveConditions:
            data.axioms = this->self().split_disjunctive_axioms(copied_task.get_axioms());
            break;
        case TranslationPhase::InitializeEquality:
            this->self().initialize_equality(data, task);
            break;
        case TranslationPhase::AddTypePredicates:
            this->self().initialize_type_literals(data, task);
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
