/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_EQUALITY_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_EQUALITY_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class EqualityTranslator : public CopyTranslatorComponent<Derived, EqualityTranslator<Derived>>
{
public:
    explicit EqualityTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EqualityTranslator<Derived>>(context) {}

    bool has_requirement(const ygg::IndexList<pddl::Requirement>& requirements, pddl::RequirementKind kind) const;
    bool equality_required(const ygg::Data<pddl::Task>& task) const;
    ygg::Index<pddl::Predicate> ensure_equality_predicate();
    ygg::Index<pddl::Literal> equality_literal(ygg::Index<pddl::Predicate> predicate, ygg::Index<pddl::Object> object);
    bool reflexive_equality_object_name(ygg::Index<pddl::Literal> literal, std::string& name) const;
    void compact_reflexive_equalities(ygg::Data<pddl::Task>& task) const;
    void initialize_equality(ygg::Data<pddl::Task>& task);
};

template<typename Derived>
bool EqualityTranslator<Derived>::has_requirement(const ygg::IndexList<pddl::Requirement>& requirements, pddl::RequirementKind kind) const
{
    for (auto requirement : requirements)
        if (this->m_storage->repository[requirement].kind == kind)
            return true;
    return false;
}

template<typename Derived>
bool EqualityTranslator<Derived>::equality_required(const ygg::Data<pddl::Task>& task) const
{
    if (this->self().has_requirement(task.requirements, pddl::RequirementKind::Equality))
        return true;
    const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
    return this->self().has_requirement(domain.requirements, pddl::RequirementKind::Equality);
}

template<typename Derived>
ygg::Index<pddl::Predicate> EqualityTranslator<Derived>::ensure_equality_predicate()
{
    if (this->m_equality_predicate)
        return *this->m_equality_predicate;
    auto domain = this->m_storage->repository[this->m_storage->translated_domain];
    for (auto predicate : domain.predicates)
    {
        if (std::string(this->m_storage->repository[predicate].name) == "=")
        {
            this->m_equality_predicate = predicate;
            return predicate;
        }
    }
    auto parameters = ygg::IndexList<pddl::Parameter> {};
    auto types = ygg::IndexList<pddl::Type> {};
    for (auto type : domain.types)
    {
        if (std::string(this->m_storage->repository[type].name) == "object")
        {
            types.push_back(type);
            break;
        }
    }
    const auto left = this->m_storage->builder.variable(this->m_storage->repository, cista::offset::string("lhs")).get_index();
    const auto right = this->m_storage->builder.variable(this->m_storage->repository, cista::offset::string("rhs")).get_index();
    parameters.push_back(this->m_storage->builder.parameter(this->m_storage->repository, left, types).get_index());
    parameters.push_back(this->m_storage->builder.parameter(this->m_storage->repository, right, types).get_index());
    const auto predicate = this->m_storage->builder.predicate(this->m_storage->repository, cista::offset::string("="), std::move(parameters)).get_index();
    domain.predicates.push_back(predicate);
    this->m_storage->translated_domain = this->m_storage->builder.domain(this->m_storage->repository, std::move(domain)).get_index();
    this->m_equality_predicate = predicate;
    return predicate;
}

template<typename Derived>
ygg::Index<pddl::Literal> EqualityTranslator<Derived>::equality_literal(ygg::Index<pddl::Predicate> predicate, ygg::Index<pddl::Object> object)
{
    auto terms = ygg::IndexList<pddl::Term> {};
    const auto term = this->m_storage->builder.term(this->m_storage->repository, ygg::Data<pddl::Term>::Variant(object)).get_index();
    terms.push_back(term);
    terms.push_back(term);
    const auto atom = this->m_storage->builder.atom(this->m_storage->repository, predicate, std::move(terms)).get_index();
    return this->m_storage->builder.literal(this->m_storage->repository, true, atom).get_index();
}

template<typename Derived>
bool EqualityTranslator<Derived>::reflexive_equality_object_name(ygg::Index<pddl::Literal> literal, std::string& name) const
{
    const auto& literal_data = this->m_storage->repository[literal];
    if (!literal_data.positive)
        return false;
    const auto& atom = this->m_storage->repository[literal_data.atom];
    if (atom.terms.size() != 2 || std::string(this->m_storage->repository[atom.predicate].name) != "=")
        return false;
    auto left_name = std::string {};
    auto right_name = std::string {};
    std::visit([&](const auto& value)
    {
        using Value = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
            left_name = std::string(this->m_storage->repository[value].name);
    }, this->m_storage->repository[atom.terms[0]].value);
    std::visit([&](const auto& value)
    {
        using Value = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
            right_name = std::string(this->m_storage->repository[value].name);
    }, this->m_storage->repository[atom.terms[1]].value);
    if (left_name.empty() || left_name != right_name)
        return false;
    name = std::move(left_name);
    return true;
}

template<typename Derived>
void EqualityTranslator<Derived>::compact_reflexive_equalities(ygg::Data<pddl::Task>& task) const
{
    auto seen = std::unordered_set<std::string> {};
    auto compacted = ygg::IndexList<pddl::Literal> {};
    for (auto literal : task.initial_literals)
    {
        auto name = std::string {};
        if (this->self().reflexive_equality_object_name(literal, name))
        {
            if (!seen.insert(name).second)
                continue;
        }
        compacted.push_back(literal);
    }
    task.initial_literals = std::move(compacted);
}

template<typename Derived>
void EqualityTranslator<Derived>::initialize_equality(ygg::Data<pddl::Task>& task)
{
    if (!this->self().equality_required(task))
        return;
    const auto predicate = this->self().ensure_equality_predicate();
    const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
    auto seen = std::unordered_set<std::string> {};
    for (auto literal : task.initial_literals)
    {
        const auto& literal_data = this->m_storage->repository[literal];
        if (!literal_data.positive)
            continue;
        const auto& atom = this->m_storage->repository[literal_data.atom];
        if (atom.predicate != predicate || atom.terms.size() != 2)
            continue;
        auto left_name = std::string {};
        auto right_name = std::string {};
        std::visit([&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                left_name = std::string(this->m_storage->repository[value].name);
        }, this->m_storage->repository[atom.terms[0]].value);
        std::visit([&](const auto& value)
        {
            using Value = std::decay_t<decltype(value)>;
            if constexpr (std::is_same_v<Value, ygg::Index<pddl::Object>>)
                right_name = std::string(this->m_storage->repository[value].name);
        }, this->m_storage->repository[atom.terms[1]].value);
        if (!left_name.empty() && left_name == right_name)
            seen.insert(left_name);
    }
    auto add_object = [&](ygg::Index<pddl::Object> object)
    {
        if (seen.insert(std::string(this->m_storage->repository[object].name)).second)
            task.initial_literals.push_back(this->self().equality_literal(predicate, object));
    };
    for (auto object : domain.constants)
        add_object(object);
    for (auto object : task.objects)
        add_object(object);
    task.domain = this->m_storage->translated_domain;
    this->self().compact_reflexive_equalities(task);
}

} // namespace loki2::semantic::detail

#endif
