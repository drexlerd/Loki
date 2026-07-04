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
    formalism::ConditionView negate_condition(formalism::ConditionView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionLiteralView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionAndView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionOrView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionNotView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionImplyView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionExistsView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionForallView source);
    formalism::ConditionView negate_condition_node(formalism::ConditionNumericConstraintView source);
    formalism::ConditionView copy_condition_node(formalism::ConditionNotView source);
    formalism::ConditionView copy_condition_node(formalism::ConditionImplyView source);
    template<typename T>
    formalism::ConditionView copy_condition_node(formalism::EntityView<T> source);
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
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition(formalism::ConditionView source)
{
    return ygg::visit([&](const auto& arg) { return this->self().negate_condition_node(arg); }, source.get_value());
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionLiteralView source)
{
    const auto literal = source.get_literal();
    const auto negated_literal =
        formalism::get_or_create<formalism::Literal>(this->m_storage->repository, as_index(this->self().copy(literal.get_atom())), !literal.get_polarity());
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, negated_literal));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionAndView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().negate_condition(condition)));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionOrView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().negate_condition(condition)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionNotView source)
{
    return this->self().copy(source.get_condition());
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionImplyView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    conditions.push_back(as_index(this->self().copy(source.get_left())));
    conditions.push_back(as_index(this->self().negate_condition(source.get_right())));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionExistsView source)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameter_views = this->self().copy_parameter_views(source.get_parameters());
    auto parameters = this->self().parameter_indices(parameter_views);
    this->self().enter_scope(parameter_views);
    auto condition = as_index(this->self().negate_condition(source.get_condition()));
    this->self().leave_scope();
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), condition)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionForallView source)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameter_views = this->self().copy_parameter_views(source.get_parameters());
    auto parameters = this->self().parameter_indices(parameter_views);
    this->self().enter_scope(parameter_views);
    auto condition = as_index(this->self().negate_condition(source.get_condition()));
    this->self().leave_scope();
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition)));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::negate_condition_node(formalism::ConditionNumericConstraintView source)
{
    const auto& data = source.get_data();
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository,
                                                                                                       this->self().negate_comparator(data.comparator),
                                                                                                       as_index(this->self().copy(source.get_left())),
                                                                                                       as_index(this->self().copy(source.get_right()))));
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(formalism::ConditionNotView source)
{
    return this->self().negate_condition(source.get_condition());
}

template<typename Derived>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(formalism::ConditionImplyView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    conditions.push_back(as_index(this->self().negate_condition(source.get_left())));
    conditions.push_back(as_index(this->self().copy(source.get_right())));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionNnfTranslator<Derived>::copy_condition_node(formalism::EntityView<T> source)
{
    return this->self().wrap_condition(this->self().copy(source));
}

}  // namespace loki::semantic::detail

#endif
