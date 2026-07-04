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


#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_NNF_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_NNF_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionNnfTranslator : public CopyTranslatorComponent<Derived, ConditionNnfTranslator<Derived>>
{
public:
    explicit ConditionNnfTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionNnfTranslator<Derived>>(context) {}

    formalism::BinaryComparator negate_comparator(formalism::BinaryComparator comparator);
    formalism::ConditionView negate_condition(ygg::Index<formalism::Condition> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository);
    formalism::ConditionView negate_condition_node(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository);
    formalism::ConditionView copy_condition_node(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository);
};

template<typename Derived>
formalism::BinaryComparator ConditionNnfTranslator<Derived>::negate_comparator(formalism::BinaryComparator comparator)
{
    switch (comparator)
    {
        case formalism::BinaryComparator::Equal:
            return formalism::BinaryComparator::NotEqual;
        case formalism::BinaryComparator::NotEqual:
            return formalism::BinaryComparator::Equal;
        case formalism::BinaryComparator::Less:
            return formalism::BinaryComparator::GreaterEqual;
        case formalism::BinaryComparator::LessEqual:
            return formalism::BinaryComparator::Greater;
        case formalism::BinaryComparator::Greater:
            return formalism::BinaryComparator::LessEqual;
        case formalism::BinaryComparator::GreaterEqual:
            return formalism::BinaryComparator::Less;
    }
    return formalism::BinaryComparator::NotEqual;
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition(ygg::Index<formalism::Condition> source, const formalism::Repository& repository)
{
    return std::visit([&](const auto& arg) { return this->self().negate_condition_node(arg, repository); }, repository[source].value);
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionLiteral> source,
                                                                                const formalism::Repository& repository)
{
    const auto literal = repository[repository[source].literal];
    const auto negated_literal =
        formalism::get_or_create<formalism::Literal>(this->m_storage->repository, as_index(this->self().copy(literal.atom, repository)), !literal.m_polarity)
            .get_index();
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, negated_literal).get_index());
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionAnd> source,
                                                                                const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(as_index(this->self().negate_condition(condition, repository)));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionOr> source,
                                                                                const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(as_index(this->self().negate_condition(condition, repository)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionNot> source,
                                                                                const formalism::Repository& repository)
{
    return this->self().copy(repository[source].condition, repository);
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionImply> source,
                                                                                const formalism::Repository& repository)
{
    const auto& data = repository[source];
    auto conditions = ygg::IndexList<formalism::Condition> {};
    conditions.push_back(as_index(this->self().copy(data.left, repository)));
    conditions.push_back(as_index(this->self().negate_condition(data.right, repository)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionExists> source,
                                                                                const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = as_index(this->self().negate_condition(data.condition, repository));
    this->self().leave_scope();
    return this->self().flatten_condition(as_index(this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), condition).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionForall> source,
                                                                                const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = as_index(this->self().negate_condition(data.condition, repository));
    this->self().leave_scope();
    return this->self().flatten_condition(as_index(this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(ygg::Index<formalism::ConditionNumericConstraint> source,
                                                                                const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository,
                                                                                                       this->self().negate_comparator(data.comparator),
                                                                                                       as_index(this->self().copy(data.left, repository)),
                                                                                                       as_index(this->self().copy(data.right, repository)))
                                           .get_index());
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionLiteral> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionAnd> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionOr> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionNot> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().negate_condition(repository[source].condition, repository);
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionImply> source,
                                                                              const formalism::Repository& repository)
{
    const auto& data = repository[source];
    auto conditions = ygg::IndexList<formalism::Condition> {};
    conditions.push_back(as_index(this->self().negate_condition(data.left, repository)));
    conditions.push_back(as_index(this->self().copy(data.right, repository)));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionExists> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionForall> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(ygg::Index<formalism::ConditionNumericConstraint> source,
                                                                              const formalism::Repository& repository)
{
    return this->self().wrap_condition(as_index(this->self().copy(source, repository)));
}

}  // namespace loki::semantic::detail

#endif
