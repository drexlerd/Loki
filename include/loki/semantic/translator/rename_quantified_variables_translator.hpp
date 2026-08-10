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

#ifndef LOKI_SEMANTIC_TRANSLATOR_RENAME_QUANTIFIED_VARIABLES_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_RENAME_QUANTIFIED_VARIABLES_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class RenameQuantifiedVariablesTranslator : public CopyTranslatorComponent<Derived, RenameQuantifiedVariablesTranslator<Derived>>
{
public:
    explicit RenameQuantifiedVariablesTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, RenameQuantifiedVariablesTranslator<Derived>>(context)
    {
    }

    void enter_variable_scope();
    void leave_variable_scope();
    formalism::ParameterView rename_parameter(formalism::ParameterView source);
    ygg::IndexList<formalism::Parameter> rename_parameters(formalism::EntityListView<formalism::Parameter> source);

    formalism::TermView rename_variables(formalism::TermView source);
    formalism::AtomView rename_variables(formalism::AtomView source);
    formalism::LiteralView rename_variables(formalism::LiteralView source);
    formalism::FunctionTermView rename_variables(formalism::FunctionTermView source);
    formalism::UnaryFunctionExpressionView rename_variables(formalism::UnaryFunctionExpressionView source);
    formalism::BinaryFunctionExpressionView rename_variables(formalism::BinaryFunctionExpressionView source);
    formalism::MultiFunctionExpressionView rename_variables(formalism::MultiFunctionExpressionView source);
    formalism::FunctionExpressionView rename_variables(formalism::FunctionExpressionView source);

    formalism::ConditionLiteralView rename_variables(formalism::ConditionLiteralView source);
    formalism::ConditionAndView rename_variables(formalism::ConditionAndView source);
    formalism::ConditionOrView rename_variables(formalism::ConditionOrView source);
    formalism::ConditionNotView rename_variables(formalism::ConditionNotView source);
    formalism::ConditionImplyView rename_variables(formalism::ConditionImplyView source);
    formalism::ConditionExistsView rename_variables(formalism::ConditionExistsView source);
    formalism::ConditionForallView rename_variables(formalism::ConditionForallView source);
    formalism::ConditionNumericConstraintView rename_variables(formalism::ConditionNumericConstraintView source);
    formalism::ConditionView rename_variables(formalism::ConditionView source);

    formalism::EffectLiteralView rename_variables(formalism::EffectLiteralView source);
    formalism::EffectAndView rename_variables(formalism::EffectAndView source);
    formalism::EffectNumericView rename_variables(formalism::EffectNumericView source);
    formalism::EffectForallView rename_variables(formalism::EffectForallView source);
    formalism::EffectWhenView rename_variables(formalism::EffectWhenView source);
    formalism::EffectOneOfView rename_variables(formalism::EffectOneOfView source);
    formalism::EffectProbabilisticAlternativeView rename_variables(formalism::EffectProbabilisticAlternativeView source);
    formalism::EffectProbabilisticView rename_variables(formalism::EffectProbabilisticView source);
    formalism::EffectView rename_variables(formalism::EffectView source);

    formalism::ActionView rename_action_variables(formalism::ActionView source);
    formalism::AxiomView rename_axiom_variables(formalism::AxiomView source);

private:
    formalism::VariableView lookup_variable(formalism::VariableView source);
    formalism::VariableView fresh_variable(formalism::VariableView source);
};

template<typename Derived>
void RenameQuantifiedVariablesTranslator<Derived>::enter_variable_scope()
{
    this->m_variable_bindings.emplace_back();
}

template<typename Derived>
void RenameQuantifiedVariablesTranslator<Derived>::leave_variable_scope()
{
    this->m_variable_bindings.pop_back();
}

template<typename Derived>
formalism::VariableView RenameQuantifiedVariablesTranslator<Derived>::lookup_variable(formalism::VariableView source)
{
    for (auto it = this->m_variable_bindings.rbegin(); it != this->m_variable_bindings.rend(); ++it)
    {
        if (auto mapped = it->find(source); mapped != it->end())
            return mapped->second;
    }
    return this->self().copy(source);
}

template<typename Derived>
formalism::VariableView RenameQuantifiedVariablesTranslator<Derived>::fresh_variable(formalism::VariableView source)
{
    // Pure capture avoidance: keep the name unless this binding shadows an enclosing one.
    // Sibling scopes may share names; the existential-lifting phase uniquifies where scopes
    // actually merge, which keeps renaming idempotent across repeated translations.
    auto shadowed = false;
    for (const auto& scope : this->m_variable_bindings)
        shadowed = shadowed || scope.contains(source);
    if (!shadowed)
        return this->self().copy(source);

    // Pick the first suffix that neither an enclosing binding nor an earlier rename in this
    // action uses; earlier renames may later be lifted into the action parameters, so reusing
    // one would shadow it on the next translation and break idempotence.
    auto taken = this->m_renamed_variable_names;
    for (const auto& scope : this->m_variable_bindings)
        for (const auto& [_, target] : scope)
            taken.insert(std::string(target.get_name()));
    for (auto k = std::size_t { 0 };; ++k)
    {
        auto name = std::string(source.get_name()) + "_" + std::to_string(k);
        if (!taken.contains(name))
        {
            this->m_renamed_variable_names.insert(name);
            return formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string(name));
        }
    }
}

template<typename Derived>
formalism::ParameterView RenameQuantifiedVariablesTranslator<Derived>::rename_parameter(formalism::ParameterView source)
{
    const auto variable = this->self().fresh_variable(source.get_variable());
    this->m_variable_bindings.back().emplace(source.get_variable(), variable);
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository,
                                                          variable.get_index(),
                                                          this->self().template copy_list<formalism::Type>(source.get_types()));
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> RenameQuantifiedVariablesTranslator<Derived>::rename_parameters(formalism::EntityListView<formalism::Parameter> source)
{
    auto result = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : source)
        result.push_back(as_index(this->self().rename_parameter(parameter)));
    return result;
}

template<typename Derived>
formalism::TermView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::TermView source)
{
    auto value = ygg::visit(
        [&](const auto& arg) -> ygg::Data<formalism::Term>::Variant
        {
            using Arg = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Arg, formalism::VariableView>)
                return as_index(this->self().lookup_variable(arg));
            else
                return as_index(this->self().copy(arg));
        },
        source.get_value());
    return formalism::get_or_create<formalism::Term>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::AtomView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::AtomView source)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term)));
    return formalism::get_or_create<formalism::Atom>(this->m_storage->repository, as_index(this->self().copy(source.get_predicate())), std::move(terms));
}

template<typename Derived>
formalism::LiteralView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::LiteralView source)
{
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository,
                                                        as_index(this->self().rename_variables(source.get_atom())),
                                                        source.get_data().m_polarity);
}

template<typename Derived>
formalism::FunctionTermView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::FunctionTermView source)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term)));
    return formalism::get_or_create<formalism::FunctionTerm>(this->m_storage->repository, as_index(this->self().copy(source.get_function())), std::move(terms));
}

template<typename Derived>
formalism::UnaryFunctionExpressionView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::UnaryFunctionExpressionView source)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::UnaryFunctionExpression>(this->m_storage->repository,
                                                                        data.op,
                                                                        as_index(this->self().rename_variables(source.get_expression())));
}

template<typename Derived>
formalism::BinaryFunctionExpressionView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::BinaryFunctionExpressionView source)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::BinaryFunctionExpression>(this->m_storage->repository,
                                                                         data.op,
                                                                         as_index(this->self().rename_variables(source.get_left())),
                                                                         as_index(this->self().rename_variables(source.get_right())));
}

template<typename Derived>
formalism::MultiFunctionExpressionView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::MultiFunctionExpressionView source)
{
    const auto& data = source.get_data();
    auto remaining = ygg::IndexList<formalism::FunctionExpression> {};
    for (auto expression : source.get_remaining())
        remaining.push_back(as_index(this->self().rename_variables(expression)));
    return formalism::get_or_create<formalism::MultiFunctionExpression>(this->m_storage->repository,
                                                                        data.op,
                                                                        as_index(this->self().rename_variables(source.get_first())),
                                                                        as_index(this->self().rename_variables(source.get_second())),
                                                                        std::move(remaining));
}

template<typename Derived>
formalism::FunctionExpressionView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::FunctionExpressionView source)
{
    auto value = ygg::visit(
        [&](const auto& arg) -> ygg::Data<formalism::FunctionExpression>::Variant
        {
            using Arg = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<Arg, formalism::FunctionExpressionNumberView>)
                return as_index(this->self().copy(arg));
            else
                return as_index(this->self().rename_variables(arg));
        },
        source.get_value());
    return formalism::get_or_create<formalism::FunctionExpression>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::ConditionLiteralView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionLiteralView source)
{
    return formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, as_index(this->self().rename_variables(source.get_literal())));
}

template<typename Derived>
formalism::ConditionAndView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionAndView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().rename_variables(condition)));
    return formalism::get_or_create<formalism::ConditionAnd>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionOrView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionOrView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().rename_variables(condition)));
    return formalism::get_or_create<formalism::ConditionOr>(this->m_storage->repository, std::move(conditions));
}

template<typename Derived>
formalism::ConditionNotView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionNotView source)
{
    return formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository, as_index(this->self().rename_variables(source.get_condition())));
}

template<typename Derived>
formalism::ConditionImplyView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionImplyView source)
{
    // Sequence the recursions: both sides can assign collision-rename suffixes, and argument
    // evaluation order is unspecified.
    const auto left = as_index(this->self().rename_variables(source.get_left()));
    const auto right = as_index(this->self().rename_variables(source.get_right()));
    return formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository, left, right);
}

template<typename Derived>
formalism::ConditionExistsView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionExistsView source)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto condition = as_index(this->self().rename_variables(source.get_condition()));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition);
}

template<typename Derived>
formalism::ConditionForallView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionForallView source)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto condition = as_index(this->self().rename_variables(source.get_condition()));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::ConditionForall>(this->m_storage->repository, std::move(parameters), condition);
}

template<typename Derived>
formalism::ConditionNumericConstraintView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionNumericConstraintView source)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::ConditionNumericConstraint>(this->m_storage->repository,
                                                                           data.comparator,
                                                                           as_index(this->self().rename_variables(source.get_left())),
                                                                           as_index(this->self().rename_variables(source.get_right())));
}

template<typename Derived>
formalism::ConditionView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::ConditionView source)
{
    auto value = ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Condition>::Variant { return as_index(this->self().rename_variables(arg)); },
                            source.get_value());
    return formalism::get_or_create<formalism::Condition>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::EffectLiteralView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectLiteralView source)
{
    return formalism::get_or_create<formalism::EffectLiteral>(this->m_storage->repository, as_index(this->self().rename_variables(source.get_literal())));
}

template<typename Derived>
formalism::EffectAndView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectAndView source)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().rename_variables(effect)));
    return formalism::get_or_create<formalism::EffectAnd>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectNumericView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectNumericView source)
{
    const auto& data = source.get_data();
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto term : source.get_terms())
        terms.push_back(as_index(this->self().rename_variables(term)));
    return formalism::get_or_create<formalism::EffectNumeric>(this->m_storage->repository,
                                                              data.op,
                                                              as_index(this->self().copy(source.get_function())),
                                                              std::move(terms),
                                                              as_index(this->self().rename_variables(source.get_expression())));
}

template<typename Derived>
formalism::EffectForallView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectForallView source)
{
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto effect = as_index(this->self().rename_variables(source.get_effect()));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::EffectForall>(this->m_storage->repository, std::move(parameters), effect);
}

template<typename Derived>
formalism::EffectWhenView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectWhenView source)
{
    return formalism::get_or_create<formalism::EffectWhen>(this->m_storage->repository,
                                                           as_index(this->self().rename_variables(source.get_condition())),
                                                           as_index(this->self().rename_variables(source.get_effect())));
}

template<typename Derived>
formalism::EffectOneOfView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectOneOfView source)
{
    auto effects = ygg::IndexList<formalism::Effect> {};
    for (auto effect : source.get_effects())
        effects.push_back(as_index(this->self().rename_variables(effect)));
    return formalism::get_or_create<formalism::EffectOneOf>(this->m_storage->repository, std::move(effects));
}

template<typename Derived>
formalism::EffectProbabilisticAlternativeView
RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectProbabilisticAlternativeView source)
{
    const auto& data = source.get_data();
    return formalism::get_or_create<formalism::EffectProbabilisticAlternative>(this->m_storage->repository,
                                                                               data.probability,
                                                                               as_index(this->self().rename_variables(source.get_effect())));
}

template<typename Derived>
formalism::EffectProbabilisticView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectProbabilisticView source)
{
    auto alternatives = ygg::IndexList<formalism::EffectProbabilisticAlternative> {};
    for (auto alternative : source.get_alternatives())
        alternatives.push_back(as_index(this->self().rename_variables(alternative)));
    return formalism::get_or_create<formalism::EffectProbabilistic>(this->m_storage->repository, std::move(alternatives));
}

template<typename Derived>
formalism::EffectView RenameQuantifiedVariablesTranslator<Derived>::rename_variables(formalism::EffectView source)
{
    auto value =
        ygg::visit([&](const auto& arg) -> ygg::Data<formalism::Effect>::Variant { return as_index(this->self().rename_variables(arg)); }, source.get_value());
    return formalism::get_or_create<formalism::Effect>(this->m_storage->repository, std::move(value));
}

template<typename Derived>
formalism::ActionView RenameQuantifiedVariablesTranslator<Derived>::rename_action_variables(formalism::ActionView source)
{
    const auto& data = source.get_data();
    this->m_num_quantifications.clear();
    this->m_renamed_variable_names.clear();
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto precondition = cista::optional<ygg::Index<formalism::Condition>> {};
    if (const auto condition = source.get_precondition())
        precondition = as_index(this->self().rename_variables(condition.value()));
    auto effect = cista::optional<ygg::Index<formalism::Effect>> {};
    if (const auto effect_view = source.get_effect())
        effect = as_index(this->self().rename_variables(effect_view.value()));
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
formalism::AxiomView RenameQuantifiedVariablesTranslator<Derived>::rename_axiom_variables(formalism::AxiomView source)
{
    const auto& data = source.get_data();
    this->m_num_quantifications.clear();
    this->m_renamed_variable_names.clear();
    this->self().enter_variable_scope();
    auto parameters = this->self().rename_parameters(source.get_parameters());
    auto head = as_index(this->self().rename_variables(source.get_head()));
    auto condition = as_index(this->self().rename_variables(source.get_condition()));
    this->self().leave_variable_scope();
    return formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, std::move(parameters), data.original_arity, head, condition);
}

}  // namespace loki::semantic::detail

#endif
