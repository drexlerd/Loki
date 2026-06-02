/*
 * Copyright (C) 2026 Dominik Drexler
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

    formalism::ConditionView make_generated_axiom_condition(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView make_generated_positive_condition(ygg::Index<formalism::Condition> condition);
    cista::optional<ygg::Index<formalism::ConditionExists>> as_exists(ygg::Index<formalism::Condition> condition) const;
    formalism::ConditionView move_existentials(ygg::Index<formalism::Condition> condition);
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>);
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>);
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionAnd> node);
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionExists> node);
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionOr> node);
    template<typename T>
    formalism::ConditionView move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>);
    void lift_top_level_exists(ygg::IndexList<formalism::Parameter>& parameters, cista::optional<ygg::Index<formalism::Condition>>& condition);
    void lift_top_level_exists(ygg::IndexList<formalism::Parameter>& parameters, ygg::Index<formalism::Condition>& condition);
};

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::make_generated_positive_condition(ygg::Index<formalism::Condition> condition)
{
    const auto dnf = as_index(this->self().flatten_condition(as_index(this->self().to_dnf(condition))));
    const auto cached = this->m_context.generated_positive_conditions.find(dnf.get_value());
    if (cached != this->m_context.generated_positive_conditions.end())
        return ygg::make_view(cached->second, this->m_storage->repository);

    const auto free_parameters = this->self().free_parameters_in_scope(dnf);
    auto predicate_parameters = ygg::IndexList<formalism::Parameter> {};
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto parameter : free_parameters)
    {
        predicate_parameters.push_back(parameter);
        terms.push_back(as_index(this->self().term_from_variable(this->m_storage->repository[parameter].variable)));
    }

    const auto name = cista::offset::string("_condition_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, this->self().maybe_strip_parameters(predicate_parameters)).get_index();
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate, terms).get_index();
    const auto positive_head = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, true, atom).get_index();

    auto parts = ygg::IndexList<formalism::Condition> {};
    if (const auto condition_or = this->self().as_or(dnf))
    {
        for (auto part : this->m_storage->repository[*condition_or].conditions)
            parts.push_back(part);
    }
    else
    {
        parts.push_back(dnf);
    }

    for (auto part : parts)
    {
        auto axiom_parameters = free_parameters;
        auto axiom_condition = part;
        this->self().lift_top_level_exists(axiom_parameters, axiom_condition);
        this->self().prepend_type_conditions(axiom_condition, axiom_parameters);
        axiom_condition = as_index(this->self().flatten_condition(axiom_condition));
        const auto axiom = formalism::get_or_create<formalism::Axiom>(
            this->m_storage->repository,
            this->self().maybe_strip_parameters(axiom_parameters),
            positive_head,
            axiom_condition).get_index();
        this->m_generated_axioms.push_back(axiom);
    }
    this->m_generated_predicates.push_back(predicate);
    const auto result = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, positive_head).get_index()));
    this->m_context.generated_positive_conditions.emplace(dnf.get_value(), result);
    return ygg::make_view(result, this->m_storage->repository);
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::make_generated_axiom_condition(ygg::Index<formalism::Condition> condition)
{
    const auto dnf = as_index(this->self().flatten_condition(as_index(this->self().to_dnf(condition))));
    const auto cached = this->m_context.generated_axiom_conditions.find(dnf.get_value());
    if (cached != this->m_context.generated_axiom_conditions.end())
        return ygg::make_view(cached->second, this->m_storage->repository);

    const auto free_parameters = this->self().free_parameters_in_scope(dnf);
    auto predicate_parameters = ygg::IndexList<formalism::Parameter> {};
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto parameter : free_parameters)
    {
        predicate_parameters.push_back(parameter);
        terms.push_back(as_index(this->self().term_from_variable(this->m_storage->repository[parameter].variable)));
    }

    const auto name = cista::offset::string("_universal_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, this->self().maybe_strip_parameters(predicate_parameters)).get_index();
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate, terms).get_index();
    const auto positive_head = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, true, atom).get_index();
    const auto negative_literal = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, false, atom).get_index();

    auto parts = ygg::IndexList<formalism::Condition> {};
    if (const auto condition_or = this->self().as_or(dnf))
    {
        for (auto part : this->m_storage->repository[*condition_or].conditions)
            parts.push_back(part);
    }
    else
    {
        parts.push_back(dnf);
    }

    for (auto part : parts)
    {
        auto axiom_parameters = free_parameters;
        auto axiom_condition = part;
        this->self().lift_top_level_exists(axiom_parameters, axiom_condition);
        this->self().prepend_type_conditions(axiom_condition, axiom_parameters);
        axiom_condition = as_index(this->self().flatten_condition(axiom_condition));
        const auto axiom = formalism::get_or_create<formalism::Axiom>(
            this->m_storage->repository,
            this->self().maybe_strip_parameters(axiom_parameters),
            positive_head,
            axiom_condition).get_index();
        this->m_generated_axioms.push_back(axiom);
    }
    this->m_generated_predicates.push_back(predicate);
    const auto result = as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, negative_literal).get_index()));
    this->m_context.generated_axiom_conditions.emplace(dnf.get_value(), result);
    return ygg::make_view(result, this->m_storage->repository);
}

template<typename Derived>
cista::optional<ygg::Index<formalism::ConditionExists>> ConditionQuantifierTranslator<Derived>::as_exists(ygg::Index<formalism::Condition> condition) const
{
    auto result = cista::optional<ygg::Index<formalism::ConditionExists>> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionExists>>)
            result = node;
    }, this->m_storage->repository[condition].value);
    return result;
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials(ygg::Index<formalism::Condition> condition)
{
    return std::visit([&](const auto& node) { return this->self().move_existentials_node(condition, node); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionLiteral>) { return ygg::make_view(condition, this->m_storage->repository); }

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<formalism::ConditionNumericConstraint>) { return ygg::make_view(condition, this->m_storage->repository); }

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionAnd> node)
{
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto moved = as_index(this->self().move_existentials(child));
        if (const auto exists = this->self().as_exists(moved))
        {
            const auto& exists_data = this->m_storage->repository[*exists];
            for (auto parameter : exists_data.parameters)
                parameters.push_back(parameter);
            parts.push_back(exists_data.condition);
        }
        else
        {
            parts.push_back(moved);
        }
    }
    auto conjunction = as_index(this->self().make_conjunction(std::move(parts)));
    if (parameters.empty())
        return ygg::make_view(conjunction, this->m_storage->repository);
    return this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), conjunction).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionExists> node)
{
    const auto& data = this->m_storage->repository[node];
    return this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, data.parameters, as_index(this->self().move_existentials(data.condition))).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition>,ygg::Index<formalism::ConditionOr> node)
{
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
        parts.push_back(as_index(this->self().move_existentials(child)));
    return this->self().make_disjunction(std::move(parts));
}

template<typename Derived>
template<typename T>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::move_existentials_node(ygg::Index<formalism::Condition> condition, ygg::Index<T>)
{
    return ygg::make_view(condition, this->m_storage->repository);
}

template<typename Derived>
void ConditionQuantifierTranslator<Derived>::lift_top_level_exists(ygg::IndexList<formalism::Parameter>& parameters, cista::optional<ygg::Index<formalism::Condition>>& condition)
{
    if (!condition)
        return;
    auto moved = as_index(this->self().move_existentials(*condition));
    if (const auto exists = this->self().as_exists(moved))
    {
        const auto& data = this->m_storage->repository[*exists];
        for (auto parameter : data.parameters)
            parameters.push_back(parameter);
        moved = data.condition;
    }
    condition = as_index(this->self().flatten_condition(moved));
}

template<typename Derived>
void ConditionQuantifierTranslator<Derived>::lift_top_level_exists(ygg::IndexList<formalism::Parameter>& parameters, ygg::Index<formalism::Condition>& condition)
{
    auto optional = cista::optional<ygg::Index<formalism::Condition>>(condition);
    this->self().lift_top_level_exists(parameters, optional);
    condition = *optional;
}

} // namespace loki::semantic::detail

#endif
