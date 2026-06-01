/*
 * Copyright (C) 2026 Dominik Drexler
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

    void update_translated_domain();

    ygg::IndexList<formalism::Requirement> strip_typing_requirement(ygg::IndexList<formalism::Requirement> requirements);




    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const formalism::Repository& repository);

    ygg::Index<formalism::Requirement> copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository);

    ygg::Index<formalism::Type> copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository);

    ygg::Index<formalism::Object> copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository);

    ygg::Index<formalism::Variable> copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository);

    ygg::Index<formalism::Parameter> copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository);

    ygg::Index<formalism::Predicate> copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository);

    ygg::Index<formalism::FunctionSkeleton> copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository);

    ygg::Index<formalism::Term> copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository);

    ygg::Index<formalism::Atom> copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository);

    ygg::Index<formalism::Literal> copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository);

    ygg::Index<formalism::FunctionExpressionNumber> copy(ygg::Index<formalism::FunctionExpressionNumber> source, const formalism::Repository& repository);

    ygg::Index<formalism::FunctionTerm> copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository);

    ygg::Index<formalism::UnaryFunctionExpression> copy(ygg::Index<formalism::UnaryFunctionExpression> source, const formalism::Repository& repository);

    ygg::Index<formalism::BinaryFunctionExpression> copy(ygg::Index<formalism::BinaryFunctionExpression> source, const formalism::Repository& repository);

    ygg::Index<formalism::MultiFunctionExpression> copy(ygg::Index<formalism::MultiFunctionExpression> source, const formalism::Repository& repository);

    ygg::Index<formalism::FunctionExpression> copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository);};

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
        result.push_back(this->self().copy(index, repository));
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> BasicCopyTranslator<Derived>::copy_parameters(const ygg::IndexList<formalism::Parameter>& source, const formalism::Repository& repository)
{
    return this->self().template copy_list<formalism::Parameter>(source, repository);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::enter_scope(const ygg::IndexList<formalism::Parameter>& parameters) { this->m_active_parameters.push_back(parameters); }

template<typename Derived>
void BasicCopyTranslator<Derived>::leave_scope() { this->m_active_parameters.pop_back(); }

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

template<typename Derived>
void BasicCopyTranslator<Derived>::update_translated_domain()
{
    auto data = this->m_storage->repository[this->m_storage->translated_domain];
    this->self().append_generated_domain_objects(data);
    this->m_storage->translated_domain = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(data)).get_index();
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
cista::optional<ygg::Index<T>> BasicCopyTranslator<Derived>::copy_optional(const cista::optional<ygg::Index<T>>& source, const formalism::Repository& repository)
{
    if (!source)
        return {};
    return this->self().copy(*source, repository);
}

template<typename Derived>
ygg::Index<formalism::Requirement> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Requirement> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Requirement> out;
    if (find_mapped(this->m_storage->requirements, source, out)) return out;
    auto kind = repository[source].kind;
    out = formalism::get_or_create<formalism::Requirement>(this->m_storage->repository, kind).get_index();
    remember(this->m_storage->requirements, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Type> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Type> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Type> out;
    if (find_mapped(this->m_storage->types, source, out)) return out;
    const auto& data = repository[source];
    out = formalism::get_or_create<formalism::Type>(this->m_storage->repository, data.name, this->self().template copy_list<formalism::Type>(data.bases, repository)).get_index();
    remember(this->m_storage->types, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Object> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Object> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Object> out;
    if (find_mapped(this->m_storage->objects, source, out)) return out;
    const auto& data = repository[source];
    auto types = this->self().template copy_list<formalism::Type>(data.types, repository);
    this->m_storage->object_types_by_name[std::string(data.name)] = types;
    out = formalism::get_or_create<formalism::Object>(this->m_storage->repository, data.name, this->self().maybe_strip_types(types)).get_index();
    remember(this->m_storage->objects, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Variable> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Variable> source, const formalism::Repository& repository)
{
    auto name = std::string(repository[source].name);
    if (this->m_renaming_enabled)
    {
        const auto key = source.get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            name += "_" + std::to_string(it->second);
    }
    return formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string(name)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Parameter> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Parameter> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, this->self().copy(data.variable, repository), this->self().template copy_list<formalism::Type>(data.types, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Predicate> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Predicate> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Predicate> out;
    if (find_mapped(this->m_storage->predicates, source, out)) return out;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    out = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, data.name, this->self().template copy_list<formalism::Parameter>(data.parameters, repository)).get_index();
    this->m_renaming_enabled = previous;
    remember(this->m_storage->predicates, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::FunctionSkeleton> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionSkeleton> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::FunctionSkeleton> out;
    if (find_mapped(this->m_storage->functions, source, out)) return out;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    out = formalism::get_or_create<formalism::FunctionSkeleton>(this->m_storage->repository, data.name, this->self().template copy_list<formalism::Parameter>(data.parameters, repository), this->self().copy(data.type, repository)).get_index();
    this->m_renaming_enabled = previous;
    remember(this->m_storage->functions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Term> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Term> source, const formalism::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::Term>::Variant { return ygg::Data<formalism::Term>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, std::move(value)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Atom> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Atom> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Atom>(this->m_storage->repository, this->self().copy(data.predicate, repository), this->self().template copy_list<formalism::Term>(data.terms, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Literal> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::Literal> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository, data.positive, this->self().copy(data.atom, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::FunctionExpressionNumber> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionExpressionNumber> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::FunctionExpressionNumber> out;
    if (find_mapped(this->m_storage->numbers, source, out)) return out;
    out = formalism::get_or_create<formalism::FunctionExpressionNumber>(this->m_storage->repository, repository[source].value).get_index();
    remember(this->m_storage->numbers, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::FunctionTerm> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionTerm> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::FunctionTerm>(this->m_storage->repository, this->self().copy(data.function, repository), this->self().template copy_list<formalism::Term>(data.terms, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::UnaryFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::UnaryFunctionExpression> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::UnaryFunctionExpression>(this->m_storage->repository, data.op, this->self().copy(data.expression, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::BinaryFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::BinaryFunctionExpression> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::BinaryFunctionExpression>(this->m_storage->repository, data.op, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::MultiFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::MultiFunctionExpression> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::MultiFunctionExpression>(this->m_storage->repository, data.op, this->self().template copy_list<formalism::FunctionExpression>(data.expressions, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::FunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<formalism::FunctionExpression> source, const formalism::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant { return ygg::Data<formalism::FunctionExpression>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    return formalism::get_or_create<formalism::FunctionExpression>(this->m_storage->repository, std::move(value)).get_index();
}

} // namespace loki::semantic::detail

#endif
