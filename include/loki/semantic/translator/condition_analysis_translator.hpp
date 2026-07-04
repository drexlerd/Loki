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


#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_ANALYSIS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_ANALYSIS_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionAnalysisTranslator : public CopyTranslatorComponent<Derived, ConditionAnalysisTranslator<Derived>>
{
public:
    explicit ConditionAnalysisTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionAnalysisTranslator<Derived>>(context) {}

    formalism::TermView term_from_variable(ygg::Index<formalism::Variable> variable);
    void collect_free_variables(ygg::Index<formalism::Term> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::Atom> atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::Literal> literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::FunctionTerm> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::FunctionExpressionNumber>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const;
    void collect_free_variables(ygg::Index<formalism::UnaryFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::BinaryFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::MultiFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::FunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::Condition> condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionLiteral> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionNumericConstraint>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const;
    void collect_free_variables(ygg::Index<formalism::ConditionNot> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionImply> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionAnd> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionOr> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionExists> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<formalism::ConditionForall> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    ygg::IndexList<formalism::Parameter> free_parameters_in_scope(ygg::Index<formalism::Condition> condition) const;
};

template<typename Derived>
formalism::TermView ConditionAnalysisTranslator<Derived>::term_from_variable(ygg::Index<formalism::Variable> variable)
{
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(variable));
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::Term> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    std::visit([&](const auto& value)
    {
        using Value = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Value, ygg::Index<formalism::Variable>>)
        {
            if (!bound.contains(value.get_value()))
                free.insert(value.get_value());
        }
    }, this->m_storage->repository[term].value);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::Atom> atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto term : this->m_storage->repository[atom].terms)
        this->self().collect_free_variables(term, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::Literal> literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(this->m_storage->repository[literal].atom, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::FunctionTerm> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto argument : this->m_storage->repository[term].terms)
        this->self().collect_free_variables(argument, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::FunctionExpressionNumber>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const {}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::UnaryFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(this->m_storage->repository[expression].expression, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::BinaryFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[expression];
    this->self().collect_free_variables(data.left, bound, free);
    this->self().collect_free_variables(data.right, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::MultiFunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto part : this->m_storage->repository[expression].expressions)
        this->self().collect_free_variables(part, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::FunctionExpression> expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    std::visit([&](const auto& node) { this->self().collect_free_variables(node, bound, free); }, this->m_storage->repository[expression].value);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::Condition> condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    std::visit([&](const auto& node) { this->self().collect_free_variables(node, bound, free); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionLiteral> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { this->self().collect_free_variables(this->m_storage->repository[node].literal, bound, free); }

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionNumericConstraint> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[node];
    this->self().collect_free_variables(data.left, bound, free);
    this->self().collect_free_variables(data.right, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionNot> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { this->self().collect_free_variables(this->m_storage->repository[node].condition, bound, free); }

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionImply> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(this->m_storage->repository[node].left, bound, free);
    this->self().collect_free_variables(this->m_storage->repository[node].right, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionAnd> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : this->m_storage->repository[node].conditions)
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionOr> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : this->m_storage->repository[node].conditions)
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionExists> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[node];
    for (auto parameter : data.parameters)
        bound.insert(this->m_storage->repository[parameter].variable.get_value());
    this->self().collect_free_variables(data.condition, bound, free);
    for (auto parameter : data.parameters)
        bound.erase(this->m_storage->repository[parameter].variable.get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<formalism::ConditionForall> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[node];
    for (auto parameter : data.parameters)
        bound.insert(this->m_storage->repository[parameter].variable.get_value());
    this->self().collect_free_variables(data.condition, bound, free);
    for (auto parameter : data.parameters)
        bound.erase(this->m_storage->repository[parameter].variable.get_value());
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> ConditionAnalysisTranslator<Derived>::free_parameters_in_scope(ygg::Index<formalism::Condition> condition) const
{
    auto bound = std::unordered_set<ygg::uint_t> {};
    auto free = std::unordered_set<ygg::uint_t> {};
    this->self().collect_free_variables(condition, bound, free);

    auto result = ygg::IndexList<formalism::Parameter> {};
    for (const auto& scope : this->m_active_parameters)
    {
        for (auto parameter : scope)
        {
            if (free.contains(this->m_storage->repository[parameter].variable.get_value()))
                result.push_back(parameter);
        }
    }
    return result;
}

} // namespace loki::semantic::detail

#endif
