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


#ifndef LOKI_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class TopLevelTranslator : public CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>
{
public:
    explicit TopLevelTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>(context) {}

    formalism::ActionView copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository);

    formalism::AxiomView copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository);

    formalism::MetricView copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository);

    formalism::InitialFunctionValueView copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository);
};

template<typename Derived>
formalism::ActionView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository)
{
    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto renamed = this->self().rename_action_variables(source, repository);
        const auto previous = this->m_renaming_enabled;
        this->m_renaming_enabled = false;
        auto out = this->self().copy(renamed.get_index(), this->m_storage->repository);
        this->m_renaming_enabled = previous;
        remember(this->m_storage->actions, source, out);
        return out;
    }

    if (auto mapped = find_mapped(this->m_storage->actions, source))
        return *mapped;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto precondition = this->self().template copy_optional<formalism::Condition>(data.precondition, repository);
    if (this->m_phase == TranslationPhase::MoveExistentialQuantifiers)
        this->self().lift_top_level_exists(parameters, precondition);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(precondition, parameters);
    const auto out_parameters = this->m_phase == TranslationPhase::AddTypePredicates ? this->self().maybe_strip_parameters(parameters) : parameters;
    auto out = formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                           data.name,
                                                           out_parameters,
                                                           data.original_arity,
                                                           precondition,
                                                           this->self().template copy_optional<formalism::Effect>(data.effect, repository));
    this->self().leave_scope();
    remember(this->m_storage->actions, source, out);
    return out;
}

template<typename Derived>
formalism::AxiomView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository)
{
    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto renamed = this->self().rename_axiom_variables(source, repository);
        const auto previous = this->m_renaming_enabled;
        this->m_renaming_enabled = false;
        auto out = this->self().copy(renamed.get_index(), this->m_storage->repository);
        this->m_renaming_enabled = previous;
        remember(this->m_storage->axioms, source, out);
        return out;
    }

    if (auto mapped = find_mapped(this->m_storage->axioms, source))
        return *mapped;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = as_index(this->self().copy(data.condition, repository));
    if (this->m_phase == TranslationPhase::MoveExistentialQuantifiers)
        this->self().lift_top_level_exists(parameters, condition);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(condition, parameters);
    const auto out_parameters = this->m_phase == TranslationPhase::AddTypePredicates ? this->self().maybe_strip_parameters(parameters) : parameters;
    auto out = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository,
                                                          out_parameters,
                                                          data.original_arity,
                                                          as_index(this->self().copy(data.head, repository)),
                                                          condition);
    this->self().leave_scope();
    remember(this->m_storage->axioms, source, out);
    return out;
}

template<typename Derived>
formalism::MetricView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->metrics, source))
        return *mapped;
    const auto& data = repository[source];
    auto out =
        formalism::get_or_create<formalism::Metric>(this->m_storage->repository, data.minimize, as_index(this->self().copy(data.expression, repository)));
    remember(this->m_storage->metrics, source, out);
    return out;
}

template<typename Derived>
formalism::InitialFunctionValueView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::InitialFunctionValue> source,
                                                                      const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->initial_function_values, source))
        return *mapped;
    const auto& data = repository[source];
    auto out = formalism::get_or_create<formalism::InitialFunctionValue>(this->m_storage->repository,
                                                                         as_index(this->self().copy(data.function, repository)),
                                                                         as_index(this->self().copy(data.value, repository)));
    remember(this->m_storage->initial_function_values, source, out);
    return out;
}

}  // namespace loki::semantic::detail

#endif
