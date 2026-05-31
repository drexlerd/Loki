/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_BASIC_COPY_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_BASIC_COPY_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class BasicCopyTranslator : public CopyTranslatorComponent<Derived, BasicCopyTranslator<Derived>>
{
public:
    explicit BasicCopyTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, BasicCopyTranslator<Derived>>(context) {}

    void increment_quantifications(const ygg::IndexList<pddl::Parameter>& parameters, const pddl::Repository& repository);

    template<typename T>
    ygg::IndexList<T> copy_list(const ygg::IndexList<T>& source, const pddl::Repository& repository);

    ygg::IndexList<pddl::Parameter> copy_parameters(const ygg::IndexList<pddl::Parameter>& source, const pddl::Repository& repository);













    void enter_scope(const ygg::IndexList<pddl::Parameter>& parameters);

    void leave_scope();

    void append_generated_domain_objects(ygg::Data<pddl::Domain>& data);

    void update_translated_domain();

    ygg::IndexList<pddl::Requirement> strip_typing_requirement(ygg::IndexList<pddl::Requirement> requirements);




    template<typename T>
    cista::optional<ygg::Index<T>> copy_optional(const cista::optional<ygg::Index<T>>& source, const pddl::Repository& repository);

    ygg::Index<pddl::Requirement> copy(ygg::Index<pddl::Requirement> source, const pddl::Repository& repository);

    ygg::Index<pddl::Type> copy(ygg::Index<pddl::Type> source, const pddl::Repository& repository);

    ygg::Index<pddl::Object> copy(ygg::Index<pddl::Object> source, const pddl::Repository& repository);

    ygg::Index<pddl::Variable> copy(ygg::Index<pddl::Variable> source, const pddl::Repository& repository);

    ygg::Index<pddl::Parameter> copy(ygg::Index<pddl::Parameter> source, const pddl::Repository& repository);

    ygg::Index<pddl::Predicate> copy(ygg::Index<pddl::Predicate> source, const pddl::Repository& repository);

    ygg::Index<pddl::FunctionSkeleton> copy(ygg::Index<pddl::FunctionSkeleton> source, const pddl::Repository& repository);

    ygg::Index<pddl::Term> copy(ygg::Index<pddl::Term> source, const pddl::Repository& repository);

    ygg::Index<pddl::Atom> copy(ygg::Index<pddl::Atom> source, const pddl::Repository& repository);

    ygg::Index<pddl::Literal> copy(ygg::Index<pddl::Literal> source, const pddl::Repository& repository);

    ygg::Index<pddl::FunctionExpressionNumber> copy(ygg::Index<pddl::FunctionExpressionNumber> source, const pddl::Repository& repository);

    ygg::Index<pddl::FunctionTerm> copy(ygg::Index<pddl::FunctionTerm> source, const pddl::Repository& repository);

    ygg::Index<pddl::UnaryFunctionExpression> copy(ygg::Index<pddl::UnaryFunctionExpression> source, const pddl::Repository& repository);

    ygg::Index<pddl::BinaryFunctionExpression> copy(ygg::Index<pddl::BinaryFunctionExpression> source, const pddl::Repository& repository);

    ygg::Index<pddl::MultiFunctionExpression> copy(ygg::Index<pddl::MultiFunctionExpression> source, const pddl::Repository& repository);

    ygg::Index<pddl::FunctionExpression> copy(ygg::Index<pddl::FunctionExpression> source, const pddl::Repository& repository);};

template<typename Derived>
void BasicCopyTranslator<Derived>::increment_quantifications(const ygg::IndexList<pddl::Parameter>& parameters, const pddl::Repository& repository)
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
ygg::IndexList<T> BasicCopyTranslator<Derived>::copy_list(const ygg::IndexList<T>& source, const pddl::Repository& repository)
{
    auto result = ygg::IndexList<T> {};
    for (auto index : source)
        result.push_back(this->self().copy(index, repository));
    return result;
}

template<typename Derived>
ygg::IndexList<pddl::Parameter> BasicCopyTranslator<Derived>::copy_parameters(const ygg::IndexList<pddl::Parameter>& source, const pddl::Repository& repository)
{
    return this->self().template copy_list<pddl::Parameter>(source, repository);
}

template<typename Derived>
void BasicCopyTranslator<Derived>::enter_scope(const ygg::IndexList<pddl::Parameter>& parameters) { this->m_active_parameters.push_back(parameters); }

template<typename Derived>
void BasicCopyTranslator<Derived>::leave_scope() { this->m_active_parameters.pop_back(); }

template<typename Derived>
void BasicCopyTranslator<Derived>::append_generated_domain_objects(ygg::Data<pddl::Domain>& data)
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
        if (this->m_storage->repository[requirement].kind == pddl::RequirementKind::DerivedPredicates)
        {
            has_derived_requirement = true;
            break;
        }
    }
    if (!has_derived_requirement)
        data.requirements.push_back(this->m_storage->builder.requirement(this->m_storage->repository, pddl::RequirementKind::DerivedPredicates).get_index());
}

template<typename Derived>
void BasicCopyTranslator<Derived>::update_translated_domain()
{
    auto data = this->m_storage->repository[this->m_storage->translated_domain];
    this->self().append_generated_domain_objects(data);
    this->m_storage->translated_domain = this->m_storage->builder.domain(this->m_storage->repository, std::move(data)).get_index();
}

template<typename Derived>
ygg::IndexList<pddl::Requirement> BasicCopyTranslator<Derived>::strip_typing_requirement(ygg::IndexList<pddl::Requirement> requirements)
{
    if (!this->m_remove_typing)
        return requirements;
    auto result = ygg::IndexList<pddl::Requirement> {};
    for (auto requirement : requirements)
        if (this->m_storage->repository[requirement].kind != pddl::RequirementKind::Typing)
            result.push_back(requirement);
    return result;
}

template<typename Derived>
template<typename T>
cista::optional<ygg::Index<T>> BasicCopyTranslator<Derived>::copy_optional(const cista::optional<ygg::Index<T>>& source, const pddl::Repository& repository)
{
    if (!source)
        return {};
    return this->self().copy(*source, repository);
}

template<typename Derived>
ygg::Index<pddl::Requirement> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Requirement> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Requirement> out;
    if (find_mapped(this->m_storage->requirements, source, out)) return out;
    auto kind = repository[source].kind;
    out = this->m_storage->builder.requirement(this->m_storage->repository, kind).get_index();
    remember(this->m_storage->requirements, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Type> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Type> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Type> out;
    if (find_mapped(this->m_storage->types, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.type(this->m_storage->repository, data.name, this->self().template copy_list<pddl::Type>(data.bases, repository)).get_index();
    remember(this->m_storage->types, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Object> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Object> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Object> out;
    if (find_mapped(this->m_storage->objects, source, out)) return out;
    const auto& data = repository[source];
    auto types = this->self().template copy_list<pddl::Type>(data.types, repository);
    this->m_storage->object_types_by_name[std::string(data.name)] = types;
    out = this->m_storage->builder.object(this->m_storage->repository, data.name, this->self().maybe_strip_types(types)).get_index();
    remember(this->m_storage->objects, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Variable> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Variable> source, const pddl::Repository& repository)
{
    auto name = std::string(repository[source].name);
    if (this->m_renaming_enabled)
    {
        const auto key = source.get_value();
        if (auto it = this->m_num_quantifications.find(key); it != this->m_num_quantifications.end())
            name += "_" + std::to_string(it->second);
    }
    return this->m_storage->builder.variable(this->m_storage->repository, cista::offset::string(name)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Parameter> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Parameter> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return this->m_storage->builder.parameter(this->m_storage->repository, this->self().copy(data.variable, repository), this->self().template copy_list<pddl::Type>(data.types, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Predicate> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Predicate> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Predicate> out;
    if (find_mapped(this->m_storage->predicates, source, out)) return out;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    out = this->m_storage->builder.predicate(this->m_storage->repository, data.name, this->self().template copy_list<pddl::Parameter>(data.parameters, repository)).get_index();
    this->m_renaming_enabled = previous;
    remember(this->m_storage->predicates, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::FunctionSkeleton> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::FunctionSkeleton> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::FunctionSkeleton> out;
    if (find_mapped(this->m_storage->functions, source, out)) return out;
    const auto& data = repository[source];
    const auto previous = this->m_renaming_enabled;
    this->m_renaming_enabled = false;
    out = this->m_storage->builder.function_skeleton(this->m_storage->repository, data.name, this->self().template copy_list<pddl::Parameter>(data.parameters, repository), this->self().copy(data.type, repository)).get_index();
    this->m_renaming_enabled = previous;
    remember(this->m_storage->functions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Term> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Term> source, const pddl::Repository& repository)
{
    auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::Term>::Variant { return ygg::Data<pddl::Term>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    return this->m_storage->builder.term(this->m_storage->repository, std::move(value)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Atom> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Atom> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return this->m_storage->builder.atom(this->m_storage->repository, this->self().copy(data.predicate, repository), this->self().template copy_list<pddl::Term>(data.terms, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Literal> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::Literal> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return this->m_storage->builder.literal(this->m_storage->repository, data.positive, this->self().copy(data.atom, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::FunctionExpressionNumber> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::FunctionExpressionNumber> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::FunctionExpressionNumber> out;
    if (find_mapped(this->m_storage->numbers, source, out)) return out;
    out = this->m_storage->builder.number(this->m_storage->repository, repository[source].value).get_index();
    remember(this->m_storage->numbers, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::FunctionTerm> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::FunctionTerm> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::FunctionTerm> out;
    if (find_mapped(this->m_storage->function_terms, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.function_term(this->m_storage->repository, this->self().copy(data.function, repository), this->self().template copy_list<pddl::Term>(data.terms, repository)).get_index();
    remember(this->m_storage->function_terms, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::UnaryFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::UnaryFunctionExpression> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::UnaryFunctionExpression> out;
    if (find_mapped(this->m_storage->unary_expressions, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.unary_function_expression(this->m_storage->repository, data.op, this->self().copy(data.expression, repository)).get_index();
    remember(this->m_storage->unary_expressions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::BinaryFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::BinaryFunctionExpression> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::BinaryFunctionExpression> out;
    if (find_mapped(this->m_storage->binary_expressions, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.binary_function_expression(this->m_storage->repository, data.op, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
    remember(this->m_storage->binary_expressions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::MultiFunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::MultiFunctionExpression> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::MultiFunctionExpression> out;
    if (find_mapped(this->m_storage->multi_expressions, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.multi_function_expression(this->m_storage->repository, data.op, this->self().template copy_list<pddl::FunctionExpression>(data.expressions, repository)).get_index();
    remember(this->m_storage->multi_expressions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::FunctionExpression> BasicCopyTranslator<Derived>::copy(ygg::Index<pddl::FunctionExpression> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::FunctionExpression> out;
    if (find_mapped(this->m_storage->function_expressions, source, out)) return out;
    auto value = std::visit([&](const auto& arg) -> ygg::Data<pddl::FunctionExpression>::Variant { return ygg::Data<pddl::FunctionExpression>::Variant(this->self().copy(arg, repository)); }, repository[source].value);
    out = this->m_storage->builder.function_expression(this->m_storage->repository, std::move(value)).get_index();
    remember(this->m_storage->function_expressions, source, out);
    return out;
}

} // namespace loki2::semantic::detail

#endif
