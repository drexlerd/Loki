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
    void collect_free_variables(formalism::TermView term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::AtomView atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::LiteralView literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::FunctionTermView term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::FunctionExpressionNumberView, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const;
    void collect_free_variables(formalism::UnaryFunctionExpressionView expression,
                                std::unordered_set<ygg::uint_t>& bound,
                                std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::BinaryFunctionExpressionView expression,
                                std::unordered_set<ygg::uint_t>& bound,
                                std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::MultiFunctionExpressionView expression,
                                std::unordered_set<ygg::uint_t>& bound,
                                std::unordered_set<ygg::uint_t>& free) const;
    void
    collect_free_variables(formalism::FunctionExpressionView expression, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionView condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionLiteralView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void
    collect_free_variables(formalism::ConditionNumericConstraintView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionNotView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionImplyView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionAndView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionOrView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionExistsView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(formalism::ConditionForallView node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    std::vector<std::pair<ygg::Index<formalism::Parameter>, ygg::Index<formalism::Variable>>>
    free_parameters_in_scope(formalism::ConditionView condition) const;
};

template<typename Derived>
formalism::TermView ConditionAnalysisTranslator<Derived>::term_from_variable(ygg::Index<formalism::Variable> variable)
{
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(variable));
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::TermView term,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    ygg::visit(
        [&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, formalism::VariableView>)
            {
                if (!bound.contains(value.get_index().get_value()))
                    free.insert(value.get_index().get_value());
            }
        },
        term.get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::AtomView atom,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto term : atom.get_terms())
        this->self().collect_free_variables(term, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::LiteralView literal,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(literal.get_atom(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::FunctionTermView term,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto argument : term.get_terms())
        this->self().collect_free_variables(argument, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::FunctionExpressionNumberView,
                                                                  std::unordered_set<ygg::uint_t>&,
                                                                  std::unordered_set<ygg::uint_t>&) const
{
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::UnaryFunctionExpressionView expression,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(expression.get_expression(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::BinaryFunctionExpressionView expression,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(expression.get_left(), bound, free);
    this->self().collect_free_variables(expression.get_right(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::MultiFunctionExpressionView expression,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto part : expression.get_expressions())
        this->self().collect_free_variables(part, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::FunctionExpressionView expression,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    ygg::visit([&](const auto& node) { this->self().collect_free_variables(node, bound, free); }, expression.get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionView condition,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    ygg::visit([&](const auto& node) { this->self().collect_free_variables(node, bound, free); }, condition.get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionLiteralView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(node.get_literal(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionNumericConstraintView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(node.get_left(), bound, free);
    this->self().collect_free_variables(node.get_right(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionNotView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(node.get_condition(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionImplyView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(node.get_left(), bound, free);
    this->self().collect_free_variables(node.get_right(), bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionAndView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : node.get_conditions())
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionOrView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : node.get_conditions())
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionExistsView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto parameter : node.get_parameters())
        bound.insert(parameter.get_variable().get_index().get_value());
    this->self().collect_free_variables(node.get_condition(), bound, free);
    for (auto parameter : node.get_parameters())
        bound.erase(parameter.get_variable().get_index().get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(formalism::ConditionForallView node,
                                                                  std::unordered_set<ygg::uint_t>& bound,
                                                                  std::unordered_set<ygg::uint_t>& free) const
{
    for (auto parameter : node.get_parameters())
        bound.insert(parameter.get_variable().get_index().get_value());
    this->self().collect_free_variables(node.get_condition(), bound, free);
    for (auto parameter : node.get_parameters())
        bound.erase(parameter.get_variable().get_index().get_value());
}

template<typename Derived>
std::vector<std::pair<ygg::Index<formalism::Parameter>, ygg::Index<formalism::Variable>>>
ConditionAnalysisTranslator<Derived>::free_parameters_in_scope(formalism::ConditionView condition) const
{
    auto bound = std::unordered_set<ygg::uint_t> {};
    auto free = std::unordered_set<ygg::uint_t> {};
    this->self().collect_free_variables(condition, bound, free);

    auto result = std::vector<std::pair<ygg::Index<formalism::Parameter>, ygg::Index<formalism::Variable>>> {};
    for (size_t scope_index = 0; scope_index < this->m_active_parameters.size(); ++scope_index)
    {
        const auto& parameters = this->m_active_parameters[scope_index];
        const auto& variables = this->m_active_parameter_variables[scope_index];
        for (size_t i = 0; i < parameters.size(); ++i)
        {
            if (free.contains(variables[i].get_value()))
                result.emplace_back(parameters[i], variables[i]);
        }
    }
    return result;
}

}  // namespace loki::semantic::detail

#endif
