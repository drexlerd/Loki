/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_EQUALITY_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_EQUALITY_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class EqualityTranslator : public CopyTranslatorComponent<Derived, EqualityTranslator<Derived>>
{
public:
    explicit EqualityTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, EqualityTranslator<Derived>>(context) {}

    bool has_requirement(const ygg::IndexList<formalism::Requirement>& requirements, formalism::RequirementKind kind) const;
    bool equality_required(const ygg::Data<formalism::Task>& task) const;
    formalism::PredicateView ensure_equality_predicate();
    formalism::LiteralView equality_literal(ygg::Index<formalism::Predicate> predicate,ygg::Index<formalism::Object> object);
    bool domain_uses_equality() const;
    void add_equality_predicate_to_domain(ygg::Data<formalism::Domain>& domain);
    void initialize_equality(ygg::Data<formalism::Task>& task);
};

template<typename Derived>
bool EqualityTranslator<Derived>::has_requirement(const ygg::IndexList<formalism::Requirement>& requirements, formalism::RequirementKind kind) const
{
    for (auto requirement : requirements)
        if (this->m_storage->repository[requirement].kind == kind)
            return true;
    return false;
}

template<typename Derived>
bool EqualityTranslator<Derived>::equality_required(const ygg::Data<formalism::Task>& task) const
{
    if (this->self().has_requirement(task.requirements, formalism::RequirementKind::Equality))
        return true;
    const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
    return this->self().has_requirement(domain.requirements, formalism::RequirementKind::Equality) || this->self().domain_uses_equality();
}

template<typename Derived>
formalism::PredicateView EqualityTranslator<Derived>::ensure_equality_predicate()
{
    if (this->m_equality_predicate)
        return ygg::make_view(*this->m_equality_predicate, this->m_storage->repository);
    auto domain = this->m_storage->repository[this->m_storage->translated_domain];
    for (auto predicate : domain.predicates)
    {
        if (std::string(this->m_storage->repository[predicate].name) == "=")
        {
            this->m_equality_predicate = predicate;
            return ygg::make_view(predicate, this->m_storage->repository);
        }
    }
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    auto types = ygg::IndexList<formalism::Type> {};
    for (auto type : domain.types)
    {
        if (std::string(this->m_storage->repository[type].name) == "object")
        {
            types.push_back(type);
            break;
        }
    }
    const auto left = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("lhs")).get_index();
    const auto right = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("rhs")).get_index();
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, left, types).get_index());
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, right, types).get_index());
    const auto predicate =
        formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, cista::offset::string("="), std::move(parameters)).get_index();
    domain.predicates.push_back(predicate);
    this->m_storage->translated_domain = formalism::get_or_create<formalism::Domain>(this->m_storage->repository, std::move(domain)).get_index();
    this->m_equality_predicate = predicate;
    return ygg::make_view(predicate, this->m_storage->repository);
}

template<typename Derived>
formalism::LiteralView EqualityTranslator<Derived>::equality_literal(ygg::Index<formalism::Predicate> predicate,ygg::Index<formalism::Object> object)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    const auto term = formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(object)).get_index();
    terms.push_back(term);
    terms.push_back(term);
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, predicate, std::move(terms)).get_index();
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository, true, atom);
}

template<typename Derived>
bool EqualityTranslator<Derived>::domain_uses_equality() const
{
    for (ygg::uint_t i = 0; i < this->m_storage->repository.template size<formalism::Predicate>(); ++i)
    {
        if (std::string(this->m_storage->repository[ygg::Index<formalism::Predicate>(i)].name) == "=")
            return true;
    }
    return false;
}

template<typename Derived>
void EqualityTranslator<Derived>::add_equality_predicate_to_domain(ygg::Data<formalism::Domain>& domain)
{
    if (!this->self().has_requirement(domain.requirements, formalism::RequirementKind::Equality) && !this->self().domain_uses_equality())
        return;

    for (auto predicate : domain.predicates)
    {
        if (std::string(this->m_storage->repository[predicate].name) == "=")
        {
            this->m_equality_predicate = predicate;
            return;
        }
    }

    for (ygg::uint_t i = 0; i < this->m_storage->repository.template size<formalism::Predicate>(); ++i)
    {
        const auto predicate = ygg::Index<formalism::Predicate>(i);
        if (std::string(this->m_storage->repository[predicate].name) == "=")
        {
            domain.predicates.push_back(predicate);
            this->m_equality_predicate = predicate;
            return;
        }
    }

    auto object_types = ygg::IndexList<formalism::Type> {};
    for (auto type : domain.types)
    {
        if (std::string(this->m_storage->repository[type].name) == "object")
        {
            object_types.push_back(type);
            break;
        }
    }

    auto parameters = ygg::IndexList<formalism::Parameter> {};
    const auto left = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("lhs")).get_index();
    const auto right = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("rhs")).get_index();
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, left, object_types).get_index());
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, right, std::move(object_types)).get_index());

    const auto predicate =
        formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, cista::offset::string("="), std::move(parameters)).get_index();
    domain.predicates.push_back(predicate);
    this->m_equality_predicate = predicate;
}

template<typename Derived>
void EqualityTranslator<Derived>::initialize_equality(ygg::Data<formalism::Task>& task)
{
    if (!this->self().equality_required(task))
        return;
    const auto predicate = as_index(this->self().ensure_equality_predicate());
    const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
    for (auto object : domain.constants)
        task.initial_literals.push_back(as_index(this->self().equality_literal(predicate, object)));
    for (auto object : task.objects)
        task.initial_literals.push_back(as_index(this->self().equality_literal(predicate, object)));
    task.domain = this->m_storage->translated_domain;
}

}  // namespace loki::semantic::detail

#endif
