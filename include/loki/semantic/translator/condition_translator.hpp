/*
 * Copyright (C) 2026 Dominik Drexler
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

    ygg::Index<formalism::ConditionLiteral> copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionAnd> copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionOr> copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionNot> copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionImply> copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionExists> copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionForall> copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository);

    ygg::Index<formalism::ConditionNumericConstraint> copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository);

    ygg::Index<formalism::Condition> copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository);
};

template<typename Derived>
ygg::Index<formalism::ConditionLiteral> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, this->self().copy(repository[source].literal, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionAnd> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, this->self().template copy_list<formalism::Condition>(repository[source].conditions, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionOr> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, this->self().template copy_list<formalism::Condition>(repository[source].conditions, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionNot> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository, this->self().copy(repository[source].condition, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionImply> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionExists> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = this->self().copy(data.condition, repository);
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionForall> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = this->self().copy(data.condition, repository);
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition).get_index();
}

template<typename Derived>
ygg::Index<formalism::ConditionNumericConstraint> ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository, data.comparator, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
}

template<typename Derived>
ygg::Index<formalism::Condition> ConditionTranslator<Derived>::copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository)
{
    return this->self().to_dnf(std::visit([&](const auto& arg) { return this->self().copy_condition_node(arg, repository); }, repository[source].value));
}

} // namespace loki::semantic::detail

#endif
