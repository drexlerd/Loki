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
    formalism::ConditionView remove_universal_quantifiers(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository);
    formalism::ConditionView remove_universal_quantifiers_node(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository);
    std::optional<formalism::ConditionExistsView> as_exists(ygg::Index<formalism::Condition> condition) const;
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
formalism::ConditionView ConditionQuantifierTranslator<Derived>::make_generated_axiom_condition(ygg::Index<formalism::Condition> condition)
{
    const auto free_parameters = this->self().free_parameters_in_scope(condition);

    auto key = std::string("condition:") + std::to_string(condition.get_value());
    for (auto parameter : free_parameters)
    {
        key += "|parameter:";
        key += std::to_string(parameter.get_value());
    }
    if (auto it = this->m_generated_universal_conditions.find(key); it != this->m_generated_universal_conditions.end())
        return it->second;

    auto predicate_parameters = ygg::IndexList<formalism::Parameter> {};
    auto terms = ygg::IndexList<formalism::Term> {};
    for (auto parameter : free_parameters)
    {
        predicate_parameters.push_back(parameter);
        terms.push_back(as_index(this->self().term_from_variable(this->m_storage->repository[parameter].variable)));
    }

    const auto name = cista::offset::string("_universal_" + std::to_string(this->m_num_generated_axioms++));
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, name, predicate_parameters).get_index();
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate, terms).get_index();
    const auto positive_head = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, true, atom).get_index();
    const auto negative_literal = formalism::get_or_create<formalism::Literal>(this->m_storage->repository, false, atom).get_index();
    const auto axiom = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, predicate_parameters, positive_head, condition).get_index();

    this->m_generated_predicates.push_back(predicate);
    this->m_generated_axioms.push_back(axiom);
    auto result = this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, negative_literal));
    this->m_generated_universal_conditions.emplace(std::move(key), result);
    return result;
}


template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers(ygg::Index<formalism::Condition> condition, const formalism::Repository& repository)
{
    return std::visit([&](const auto& node) { return this->self().remove_universal_quantifiers_node(node, repository); }, repository[condition].value);
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionLiteral> source, const formalism::Repository& repository) { return this->self().wrap_condition(as_index(this->self().copy(source, repository))); }

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionAnd> source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(as_index(this->self().remove_universal_quantifiers(condition, repository)));
    return this->self().make_conjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionOr> source, const formalism::Repository& repository)
{
    auto conditions = ygg::IndexList<formalism::Condition> {};
    for (auto condition : repository[source].conditions)
        conditions.push_back(as_index(this->self().remove_universal_quantifiers(condition, repository)));
    return this->self().make_disjunction(std::move(conditions));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionNot> source, const formalism::Repository& repository)
{
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionNot>(this->m_storage->repository, as_index(this->self().remove_universal_quantifiers(repository[source].condition, repository))).get_index());
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionImply> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionImply>(this->m_storage->repository, as_index(this->self().remove_universal_quantifiers(data.left, repository)), as_index(this->self().remove_universal_quantifiers(data.right, repository))).get_index());
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionExists> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = as_index(this->self().remove_universal_quantifiers(data.condition, repository));
    this->self().leave_scope();
    return this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), condition).get_index())));
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionForall> source, const formalism::Repository& repository)
{
    const auto& data = repository[source];
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto negated = as_index(this->self().negate_condition(data.condition, repository));
    this->self().leave_scope();
    const auto exists_not = as_index(this->self().flatten_condition(as_index(this->self().wrap_condition(formalism::get_or_create<formalism::ConditionExists>(this->m_storage->repository, std::move(parameters), negated).get_index()))));
    const auto translated_exists_not = as_index(this->self().remove_universal_quantifiers(exists_not, this->m_storage->repository));
    return this->self().make_generated_axiom_condition(translated_exists_not);
}

template<typename Derived>
formalism::ConditionView ConditionQuantifierTranslator<Derived>::remove_universal_quantifiers_node(ygg::Index<formalism::ConditionNumericConstraint> source, const formalism::Repository& repository) { return this->self().wrap_condition(as_index(this->self().copy(source, repository))); }

template<typename Derived>
std::optional<formalism::ConditionExistsView> ConditionQuantifierTranslator<Derived>::as_exists(ygg::Index<formalism::Condition> condition) const
{
    auto result = std::optional<formalism::ConditionExistsView> {};
    std::visit([&](const auto& node)
    {
        using Node = std::decay_t<decltype(node)>;
        if constexpr (std::is_same_v<Node, ygg::Index<formalism::ConditionExists>>)
            result = ygg::make_view(node, this->m_storage->repository);
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
    auto seen_parameters = std::unordered_set<ygg::uint_t> {};
    auto parts = ygg::IndexList<formalism::Condition> {};
    for (auto child : this->m_storage->repository[node].conditions)
    {
        const auto moved = as_index(this->self().move_existentials(child));
        if (const auto exists = this->self().as_exists(moved))
        {
            const auto& exists_data = exists->get_data();
            for (auto parameter : exists_data.parameters)
            {
                if (seen_parameters.insert(parameter.get_value()).second)
                    parameters.push_back(parameter);
            }
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
        auto seen_parameters = std::unordered_set<ygg::uint_t> {};
        for (auto parameter : parameters)
            seen_parameters.insert(parameter.get_value());

        const auto& data = exists->get_data();
        for (auto parameter : data.parameters)
        {
            if (seen_parameters.insert(parameter.get_value()).second)
                parameters.push_back(parameter);
        }
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
