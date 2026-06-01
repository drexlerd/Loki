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

    ygg::Index<pddl::ConditionLiteral> copy(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionAnd> copy(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionOr> copy(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionNot> copy(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionImply> copy(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionExists> copy(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionForall> copy(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository);

    ygg::Index<pddl::ConditionNumericConstraint> copy(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository);

    ygg::Index<pddl::Condition> copy(ygg::Index<pddl::Condition> source, const pddl::Repository& repository);
};

template<typename Derived>
ygg::Index<pddl::ConditionLiteral> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository)
{
    return pddl::get_or_create<pddl::ConditionLiteral>(this->m_storage->repository, this->self().copy(repository[source].literal, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionAnd> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository)
{
    return pddl::get_or_create<pddl::ConditionAnd>(this->m_storage->repository, this->self().template copy_list<pddl::Condition>(repository[source].conditions, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionOr> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository)
{
    return pddl::get_or_create<pddl::ConditionOr>(this->m_storage->repository, this->self().template copy_list<pddl::Condition>(repository[source].conditions, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionNot> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository)
{
    return pddl::get_or_create<pddl::ConditionNot>(this->m_storage->repository, this->self().copy(repository[source].condition, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionImply> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return pddl::get_or_create<pddl::ConditionImply>(this->m_storage->repository, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionExists> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = this->self().copy(data.condition, repository);
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return pddl::get_or_create<pddl::ConditionExists>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionForall> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = this->self().copy(data.condition, repository);
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return pddl::get_or_create<pddl::ConditionForall>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition).get_index();
}

template<typename Derived>
ygg::Index<pddl::ConditionNumericConstraint> ConditionTranslator<Derived>::copy(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return pddl::get_or_create<pddl::ConditionNumericConstraint>(this->m_storage->repository, data.comparator, this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index();
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionTranslator<Derived>::copy(ygg::Index<pddl::Condition> source, const pddl::Repository& repository)
{
    return this->self().to_dnf(std::visit([&](const auto& arg) { return this->self().copy_condition_node(arg, repository); }, repository[source].value));
}

} // namespace loki::semantic::detail

#endif
