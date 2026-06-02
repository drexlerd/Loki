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

    formalism::ConditionLiteralView copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository);

    formalism::ConditionAndView copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository);

    formalism::ConditionOrView copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository);

    formalism::ConditionNotView copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository);

    formalism::ConditionImplyView copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository);

    formalism::ConditionExistsView copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository);

    formalism::ConditionForallView copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository);

    formalism::ConditionNumericConstraintView copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository);

    formalism::ConditionView copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository);
};

template<typename Derived>
formalism::ConditionLiteralView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, as_index(this->self().copy(repository[source].literal, repository)));
}

template<typename Derived>
formalism::ConditionAndView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, this->self().template copy_list<formalism::Condition>(repository[source].conditions, repository));
}

template<typename Derived>
formalism::ConditionOrView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, this->self().template copy_list<formalism::Condition>(repository[source].conditions, repository));
}

template<typename Derived>
formalism::ConditionNotView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository, as_index(this->self().copy(repository[source].condition, repository)));
}

template<typename Derived>
formalism::ConditionImplyView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository, as_index(this->self().copy(data.left, repository)), as_index(this->self().copy(data.right, repository)));
}

template<typename Derived>
formalism::ConditionExistsView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = as_index(this->self().copy(data.condition, repository));
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition);
}

template<typename Derived>
formalism::ConditionForallView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = as_index(this->self().copy(data.condition, repository));
    this->self().leave_scope();
    auto typed_condition = condition;
    this->self().prepend_type_conditions(typed_condition, parameters);
    return formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), typed_condition);
}

template<typename Derived>
formalism::ConditionNumericConstraintView ConditionTranslator<Derived>::copy(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository, data.comparator, as_index(this->self().copy(data.left, repository)), as_index(this->self().copy(data.right, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionTranslator<Derived>::copy(ygg::Index<formalism::Condition> source, const formalism::Repository& repository)
{
    return this->self().flatten_condition(as_index(this->self().to_dnf(as_index(std::visit([&](const auto& arg) { return this->self().copy_condition_node(arg, repository); }, repository[source].value)))));
}

} // namespace loki::semantic::detail

#endif
