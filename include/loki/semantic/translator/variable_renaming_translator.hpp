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

#ifndef LOKI_SEMANTIC_TRANSLATOR_VARIABLE_RENAMING_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_VARIABLE_RENAMING_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class VariableRenamingTranslator : public CopyTranslatorComponent<Derived, VariableRenamingTranslator<Derived>>
{
public:
    explicit VariableRenamingTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, VariableRenamingTranslator<Derived>>(context) {}

    void enter_variable_scope();
    void leave_variable_scope();
    formalism::ParameterView rename_parameter(formalism::ParameterView source, const formalism::Repository& repository);
    ygg::IndexList<formalism::Parameter> rename_parameters(formalism::EntityListView<formalism::Parameter> source);

    formalism::TermView rename_variables(formalism::TermView source, const formalism::Repository& repository);
    formalism::AtomView rename_variables(formalism::AtomView source, const formalism::Repository& repository);
    formalism::LiteralView rename_variables(formalism::LiteralView source, const formalism::Repository& repository);
    formalism::FunctionTermView rename_variables(formalism::FunctionTermView source, const formalism::Repository& repository);
    formalism::UnaryFunctionExpressionView rename_variables(formalism::UnaryFunctionExpressionView source, const formalism::Repository& repository);
    formalism::BinaryFunctionExpressionView rename_variables(formalism::BinaryFunctionExpressionView source, const formalism::Repository& repository);
    formalism::MultiFunctionExpressionView rename_variables(formalism::MultiFunctionExpressionView source, const formalism::Repository& repository);
    formalism::FunctionExpressionView rename_variables(formalism::FunctionExpressionView source, const formalism::Repository& repository);

    formalism::ConditionLiteralView rename_variables(formalism::ConditionLiteralView source, const formalism::Repository& repository);
    formalism::ConditionAndView rename_variables(formalism::ConditionAndView source, const formalism::Repository& repository);
    formalism::ConditionOrView rename_variables(formalism::ConditionOrView source, const formalism::Repository& repository);
    formalism::ConditionNotView rename_variables(formalism::ConditionNotView source, const formalism::Repository& repository);
    formalism::ConditionImplyView rename_variables(formalism::ConditionImplyView source, const formalism::Repository& repository);
    formalism::ConditionExistsView rename_variables(formalism::ConditionExistsView source, const formalism::Repository& repository);
    formalism::ConditionForallView rename_variables(formalism::ConditionForallView source, const formalism::Repository& repository);
    formalism::ConditionNumericConstraintView rename_variables(formalism::ConditionNumericConstraintView source, const formalism::Repository& repository);
    formalism::ConditionView rename_variables(formalism::ConditionView source, const formalism::Repository& repository);

    formalism::EffectLiteralView rename_variables(formalism::EffectLiteralView source, const formalism::Repository& repository);
    formalism::EffectAndView rename_variables(formalism::EffectAndView source, const formalism::Repository& repository);
    formalism::EffectNumericView rename_variables(formalism::EffectNumericView source, const formalism::Repository& repository);
    formalism::EffectForallView rename_variables(formalism::EffectForallView source, const formalism::Repository& repository);
    formalism::EffectWhenView rename_variables(formalism::EffectWhenView source, const formalism::Repository& repository);
    formalism::EffectOneOfView rename_variables(formalism::EffectOneOfView source, const formalism::Repository& repository);
    formalism::EffectProbabilisticAlternativeView rename_variables(formalism::EffectProbabilisticAlternativeView source,
                                                                   const formalism::Repository& repository);
    formalism::EffectProbabilisticView rename_variables(formalism::EffectProbabilisticView source, const formalism::Repository& repository);
    formalism::EffectView rename_variables(formalism::EffectView source, const formalism::Repository& repository);

    formalism::ActionView rename_action_variables(formalism::ActionView source, const formalism::Repository& repository);
    formalism::AxiomView rename_axiom_variables(formalism::AxiomView source, const formalism::Repository& repository);

private:
    formalism::VariableView lookup_variable(formalism::VariableView source, const formalism::Repository& repository);
    formalism::VariableView fresh_variable(formalism::VariableView source, const formalism::Repository& repository);
};

template<typename Derived>
void VariableRenamingTranslator<Derived>::enter_variable_scope()
{
    this->m_variable_bindings.emplace_back();
}

template<typename Derived>
void VariableRenamingTranslator<Derived>::leave_variable_scope()
{
    this->m_variable_bindings.pop_back();
}

template<typename Derived>
formalism::VariableView VariableRenamingTranslator<Derived>::lookup_variable(formalism::VariableView source, const formalism::Repository& repository)
{
    for (auto it = this->m_variable_bindings.rbegin(); it != this->m_variable_bindings.rend(); ++it)
    {
        if (auto mapped = it->find(source.get_index().get_value()); mapped != it->end())
            return mapped->second;
    }
    return this->self().copy(source, repository);
}

template<typename Derived>
formalism::VariableView VariableRenamingTranslator<Derived>::fresh_variable(formalism::VariableView source, const formalism::Repository&)
{
    auto& counter = this->m_num_quantifications[source.get_index().get_value()];
    auto name = std::string(source.get_name()) + "_" + std::to_string(counter++);
    return formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string(name));
}

template<typename Derived>
formalism::ParameterView VariableRenamingTranslator<Derived>::rename_parameter(formalism::ParameterView source, const formalism::Repository& repository)
{
    const auto variable = this->self().fresh_variable(source.get_variable(), repository);
    this->m_variable_bindings.back().emplace(source.get_variable().get_index().get_value(), variable);
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository,
                                                          variable.get_index(),
                                                          this->self().template copy_list<formalism::Type>(source.get_types()));
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> VariableRenamingTranslator<Derived>::rename_parameters(formalism::EntityListView<formalism::Parameter> source)
{
    auto result = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : source)
        result.push_back(as_index(this->self().rename_parameter(parameter, source.get_context())));
    return result;
}

template<typename Derived>
formalism::TermView VariableRenamingTranslator<Derived>::rename_variables(formalism::TermView source, const formalism::Repository& repository)
{
    auto value = ygg::visit(
        [&](const auto& arg) -> ygg::Data<formalism::Term>::Variant
        {
            using Arg = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Arg, formalism::VariableView>)
                return as_index(this->self().lookup_variable(arg, repository));
            else
                return as_index(this->self().copy(arg, repository));
        },
        source.get_value());
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::AtomView VariableRenamingTranslator<Derived>::rename_variables(formalism::AtomView source, const formalism::Repository& repository)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term, repository)));
    return formalism::get_or_create<formalism::Atom>(this->m_storage->repository,
                                                     as_index(this->self().copy(source.get_predicate(), repository)),
                                                     std::move(terms));
}

template<typename Derived>
formalism::LiteralView VariableRenamingTranslator<Derived>::rename_variables(formalism::LiteralView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository,
                                                        as_index(this->self().rename_variables(source.get_atom(), repository)),
                                                        source.get_data().m_polarity);
}

template<typename Derived>
formalism::FunctionTermView VariableRenamingTranslator<Derived>::rename_variables(formalism::FunctionTermView source, const formalism::Repository& repository)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term, repository)));
    return formalism::get_or_create<formalism::FunctionTerm>(this->m_storage->repository,
                                                             as_index(this->self().copy(source.get_function(), repository)),
                                                             std::move(terms));
}

template<typename Derived>
formalism::UnaryFunctionExpressionView VariableRenamingTranslator<Derived>::rename_variables(formalism::UnaryFunctionExpressionView source,
                                                                                             const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::UnaryFunctionExpression>(this->m_storage->repository,
                                                                        data.op,
                                                                        as_index(this->self().rename_variables(source.get_expression(), repository)));
}

template<typename Derived>
formalism::BinaryFunctionExpressionView VariableRenamingTranslator<Derived>::rename_variables(formalism::BinaryFunctionExpressionView source,
                                                                                              const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::BinaryFunctionExpression>(this->m_storage->repository,
                                                                         data.op,
                                                                         as_index(this->self().rename_variables(source.get_left(), repository)),
                                                                         as_index(this->self().rename_variables(source.get_right(), repository)));
}

template<typename Derived>
formalism::MultiFunctionExpressionView VariableRenamingTranslator<Derived>::rename_variables(formalism::MultiFunctionExpressionView source,
                                                                                             const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    auto expressions = ygg::IndexList<formalism::FunctionExpression> {};
    for (auto expression : source.get_expressions())
        expressions.push_back(as_index(this->self().rename_variables(expression, repository)));
    return formalism::get_or_create<formalism::MultiFunctionExpression>(this->m_storage->repository, data.op, std::move(expressions));
}

template<typename Derived>
formalism::FunctionExpressionView VariableRenamingTranslator<Derived>::rename_variables(formalism::FunctionExpressionView source,
                                                                                        const formalism::Repository& repository)
{
    auto value = ygg::visit(
        [&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant
        {
            using Arg = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Arg, formalism::FunctionExpressionNumberView>)
                return as_index(this->self().copy(arg, repository));
            else
                return as_index(this->self().rename_variables(arg, repository));
        },
        source.get_value());
    return formalism::get_or_create<formalism::FunctionExpression>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::ConditionLiteralView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionLiteralView source,
                                                                                      const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository,
                                                                 as_index(this->self().rename_variables(source.get_literal(), repository)));
}

template<typename Derived>
formalism::ConditionAndView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionAndView source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().rename_variables(condition, repository)));
    return formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionOrView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionOrView source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().rename_variables(condition, repository)));
    return formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionNotView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionNotView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository,
                                                             as_index(this->self().rename_variables(source.get_condition(), repository)));
}

template<typename Derived>
formalism::ConditionImplyView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionImplyView source,
                                                                                    const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository,
                                                               as_index(this->self().rename_variables(source.get_left(), repository)),
                                                               as_index(this->self().rename_variables(source.get_right(), repository)));
}

template<typename Derived>
formalism::ConditionExistsView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionExistsView source,
                                                                                     const formalism::Repository& repository)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto condition = as_index(this->self().rename_variables(source.get_condition(), repository));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition);
}

template<typename Derived>
formalism::ConditionForallView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionForallView source,
                                                                                     const formalism::Repository& repository)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto condition = as_index(this->self().rename_variables(source.get_condition(), repository));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), condition);
}

template<typename Derived>
formalism::ConditionNumericConstraintView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionNumericConstraintView source,
                                                                                                const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository,
                                                                           data.comparator,
                                                                           as_index(this->self().rename_variables(source.get_left(), repository)),
                                                                           as_index(this->self().rename_variables(source.get_right(), repository)));
}

template<typename Derived>
formalism::ConditionView VariableRenamingTranslator<Derived>::rename_variables(formalism::ConditionView source, const formalism::Repository& repository)
{
    auto value =
        ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Condition>::Variant { return as_index(this->self().rename_variables(arg, repository)); },
                   source.get_value());
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::EffectLiteralView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectLiteralView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectLiteral>(this->m_storage->repository,
                                                              as_index(this->self().rename_variables(source.get_literal(), repository)));
}

template<typename Derived>
formalism::EffectAndView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectAndView source, const formalism::Repository& repository)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().rename_variables(effect, repository)));
    return formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectNumericView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectNumericView source, const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term, repository)));
    return formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository,
                                                              data.op,
                                                              as_index(this->self().copy(source.get_function(), repository)),
                                                              std::move(terms),
                                                              as_index(this->self().rename_variables(source.get_expression(), repository)));
}

template<typename Derived>
formalism::EffectForallView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectForallView source, const formalism::Repository& repository)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto effect = as_index(this->self().rename_variables(source.get_effect(), repository));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), effect);
}

template<typename Derived>
formalism::EffectWhenView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectWhenView source, const formalism::Repository& repository)
{
    return formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository,
                                                           as_index(this->self().rename_variables(source.get_condition(), repository)),
                                                           as_index(this->self().rename_variables(source.get_effect(), repository)));
}

template<typename Derived>
formalism::EffectOneOfView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectOneOfView source, const formalism::Repository& repository)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().rename_variables(effect, repository)));
    return formalism::get_or_create<formalism::EffectOneOf>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectProbabilisticAlternativeView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectProbabilisticAlternativeView source,
                                                                                                    const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::EffectProbabilisticAlternative>(this->m_storage->repository,
                                                                               data.probability,
                                                                               as_index(this->self().rename_variables(source.get_effect(), repository)));
}

template<typename Derived>
formalism::EffectProbabilisticView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectProbabilisticView source,
                                                                                         const formalism::Repository& repository)
{
    auto alternatives = ygg::IndexList<formalism::EffectProbabilisticAlternative> {};
    for (auto alternative : source.get_alternatives())
        alternatives.push_back(as_index(this->self().rename_variables(alternative, repository)));
    return formalism::get_or_create<formalism::EffectProbabilistic>(this->m_storage->repository, std::move(alternatives));
}

template<typename Derived>
formalism::EffectView VariableRenamingTranslator<Derived>::rename_variables(formalism::EffectView source, const formalism::Repository& repository)
{
    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return as_index(this->self().rename_variables(arg, repository)); },
                            source.get_value());
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::ActionView VariableRenamingTranslator<Derived>::rename_action_variables(formalism::ActionView source, const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    this->m_num_quantifications.clear();
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
    if (const auto condition = source.get_precondition())
        precondition = as_index(this->self().rename_variables(condition.value(), repository));
    auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
    if (const auto effect_view = source.get_effect())
        effect = as_index(this->self().rename_variables(effect_view.value(), repository));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::Action>(this->m_storage->repository,
                                                       data.name,
                                                       data.original_name,
                                                       std::move(parameters),
                                                       data.original_arity,
                                                       precondition,
                                                       effect);
}

template<typename Derived>
formalism::AxiomView VariableRenamingTranslator<Derived>::rename_axiom_variables(formalism::AxiomView source, const formalism::Repository& repository)
{
    const auto& data = source.get_data();
    this->m_num_quantifications.clear();
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto head = as_index(this->self().rename_variables(source.get_head(), repository));
    auto condition = as_index(this->self().rename_variables(source.get_condition(), repository));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, std::move(parameters), data.original_arity, head, condition);
}

}  // namespace loki::semantic::detail

#endif
