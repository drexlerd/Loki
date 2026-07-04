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

#ifndef LOKI_SEMANTIC_TRANSLATOR_BASIC_COPY_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_BASIC_COPY_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class BasicCopyTranslator : public CopyTranslatorComponent<Derived, BasicCopyTranslator<Derived>>
{
public:
    explicit BasicCopyTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, BasicCopyTranslator<Derived>>(context) {}

    void increment_quantifications(formalism::EntityListView<formalism::Parameter> parameters);

    template<typename T>
    ygg::IndexList<T> copy_list(formalism::EntityListView<T> source);

    ygg::IndexList<formalism::Parameter> copy_parameters(formalism::EntityListView<formalism::Parameter> source);

    void enter_scope(const ygg::IndexList<formalism::Parameter>& parameters, formalism::EntityListView<formalism::Parameter> source_parameters);

    void leave_scope();

    void append_generated_domain_objects(ygg::Data<formalism::Domain>& data, formalism::EntityListView<formalism::Requirement> requirements);

    void ensure_derived_predicates_requirement(formalism::EntityListView<formalism::Requirement> requirements,
                                               ygg::IndexList<formalism::Requirement>& out_requirements);

    void update_translated_domain();

    ygg::IndexList<formalism::Requirement> strip_requirement(formalism::EntityListView<formalism::Requirement> requirements, formalism::RequirementKind kind);

    ygg::IndexList<formalism::Requirement> strip_typing_requirement(formalism::EntityListView<formalism::Requirement> requirements);

    formalism::RequirementView copy(formalism::RequirementView source, const formalism::Repository& repository);

    formalism::TypeView copy(formalism::TypeView source, const formalism::Repository& repository);

    formalism::ObjectView copy(formalism::ObjectView source, const formalism::Repository& repository);

    formalism::VariableView copy(formalism::VariableView source, const formalism::Repository& repository);

    formalism::ParameterView copy(formalism::ParameterView source, const formalism::Repository& repository);

    formalism::PredicateView copy(formalism::PredicateView source, const formalism::Repository& repository);

    formalism::FunctionSkeletonView copy(formalism::FunctionSkeletonView source, const formalism::Repository& repository);

    formalism::TermView copy(formalism::TermView source, const formalism::Repository& repository);

    formalism::AtomView copy(formalism::AtomView source, const formalism::Repository& repository);

    formalism::LiteralView copy(formalism::LiteralView source, const formalism::Repository& repository);

    formalism::FunctionExpressionNumberView copy(formalism::FunctionExpressionNumberView source, const formalism::Repository& repository);

    formalism::FunctionTermView copy(formalism::FunctionTermView source, const formalism::Repository& repository);

    formalism::UnaryFunctionExpressionView copy(formalism::UnaryFunctionExpressionView source, const formalism::Repository& repository);

    formalism::BinaryFunctionExpressionView copy(formalism::BinaryFunctionExpressionView source, const formalism::Repository& repository);

    formalism::MultiFunctionExpressionView copy(formalism::MultiFunctionExpressionView source, const formalism::Repository& repository);

    formalism::FunctionExpressionView copy(formalism::FunctionExpressionView source, const formalism::Repository& repository);
};

template<typename Derived>
void BasicCopyTranslator<Derived>::increment_quantifications(formalism::EntityListView<formalism::Parameter> parameters)
{
    for (auto parameter : parameters)
    {
        const auto variable = parameter.get_variable().get_index();
        const auto key = variable.get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            ++it->second;
        else
            this->m_num_quantifications.emplace(key, 0);
    }
}

template<typename Derived>
template<typename T>
ygg::IndexList<T> BasicCopyTranslator<Derived>::copy_list(formalism::EntityListView<T> source)
{
    auto result = ygg::IndexList<T> {};
    for (auto view : source)
        result.push_back(as_index(this->self().copy(view, source.get_context())));
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> BasicCopyTranslator<Derived>::copy_parameters(formalism::EntityListView<formalism::Parameter> source)
{
    return this->self().template copy_list<formalism::Parameter>(source);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::enter_scope(const ygg::IndexList<formalism::Parameter>& parameters,
                                               formalism::EntityListView<formalism::Parameter> source_parameters)
{
    auto variables = ygg::IndexList<formalism::Variable> {};
    for (auto parameter : source_parameters)
        variables.push_back(as_index(this->self().copy(parameter.get_variable(), source_parameters.get_context())));
    this->m_active_parameters.push_back(parameters);
    this->m_active_parameter_variables.push_back(std::move(variables));
}

template<typename Derived>
void BasicCopyTranslator<Derived>::leave_scope()
{
    this->m_active_parameters.pop_back();
    this->m_active_parameter_variables.pop_back();
}

template<typename Derived>
void BasicCopyTranslator<Derived>::append_generated_domain_objects(ygg::Data<formalism::Domain>& data,
                                                                   formalism::EntityListView<formalism::Requirement> requirements)
{
    if (this->m_generated_predicates.empty() && this->m_generated_axioms.empty())
        return;

    for (auto predicate : this->m_generated_predicates)
        data.predicates.push_back(predicate);
    if (this->m_append_generated_axioms_to_domain)
    {
        for (auto axiom : this->m_generated_axioms)
            data.axioms.push_back(axiom);
    }

    this->self().ensure_derived_predicates_requirement(requirements, data.requirements);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::ensure_derived_predicates_requirement(formalism::EntityListView<formalism::Requirement> requirements,
                                                                         ygg::IndexList<formalism::Requirement>& out_requirements)
{
    auto has_derived_requirement = false;
    for (auto requirement : requirements)
    {
        if (requirement.get_kind() == formalism::RequirementKind::DerivedPredicates)
        {
            has_derived_requirement = true;
            break;
        }
    }
    if (!has_derived_requirement)
        out_requirements.push_back(
            formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, formalism::RequirementKind::DerivedPredicates).get_index());
}

template<typename Derived>
void BasicCopyTranslator<Derived>::update_translated_domain()
{
    auto domain = *this->m_storage->translated_domain;
    auto data = domain.get_data();
    this->self().append_generated_domain_objects(data, domain.get_requirements());
    auto view = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view;
}

template<typename Derived>
ygg::IndexList<formalism::Requirement> BasicCopyTranslator<Derived>::strip_requirement(formalism::EntityListView<formalism::Requirement> requirements,
                                                                                       formalism::RequirementKind kind)
{
    auto result = ygg::IndexList<formalism::Requirement> {};
    for (auto requirement : requirements)
        if (requirement.get_kind() != kind)
            result.push_back(as_index(this->self().copy(requirement, requirements.get_context())));
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Requirement> BasicCopyTranslator<Derived>::strip_typing_requirement(formalism::EntityListView<formalism::Requirement> requirements)
{
    return this->m_remove_typing ? this->self().strip_requirement(requirements, formalism::RequirementKind::Typing) :
                                   this->self().template copy_list<formalism::Requirement>(requirements);
}

template<typename Derived>
formalism::RequirementView BasicCopyTranslator<Derived>::copy(formalism::RequirementView source, const formalism::Repository&)
{
    if (auto mapped = find_mapped(this->m_storage->requirements, source.get_index()))
        return *mapped;
    auto out = formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, source.get_kind());
    remember(this->m_storage->requirements, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::TypeView BasicCopyTranslator<Derived>::copy(formalism::TypeView source, const formalism::Repository&)
{
    if (auto mapped = find_mapped(this->m_storage->types, source.get_index()))
        return *mapped;
    auto out = formalism::get_or_create<formalism::Type>(this->m_storage->repository,
                                                         source.get_name(),
                                                         this->self().template copy_list<formalism::Type>(source.get_bases()));
    remember(this->m_storage->types, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::ObjectView BasicCopyTranslator<Derived>::copy(formalism::ObjectView source, const formalism::Repository&)
{
    if (auto mapped = find_mapped(this->m_storage->objects, source.get_index()))
        return *mapped;
    auto types = this->self().template copy_list<formalism::Type>(source.get_types());
    auto source_types = std::vector<formalism::TypeView> {};
    for (auto type : source.get_types())
        source_types.push_back(type);
    auto out = formalism::get_or_create<formalism::Object>(this->m_storage->repository, source.get_name(), this->self().maybe_strip_types(types));
    this->m_storage->object_types[out.get_index().get_value()] = std::move(types);
    this->m_storage->object_type_views[out.get_index().get_value()] = std::move(source_types);
    remember(this->m_storage->objects, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::VariableView BasicCopyTranslator<Derived>::copy(formalism::VariableView source, const formalism::Repository&)
{
    auto name = std::string(source.get_name());
    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto key = source.get_index().get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            name += "_" + std::to_string(it->second);
    }
    return formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string(name));
}

template<typename Derived>
formalism::ParameterView BasicCopyTranslator<Derived>::copy(formalism::ParameterView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository,
                                                          as_index(this->self().copy(source.get_variable(), repository)),
                                                          this->self().template copy_list<formalism::Type>(source.get_types()));
}

template<typename Derived>
formalism::PredicateView BasicCopyTranslator<Derived>::copy(formalism::PredicateView source, const formalism::Repository&)
{
    if (auto mapped = find_mapped(this->m_storage->predicates, source.get_index()))
        return *mapped;
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    auto parameters = this->self().template copy_list<formalism::Parameter>(source.get_parameters());
    if (this->self().removes_typing_now())
        parameters = this->self().copy_parameters_without_types(source.get_parameters());
    auto out = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, source.get_name(), std::move(parameters));
    this->m_used_predicate_names.insert(std::string(source.get_name()));
    this->m_renaming_enabled = previous;
    remember(this->m_storage->predicates, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::FunctionSkeletonView BasicCopyTranslator<Derived>::copy(formalism::FunctionSkeletonView source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->functions, source.get_index()))
        return *mapped;
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    auto parameters = this->self().template copy_list<formalism::Parameter>(source.get_parameters());
    if (this->self().removes_typing_now())
        parameters = this->self().copy_parameters_without_types(source.get_parameters());
    auto out = formalism::get_or_create<formalism::FunctionSkeleton>(this->m_storage->repository,
                                                                     source.get_name(),
                                                                     std::move(parameters),
                                                                     as_index(this->self().copy(source.get_type(), repository)));
    this->m_renaming_enabled = previous;
    remember(this->m_storage->functions, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::TermView BasicCopyTranslator<Derived>::copy(formalism::TermView source, const formalism::Repository& repository)
{
    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant
                            { return ygg::Data<formalism::Term>::Variant(as_index(this->self().copy(arg, repository))); },
                            source.get_value());
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::AtomView BasicCopyTranslator<Derived>::copy(formalism::AtomView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::Atom>(this->m_storage->repository,
                                                     as_index(this->self().copy(source.get_predicate(), repository)),
                                                     this->self().template copy_list<formalism::Term>(source.get_terms()));
}

template<typename Derived>
formalism::LiteralView BasicCopyTranslator<Derived>::copy(formalism::LiteralView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository,
                                                        as_index(this->self().copy(source.get_atom(), repository)),
                                                        source.get_polarity());
}

template<typename Derived>
formalism::FunctionExpressionNumberView BasicCopyTranslator<Derived>::copy(formalism::FunctionExpressionNumberView source, const formalism::Repository&)
{
    if (auto mapped = find_mapped(this->m_storage->numbers, source.get_index()))
        return *mapped;
    auto out = formalism::get_or_create<formalism::FunctionExpressionNumber>(this->m_storage->repository, source.get_value());
    remember(this->m_storage->numbers, source.get_index(), out);
    return out;
}

template<typename Derived>
formalism::FunctionTermView BasicCopyTranslator<Derived>::copy(formalism::FunctionTermView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::FunctionTerm>(this->m_storage->repository,
                                                             as_index(this->self().copy(source.get_function(), repository)),
                                                             this->self().template copy_list<formalism::Term>(source.get_terms()));
}

template<typename Derived>
formalism::UnaryFunctionExpressionView BasicCopyTranslator<Derived>::copy(formalism::UnaryFunctionExpressionView source,
                                                                          const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::UnaryFunctionExpression>(this->m_storage->repository,
                                                                        data.op,
                                                                        as_index(this->self().copy(source.get_expression(), repository)));
}

template<typename Derived>
formalism::BinaryFunctionExpressionView BasicCopyTranslator<Derived>::copy(formalism::BinaryFunctionExpressionView source,
                                                                           const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::BinaryFunctionExpression>(this->m_storage->repository,
                                                                         data.op,
                                                                         as_index(this->self().copy(source.get_left(), repository)),
                                                                         as_index(this->self().copy(source.get_right(), repository)));
}

template<typename Derived>
formalism::MultiFunctionExpressionView BasicCopyTranslator<Derived>::copy(formalism::MultiFunctionExpressionView source, const formalism::Repository&)
{
    return formalism::get_or_create<formalism::MultiFunctionExpression>(
        this->m_storage->repository,
        source.get_data().op,
        this->self().template copy_list<formalism::FunctionExpression>(source.get_expressions()));
}

template<typename Derived>
formalism::FunctionExpressionView BasicCopyTranslator<Derived>::copy(formalism::FunctionExpressionView source, const formalism::Repository& repository)
{
    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant
                            { return ygg::Data<formalism::FunctionExpression>::Variant(as_index(this->self().copy(arg, repository))); },
                            source.get_value());
    return formalism::get_or_create<formalism::FunctionExpression>(this->m_storage->repository, std::move(value));
}

}  // namespace loki::semantic::detail

#endif
