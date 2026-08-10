/*
 * Copyright (C) 2026 Dominik Drexler
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

#ifndef LOKI_SEMANTIC_TRANSLATOR_NORMALIZE_ARITHMETIC_EXPRESSIONS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_NORMALIZE_ARITHMETIC_EXPRESSIONS_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace loki::semantic::detail
{

template<typename Derived>
class NormalizeArithmeticExpressionsTranslator : public CopyTranslatorComponent<Derived, NormalizeArithmeticExpressionsTranslator<Derived>>
{
public:
    explicit NormalizeArithmeticExpressionsTranslator(CopyContext& context) :
        CopyTranslatorComponent<Derived, NormalizeArithmeticExpressionsTranslator<Derived>>(context)
    {
    }

    formalism::FunctionExpressionView normalize_arithmetic_expression(formalism::FunctionExpressionView source);

private:
    template<typename T>
    formalism::FunctionExpressionView wrap(T value);

    formalism::FunctionExpressionView normalize_node(formalism::FunctionExpressionNumberView source);
    formalism::FunctionExpressionView normalize_node(formalism::FunctionTermView source);
    formalism::FunctionExpressionView normalize_node(formalism::UnaryFunctionExpressionView source);
    formalism::FunctionExpressionView normalize_node(formalism::BinaryFunctionExpressionView source);
    formalism::FunctionExpressionView normalize_node(formalism::MultiFunctionExpressionView source);

    formalism::FunctionExpressionView make_number(double value);
    formalism::FunctionExpressionView normalize_associative(formalism::MultiArithmeticOperator op,
                                                            const std::vector<formalism::FunctionExpressionView>& source_operands);
    void append_source_operand(formalism::MultiArithmeticOperator op,
                               formalism::FunctionExpressionView source,
                               std::vector<formalism::FunctionExpressionView>& operands);
    void append_normalized_operand(formalism::MultiArithmeticOperator op,
                                   formalism::FunctionExpressionView source,
                                   std::vector<formalism::FunctionExpressionView>& operands);
    bool is_unit(formalism::MultiArithmeticOperator op, formalism::FunctionExpressionView expression) const;
};

template<typename Derived>
template<typename T>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::wrap(T value)
{
    auto data = this->template checkout<formalism::FunctionExpression>();
    data->value = ygg::Data<formalism::FunctionExpression>::Variant(value.get_index());
    return formalism::get_or_create(this->m_storage->repository, *data).first;
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_arithmetic_expression(formalism::FunctionExpressionView source)
{
    if (const auto mapped = find_mapped(this->m_storage->function_expressions, source))
        return *mapped;
    const auto normalized = ygg::visit([this](const auto& node) { return normalize_node(node); }, source.get_value());
    remember(this->m_storage->function_expressions, source, normalized);
    return normalized;
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_node(formalism::FunctionExpressionNumberView source)
{
    return wrap(this->self().copy(source));
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_node(formalism::FunctionTermView source)
{
    return wrap(this->self().copy(source));
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_node(formalism::UnaryFunctionExpressionView source)
{
    const auto expression = normalize_arithmetic_expression(source.get_expression());
    auto data = this->template checkout<formalism::UnaryFunctionExpression>();
    data->op = source.get_operator();
    data->expression = expression.get_index();
    return wrap(formalism::get_or_create(this->m_storage->repository, *data).first);
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_node(formalism::BinaryFunctionExpressionView source)
{
    switch (source.get_operator())
    {
        case formalism::BinaryArithmeticOperator::Add:
            return normalize_associative(formalism::MultiArithmeticOperator::Add, { source.get_left(), source.get_right() });
        case formalism::BinaryArithmeticOperator::Mul:
            return normalize_associative(formalism::MultiArithmeticOperator::Mul, { source.get_left(), source.get_right() });
        case formalism::BinaryArithmeticOperator::Sub:
        case formalism::BinaryArithmeticOperator::Div:
        {
            const auto left = normalize_arithmetic_expression(source.get_left());
            const auto right = normalize_arithmetic_expression(source.get_right());
            auto data = this->template checkout<formalism::BinaryFunctionExpression>();
            data->op = source.get_operator();
            data->left = left.get_index();
            data->right = right.get_index();
            return wrap(formalism::get_or_create(this->m_storage->repository, *data).first);
        }
    }
    throw std::invalid_argument("invalid BinaryArithmeticOperator");
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::normalize_node(formalism::MultiFunctionExpressionView source)
{
    auto operands = std::vector<formalism::FunctionExpressionView> {};
    operands.reserve(source.get_args().size());
    for (const auto expression : source.get_args())
        operands.push_back(expression);
    return normalize_associative(source.get_operator(), operands);
}

template<typename Derived>
formalism::FunctionExpressionView NormalizeArithmeticExpressionsTranslator<Derived>::make_number(double value)
{
    auto data = this->template checkout<formalism::FunctionExpressionNumber>();
    data->value = value;
    return wrap(formalism::get_or_create(this->m_storage->repository, *data).first);
}

template<typename Derived>
formalism::FunctionExpressionView
NormalizeArithmeticExpressionsTranslator<Derived>::normalize_associative(formalism::MultiArithmeticOperator op,
                                                                         const std::vector<formalism::FunctionExpressionView>& source_operands)
{
    auto operands = std::vector<formalism::FunctionExpressionView> {};
    for (const auto operand : source_operands)
        append_source_operand(op, operand, operands);

    if (operands.empty())
        return make_number(op == formalism::MultiArithmeticOperator::Add ? 0.0 : 1.0);
    if (operands.size() == 1)
        return operands.front();

    auto data = this->template checkout<formalism::MultiFunctionExpression>();
    data->op = op;
    for (const auto operand : operands)
        data->args.push_back(operand.get_index());
    return wrap(formalism::get_or_create(this->m_storage->repository, *data).first);
}

template<typename Derived>
void NormalizeArithmeticExpressionsTranslator<Derived>::append_source_operand(formalism::MultiArithmeticOperator op,
                                                                              formalism::FunctionExpressionView source,
                                                                              std::vector<formalism::FunctionExpressionView>& operands)
{
    const auto variant = source.get_variant();
    if (variant.template is<ygg::Index<formalism::BinaryFunctionExpression>>())
    {
        const auto binary = variant.template get<ygg::Index<formalism::BinaryFunctionExpression>>();
        if ((op == formalism::MultiArithmeticOperator::Add && binary.get_operator() == formalism::BinaryArithmeticOperator::Add)
            || (op == formalism::MultiArithmeticOperator::Mul && binary.get_operator() == formalism::BinaryArithmeticOperator::Mul))
        {
            append_source_operand(op, binary.get_left(), operands);
            append_source_operand(op, binary.get_right(), operands);
            return;
        }
    }
    else if (variant.template is<ygg::Index<formalism::MultiFunctionExpression>>())
    {
        const auto multi = variant.template get<ygg::Index<formalism::MultiFunctionExpression>>();
        if (multi.get_operator() == op)
        {
            for (const auto expression : multi.get_args())
                append_source_operand(op, expression, operands);
            return;
        }
    }
    append_normalized_operand(op, source, operands);
}

template<typename Derived>
void NormalizeArithmeticExpressionsTranslator<Derived>::append_normalized_operand(formalism::MultiArithmeticOperator op,
                                                                                  formalism::FunctionExpressionView source,
                                                                                  std::vector<formalism::FunctionExpressionView>& operands)
{
    const auto normalized = normalize_arithmetic_expression(source);
    const auto variant = normalized.get_variant();
    if (variant.template is<ygg::Index<formalism::MultiFunctionExpression>>())
    {
        const auto multi = variant.template get<ygg::Index<formalism::MultiFunctionExpression>>();
        if (multi.get_operator() == op)
        {
            for (const auto expression : multi.get_args())
                operands.push_back(expression);
            return;
        }
    }
    if (!is_unit(op, normalized))
        operands.push_back(normalized);
}

template<typename Derived>
bool NormalizeArithmeticExpressionsTranslator<Derived>::is_unit(formalism::MultiArithmeticOperator op, formalism::FunctionExpressionView expression) const
{
    auto unit = false;
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::FunctionExpressionNumberView>)
                unit = node.get_value() == (op == formalism::MultiArithmeticOperator::Add ? 0.0 : 1.0);
        },
        expression.get_value());
    return unit;
}

}  // namespace loki::semantic::detail

#endif
