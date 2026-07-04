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

#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionTranslator : public CopyTranslatorComponent<Derived, ConditionTranslator<Derived>>
{
public:
    explicit ConditionTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionTranslator<Derived>>(context) {}

    formalism::ConditionLiteralView copy(formalism::ConditionLiteralView source, const formalism::Repository& repository);

    formalism::ConditionAndView copy(formalism::ConditionAndView source, const formalism::Repository& repository);

    formalism::ConditionOrView copy(formalism::ConditionOrView source, const formalism::Repository& repository);

    formalism::ConditionNotView copy(formalism::ConditionNotView source, const formalism::Repository& repository);

    formalism::ConditionImplyView copy(formalism::ConditionImplyView source, const formalism::Repository& repository);

    formalism::ConditionExistsView copy(formalism::ConditionExistsView source, const formalism::Repository& repository);

    formalism::ConditionForallView copy(formalism::ConditionForallView source, const formalism::Repository& repository);

    formalism::ConditionNumericConstraintView copy(formalism::ConditionNumericConstraintView source, const formalism::Repository& repository);

    formalism::ConditionView copy(formalism::ConditionView source, const formalism::Repository& repository);
};

template<typename Derived>
formalism::ConditionLiteralView ConditionTranslator<Derived>::copy(formalism::ConditionLiteralView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, as_index(this->self().copy(source.get_literal(), repository)));
}

template<typename Derived>
formalism::ConditionAndView ConditionTranslator<Derived>::copy(formalism::ConditionAndView source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().copy(condition, repository)));
    return formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionOrView ConditionTranslator<Derived>::copy(formalism::ConditionOrView source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().copy(condition, repository)));
    return formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionNotView ConditionTranslator<Derived>::copy(formalism::ConditionNotView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository, as_index(this->self().copy(source.get_condition(), repository)));
}

template<typename Derived>
formalism::ConditionImplyView ConditionTranslator<Derived>::copy(formalism::ConditionImplyView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository,
                                                               as_index(this->self().copy(source.get_left(), repository)),
                                                               as_index(this->self().copy(source.get_right(), repository)));
}

template<typename Derived>
formalism::ConditionExistsView ConditionTranslator<Derived>::copy(formalism::ConditionExistsView source, const formalism::Repository& repository)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameters = this->self().copy_parameters(source.get_parameters());
    this->self().enter_scope(parameters, source.get_parameters());
    const auto condition = as_index(this->self().copy(source.get_condition(), repository));
    this->self().leave_scope();
    auto typed_condition = condition;
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(typed_condition, source.get_parameters());
    const auto out_parameters = this->self().removes_typing_now() ? this->self().copy_parameters_without_types(source.get_parameters()) : parameters;
    return formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, out_parameters, typed_condition);
}

template<typename Derived>
formalism::ConditionForallView ConditionTranslator<Derived>::copy(formalism::ConditionForallView source, const formalism::Repository& repository)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameters = this->self().copy_parameters(source.get_parameters());
    this->self().enter_scope(parameters, source.get_parameters());
    const auto condition = as_index(this->self().copy(source.get_condition(), repository));
    this->self().leave_scope();
    auto typed_condition = condition;
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(typed_condition, source.get_parameters());
    const auto out_parameters = this->self().removes_typing_now() ? this->self().copy_parameters_without_types(source.get_parameters()) : parameters;
    return formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, out_parameters, typed_condition);
}

template<typename Derived>
formalism::ConditionNumericConstraintView ConditionTranslator<Derived>::copy(formalism::ConditionNumericConstraintView source,
                                                                             const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository,
                                                                           data.comparator,
                                                                           as_index(this->self().copy(source.get_left(), repository)),
                                                                           as_index(this->self().copy(source.get_right(), repository)));
}

template<typename Derived>
formalism::ConditionView ConditionTranslator<Derived>::copy(formalism::ConditionView source, const formalism::Repository& repository)
{
    switch (this->m_phase)
    {
        case TranslationPhase::ToNegationNormalForm:
            return this->self().flatten_condition(
                ygg::visit([&](const auto& arg) { return this->self().copy_condition_node(arg, repository); }, source.get_value()));
        case TranslationPhase::RemoveUniversalQuantifiers:
            return this->self().remove_universal_quantifiers(source, repository);
        case TranslationPhase::ToDisjunctiveNormalForm:
        {
            const auto copied =
                ygg::visit([&](const auto& arg) { return this->self().wrap_condition(this->self().copy(arg, repository)); }, source.get_value());
            return this->self().flatten_condition(this->self().to_dnf(copied));
        }
        default:
            return this->self().flatten_condition(
                ygg::visit([&](const auto& arg) { return this->self().wrap_condition(this->self().copy(arg, repository)); }, source.get_value()));
    }
}

}  // namespace loki::semantic::detail

#endif
