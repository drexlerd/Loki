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

#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_QUANTIFIER_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_QUANTIFIER_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionQuantifierTranslator : public CopyTranslatorComponent<Derived, ConditionQuantifierTranslator<Derived>>
{
public:
    explicit ConditionQuantifierTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionQuantifierTranslator<Derived>>(context) {}

    formalism::ConditionView make_generated_axiom_condition(formalism::ConditionView condition);
    formalism::ConditionView remove_universal_quantifiers(formalism::ConditionView condition);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionAndView source);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionOrView source);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionNotView source);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionImplyView source);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionExistsView source);
    formalism::ConditionView remove_universal_quantifiers_node(formalism::ConditionForallView source);
    template<typename T>
    formalism::ConditionView remove_universal_quantifiers_node(formalism::EntityView<T> source);
    std::optional<formalism::ConditionExistsView> as_exists(formalism::ConditionView condition) const;
    formalism::ConditionView move_existentials(formalism::ConditionView condition);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionAndView node);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionExistsView node);
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::ConditionOrView node);
    template<typename T>
    formalism::ConditionView move_existentials_node(formalism::ConditionView condition, formalism::EntityView<T>);
    formalism::ConditionView lift_top_level_exists(std::vector<formalism::ParameterView>& parameters, formalism::ConditionView condition);
};

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::make_generated_axiom_condition(formalism::ConditionView condition)
{
    const auto free_parameters = this->self().free_parameters_in_scope(condition);

    if (auto it = this->m_generated_universal_conditions.find(condition); it != this->m_generated_universal_conditions.end())
        return it->second;

    auto predicate_parameters = ygg::IndexList<formalism::Parameter> {};
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto [parameter, variable] : free_parameters)
    {
        predicate_parameters.push_back(parameter.get_index());
        terms.push_back(as_index(this->self().term_from_variable(variable)));
    }

    const auto name = cista::offset::string(this->self().next_generated_predicate_name("loki-universal-"));
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, predicate_parameters);
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate.get_index(), terms).get_index();
    const auto positive_head = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, atom, true).get_index();
    const auto negative_literal = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, atom, false).get_index();
    const auto axiom = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, predicate_parameters, positive_head, condition.get_index());

    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    auto result = this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, negative_literal));
    this->m_generated_universal_conditions.emplace(condition, result);
    return result;
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers(formalism::ConditionView condition)
{
    return ygg::visit([&](const auto& node) { return this->self().remove_universal_quantifiers_node(node); }, condition.get_value());
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionAndView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().remove_universal_quantifiers(condition)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionOrView source)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : source.get_conditions())
        conditions.push_back(as_index(this->self().remove_universal_quantifiers(condition)));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionNotView source)
{
    return this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository,
                                                          as_index(this->self().remove_universal_quantifiers(source.get_condition()))));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionImplyView source)
{
    return this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository,
                                                            as_index(this->self().remove_universal_quantifiers(source.get_left())),
                                                            as_index(this->self().remove_universal_quantifiers(source.get_right()))));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionExistsView source)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameter_views = this->self().copy_parameter_views(source.get_parameters());
    auto parameters = this->self().parameter_indices(parameter_views);
    this->self().enter_scope(parameter_views);
    auto condition = as_index(this->self().remove_universal_quantifiers(source.get_condition()));
    this->self().leave_scope();
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition)));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::ConditionForallView source)
{
    this->self().increment_quantifications(source.get_parameters());
    auto parameter_views = this->self().copy_parameter_views(source.get_parameters());
    auto parameters = this->self().parameter_indices(parameter_views);
    this->self().enter_scope(parameter_views);
    auto negated = as_index(this->self().negate_condition(source.get_condition()));
    this->self().leave_scope();
    const auto exists_not = this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), negated)));
    const auto translated_exists_not = this->self().remove_universal_quantifiers(exists_not);
    return this->self().make_generated_axiom_condition(translated_exists_not);
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(formalism::EntityView<T> source)
{
    return this->self().wrap_condition(this->self().copy(source));
}

template<typename Derived>
std::optional<formalism::ConditionExistsView> ConditionQuantifierTranslator<Derived>::as_exists(formalism::ConditionView condition) const
{
    auto result = std::optional<formalism::ConditionExistsView> {};
    ygg::visit(
        [&](const auto& node)
        {
            using Node = std::decay_t<decltype(node)>;
            if constexpr (std::is_same_v<Node, formalism::ConditionExistsView>)
                result = node;
        },
        condition.get_value());
    return result;
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials(formalism::ConditionView condition)
{
    return ygg::visit([&](const auto& node) { return this->self().move_existentials_node(condition, node); }, condition.get_value());
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionAndView node)
{
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    auto seen_parameters = ygg::UnorderedSet<formalism::ParameterView> {};
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
    {
        const auto moved = this->self().move_existentials(child);
        if (const auto exists = this->self().as_exists(moved))
        {
            for (auto parameter : exists->get_parameters())
            {
                if (seen_parameters.insert(parameter).second)
                    parameters.push_back(parameter.get_index());
            }
            parts.push_back(exists->get_condition().get_index());
        }
        else
        {
            parts.push_back(moved.get_index());
        }
    }
    auto conjunction = this->self().make_conjunction(std::move(parts));
    if (parameters.empty())
        return conjunction;
    return this->self().flatten_condition(this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), conjunction.get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionExistsView node)
{
    const auto& data = node.get_data();
    const auto condition = this->self().move_existentials(node.get_condition());
    return this->self().flatten_condition(
        this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, condition.get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(formalism::ConditionView, formalism::ConditionOrView node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : node.get_conditions())
        parts.push_back(this->self().move_existentials(child).get_index());
    return this->self().make_disjunction(std::move(parts));
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(formalism::ConditionView condition, formalism::EntityView<T>)
{
    return condition;
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::lift_top_level_exists(std::vector<formalism::ParameterView>& parameters,
                                                                                       formalism::ConditionView condition)
{
    auto moved = this->self().move_existentials(condition);
    if (const auto exists = this->self().as_exists(moved))
    {
        auto seen_parameters = ygg::UnorderedSet<formalism::ParameterView> {};
        for (auto parameter : parameters)
            seen_parameters.insert(parameter);

        for (auto parameter : exists->get_parameters())
        {
            if (seen_parameters.insert(parameter).second)
                parameters.push_back(parameter);
        }
        moved = exists->get_condition();
    }
    return this->self().flatten_condition(moved);
}

}  // namespace loki::semantic::detail

#endif
