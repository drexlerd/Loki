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

    void increment_quantifications(const ygg::IndexList<formalism::Parameter>& parameters, const formalism::Repository& repository);

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const formalism::Repository& repository);

    ygg::IndexList<formalism::Parameter> copy_parameters(const ygg::IndexList<formalism::Parameter>& source, const formalism::Repository& repository);

    void enter_scope(const ygg::IndexList<formalism::Parameter>& parameters);

    void leave_scope();

    void append_generated_domain_objects(ygg::Data<formalism::Domain>& data);

    void ensure_derived_predicates_requirement(ygg::IndexList<formalism::Requirement>& requirements);

    void update_translated_domain();

    ygg::IndexList<formalism::Requirement> strip_typing_requirement(ygg::IndexList<formalism::Requirement> requirements);

    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const formalism::Repository& repository);

    formalism::RequirementView copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository);

    formalism::TypeView copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository);

    formalism::ObjectView copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository);

    formalism::VariableView copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository);

    formalism::ParameterView copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository);

    formalism::PredicateView copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository);

    formalism::FunctionSkeletonView copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository);

    formalism::TermView copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository);

    formalism::AtomView copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository);

    formalism::LiteralView copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository);

    formalism::FunctionExpressionNumberView copy(ygg::Index<formalism::FunctionExpressionNumber> source, const formalism::Repository& repository);

    formalism::FunctionTermView copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository);

    formalism::UnaryFunctionExpressionView copy(ygg::Index<formalism::UnaryFunctionExpression> source, const formalism::Repository& repository);

    formalism::BinaryFunctionExpressionView copy(ygg::Index<formalism::BinaryFunctionExpression> source, const formalism::Repository& repository);

    formalism::MultiFunctionExpressionView copy(ygg::Index<formalism::MultiFunctionExpression> source, const formalism::Repository& repository);

    formalism::FunctionExpressionView copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository);
};

template<typename Derived>
void BasicCopyTranslator<Derived>::increment_quantifications(const ygg::IndexList<formalism::Parameter>& parameters, const formalism::Repository& repository)
{
    for (auto parameter : parameters)
    {
        const auto variable = repository[parameter].variable;
        const auto key = variable.get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            ++it->second;
        else
            this->m_num_quantifications.emplace(key, 0);
    }
}

template<typename Derived>
template<typename T>
ygg::IndexList<T> BasicCopyTranslator<Derived>::copy_list(const ygg::IndexList<T>& source, const formalism::Repository& repository)
{
    auto result = ygg::IndexList<T> {};
    for (auto index : source)
        result.push_back(as_index(this->self().copy(index, repository)));
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> BasicCopyTranslator<Derived>::copy_parameters(const ygg::IndexList<formalism::Parameter>& source,
                                                                                   const formalism::Repository& repository)
{
    return this->self().template copy_list<formalism::Parameter>(source, repository);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::enter_scope(const ygg::IndexList<formalism::Parameter>& parameters)
{
    this->m_active_parameters.push_back(parameters);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::leave_scope()
{
    this->m_active_parameters.pop_back();
}

template<typename Derived>
void BasicCopyTranslator<Derived>::append_generated_domain_objects(ygg::Data<formalism::Domain>& data)
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

    this->self().ensure_derived_predicates_requirement(data.requirements);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::ensure_derived_predicates_requirement(ygg::IndexList<formalism::Requirement>& requirements)
{
    auto has_derived_requirement = false;
    for (auto requirement : requirements)
    {
        if (this->m_storage->repository[requirement].kind == formalism::RequirementKind::DerivedPredicates)
        {
            has_derived_requirement = true;
            break;
        }
    }
    if (!has_derived_requirement)
        requirements.push_back(
            formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, formalism::RequirementKind::DerivedPredicates).get_index());
}

template<typename Derived>
void BasicCopyTranslator<Derived>::update_translated_domain()
{
    auto data = this->m_storage->translated_domain->get_data();
    this->self().append_generated_domain_objects(data);
    auto view = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data));
    this->m_storage->translated_domain = view;
}

template<typename Derived>
ygg::IndexList<formalism::Requirement> BasicCopyTranslator<Derived>::strip_typing_requirement(ygg::IndexList<formalism::Requirement> requirements)
{
    if (!this->m_remove_typing)
        return requirements;
    auto result = ygg::IndexList<formalism::Requirement> {};
    for (auto requirement : requirements)
        if (this->m_storage->repository[requirement].kind != formalism::RequirementKind::Typing)
            result.push_back(requirement);
    return result;
}

template<typename Derived>
template<typename T>
cista::optional<ygg::Index<T>> BasicCopyTranslator<Derived>::copy_optional(const cista::optional<ygg::Index<T>>& source,
                                                                           const formalism::Repository& repository)
{
    if (!source)
        return {};
    return as_index(this->self().copy(*source, repository));
}

template<typename Derived>
formalism::RequirementView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->requirements, source))
        return *mapped;
    auto kind = repository[source].kind;
    auto out = formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, kind);
    remember(this->m_storage->requirements, source, out);
    return out;
}

template<typename Derived>
formalism::TypeView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->types, source))
        return *mapped;
    const auto& data = repository[source];
    auto out = formalism::get_or_create<formalism::Type>(this->m_storage->repository,
                                                         data.name,
                                                         this->self().template copy_list<formalism::Type>(data.bases, repository));
    remember(this->m_storage->types, source, out);
    return out;
}

template<typename Derived>
formalism::ObjectView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->objects, source))
        return *mapped;
    const auto& data = repository[source];
    auto types = this->self().template copy_list<formalism::Type>(data.types, repository);
    auto out = formalism::get_or_create<formalism::Object>(this->m_storage->repository, data.name, this->self().maybe_strip_types(types));
    this->m_storage->object_types[out.get_index().get_value()] = std::move(types);
    remember(this->m_storage->objects, source, out);
    return out;
}

template<typename Derived>
formalism::VariableView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository)
{
    auto name = std::string(repository[source].name);
    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto key = source.get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            name += "_" + std::to_string(it->second);
    }
    return formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string(name));
}

template<typename Derived>
formalism::ParameterView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository,
                                                          as_index(this->self().copy(data.variable, repository)),
                                                          this->self().template copy_list<formalism::Type>(data.types, repository));
}

template<typename Derived>
formalism::PredicateView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->predicates, source))
        return *mapped;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    auto parameters = this->self().template copy_list<formalism::Parameter>(data.parameters, repository);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        parameters = this->self().maybe_strip_parameters(parameters);
    auto out = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, data.name, std::move(parameters));
    this->m_renaming_enabled = previous;
    remember(this->m_storage->predicates, source, out);
    return out;
}

template<typename Derived>
formalism::FunctionSkeletonView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->functions, source))
        return *mapped;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    auto parameters = this->self().template copy_list<formalism::Parameter>(data.parameters, repository);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        parameters = this->self().maybe_strip_parameters(parameters);
    auto out = formalism::get_or_create<formalism::FunctionSkeleton>(this->m_storage->repository,
                                                                     data.name,
                                                                     std::move(parameters),
                                                                     as_index(this->self().copy(data.type, repository)));
    this->m_renaming_enabled = previous;
    remember(this->m_storage->functions, source, out);
    return out;
}

template<typename Derived>
formalism::TermView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant
                            { return ygg::Data<formalism::Term>::Variant(as_index(this->self().copy(arg, repository))); },
                            repository[source].value);
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::AtomView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Atom>(this->m_storage->repository,
                                                     as_index(this->self().copy(data.predicate, repository)),
                                                     this->self().template copy_list<formalism::Term>(data.terms, repository));
}

template<typename Derived>
formalism::LiteralView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository, as_index(this->self().copy(data.atom, repository)), data.m_polarity);
}

template<typename Derived>
formalism::FunctionExpressionNumberView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionExpressionNumber> source,
                                                                           const formalism::Repository& repository)
{
    if (auto mapped = find_mapped(this->m_storage->numbers, source))
        return *mapped;
    auto out = formalism::get_or_create<formalism::FunctionExpressionNumber>(this->m_storage->repository, repository[source].value);
    remember(this->m_storage->numbers, source, out);
    return out;
}

template<typename Derived>
formalism::FunctionTermView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::FunctionTerm>(this->m_storage->repository,
                                                             as_index(this->self().copy(data.function, repository)),
                                                             this->self().template copy_list<formalism::Term>(data.terms, repository));
}

template<typename Derived>
formalism::UnaryFunctionExpressionView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::UnaryFunctionExpression> source,
                                                                          const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::UnaryFunctionExpression>(this->m_storage->repository,
                                                                        data.op,
                                                                        as_index(this->self().copy(data.expression, repository)));
}

template<typename Derived>
formalism::BinaryFunctionExpressionView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::BinaryFunctionExpression> source,
                                                                           const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::BinaryFunctionExpression>(this->m_storage->repository,
                                                                         data.op,
                                                                         as_index(this->self().copy(data.left, repository)),
                                                                         as_index(this->self().copy(data.right, repository)));
}

template<typename Derived>
formalism::MultiFunctionExpressionView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::MultiFunctionExpression> source,
                                                                          const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::MultiFunctionExpression>(
        this->m_storage->repository,
        data.op,
        this->self().template copy_list<formalism::FunctionExpression>(data.expressions, repository));
}

template<typename Derived>
formalism::FunctionExpressionView BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant
                            { return ygg::Data<formalism::FunctionExpression>::Variant(as_index(this->self().copy(arg, repository))); },
                            repository[source].value);
    return formalism::get_or_create<formalism::FunctionExpression>(this->m_storage->repository, std::move(value));
}

}  // namespace loki::semantic::detail

#endif
