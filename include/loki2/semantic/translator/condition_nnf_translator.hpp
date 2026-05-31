/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_CONDITION_NNF_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_CONDITION_NNF_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class ConditionNnfTranslator : public CopyTranslatorComponent<Derived, ConditionNnfTranslator<Derived>>
{
public:
    explicit ConditionNnfTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionNnfTranslator<Derived>>(context) {}

    pddl::BinaryComparator negate_comparator(pddl::BinaryComparator comparator);
    ygg::Index<pddl::Condition> negate_condition(ygg::Index<pddl::Condition> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> negate_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository);
    ygg::Index<pddl::Condition> copy_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository);
};

template<typename Derived>
pddl::BinaryComparator ConditionNnfTranslator<Derived>::negate_comparator(pddl::BinaryComparator comparator)
{
    switch (comparator)
    {
        case pddl::BinaryComparator::Equal: return pddl::BinaryComparator::NotEqual;
        case pddl::BinaryComparator::NotEqual: return pddl::BinaryComparator::Equal;
        case pddl::BinaryComparator::Less: return pddl::BinaryComparator::GreaterEqual;
        case pddl::BinaryComparator::LessEqual: return pddl::BinaryComparator::Greater;
        case pddl::BinaryComparator::Greater: return pddl::BinaryComparator::LessEqual;
        case pddl::BinaryComparator::GreaterEqual: return pddl::BinaryComparator::Less;
    }
    return pddl::BinaryComparator::NotEqual;
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition(ygg::Index<pddl::Condition> source, const pddl::Repository& repository)
{
    return std::visit([&](const auto& arg) { return this->self().negate_condition_node(arg, repository); }, repository[source].value);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository)
{
    const auto literal = repository[repository[source].literal];
    const auto negated_literal = this->m_storage->builder.literal(this->m_storage->repository, !literal.positive, this->self().copy(literal.atom, repository)).get_index();
    return this->self().wrap_condition(this->m_storage->builder.condition_literal(this->m_storage->repository, negated_literal).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository)
{
    auto conditions = ygg::IndexList<pddl::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(this->self().negate_condition(condition, repository));
    return this->self().wrap_condition(this->m_storage->builder.condition_or(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository)
{
    auto conditions = ygg::IndexList<pddl::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(this->self().negate_condition(condition, repository));
    return this->self().wrap_condition(this->m_storage->builder.condition_and(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository)
{
    return this->self().copy(repository[source].condition, repository);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    auto conditions = ygg::IndexList<pddl::Condition> {};
    conditions.push_back(this->self().copy(data.left, repository));
    conditions.push_back(this->self().negate_condition(data.right, repository));
    return this->self().wrap_condition(this->m_storage->builder.condition_and(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto negated = this->self().negate_condition(data.condition, repository);
    this->self().leave_scope();
    const auto exists_not = this->self().wrap_condition(this->m_storage->builder.condition_exists(this->m_storage->repository, std::move(parameters), negated).get_index());
    return this->self().make_generated_axiom_condition(exists_not);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto condition = this->self().negate_condition(data.condition, repository);
    this->self().leave_scope();
    return this->self().wrap_condition(this->m_storage->builder.condition_exists(this->m_storage->repository, std::move(parameters), condition).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    return this->self().wrap_condition(this->m_storage->builder.condition_numeric_constraint(this->m_storage->repository, this->self().negate_comparator(data.comparator), this->self().copy(data.left, repository), this->self().copy(data.right, repository)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionLiteral> source, const pddl::Repository& repository) { return this->self().wrap_condition(this->self().copy(source, repository)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionAnd> source, const pddl::Repository& repository) { return this->self().wrap_condition(this->self().copy(source, repository)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionOr> source, const pddl::Repository& repository) { return this->self().wrap_condition(this->self().copy(source, repository)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionNot> source, const pddl::Repository& repository) { return this->self().negate_condition(repository[source].condition, repository); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionImply> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    auto conditions = ygg::IndexList<pddl::Condition> {};
    conditions.push_back(this->self().negate_condition(data.left, repository));
    conditions.push_back(this->self().copy(data.right, repository));
    return this->self().wrap_condition(this->m_storage->builder.condition_or(this->m_storage->repository, std::move(conditions)).get_index());
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionExists> source, const pddl::Repository& repository) { return this->self().wrap_condition(this->self().copy(source, repository)); }

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionForall> source, const pddl::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    const auto negated = this->self().negate_condition(data.condition, repository);
    this->self().leave_scope();
    const auto exists_not = this->self().wrap_condition(this->m_storage->builder.condition_exists(this->m_storage->repository, std::move(parameters), negated).get_index());
    return this->self().make_generated_axiom_condition(exists_not);
}

template<typename Derived>
ygg::Index<pddl::Condition> ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<pddl::ConditionNumericConstraint> source, const pddl::Repository& repository) { return this->self().wrap_condition(this->self().copy(source, repository)); }

} // namespace loki2::semantic::detail

#endif
