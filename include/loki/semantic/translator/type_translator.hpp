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

#ifndef LOKI_SEMANTIC_TRANSLATOR_TYPE_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_TYPE_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class TypeTranslator : public CopyTranslatorComponent<Derived, TypeTranslator<Derived>>
{
public:
    explicit TypeTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, TypeTranslator<Derived>>(context) {}

    bool removes_typing_now() const noexcept;
    ygg::IndexList<formalism::Type> collect_type_hierarchy(formalism::TypeView type);
    ygg::IndexList<formalism::Type> collect_type_hierarchy(formalism::EntityListView<formalism::Type> types);
    ygg::IndexList<formalism::Type> maybe_strip_types(const ygg::IndexList<formalism::Type>& types) const;
    ygg::IndexList<formalism::Parameter> copy_parameters_without_types(formalism::EntityListView<formalism::Parameter> parameters);
    formalism::PredicateView type_predicate(formalism::TypeView type);
    formalism::LiteralView type_literal(formalism::TypeView type, ygg::Index<formalism::Term> term);
    formalism::ConditionView type_condition(formalism::TypeView type, formalism::VariableView variable);
    ygg::IndexList<formalism::Condition> type_conditions_for_parameters(formalism::EntityListView<formalism::Parameter> parameters);
    void prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition, formalism::EntityListView<formalism::Parameter> parameters);
    void prepend_type_conditions(ygg::Index<formalism::Condition>& condition, formalism::EntityListView<formalism::Parameter> parameters);
    void add_type_predicates_to_domain(ygg::Data<formalism::Domain>& data, formalism::DomainView domain);
    void add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals, formalism::ObjectView object);
    void initialize_type_literals(ygg::Data<formalism::Task>& data, formalism::TaskView task);
};

template<typename Derived>
bool TypeTranslator<Derived>::removes_typing_now() const noexcept
{
    return this->m_remove_typing && this->m_phase == TranslationPhase::AddTypePredicates;
}

template<typename Derived>
ygg::IndexList<formalism::Type> TypeTranslator<Derived>::collect_type_hierarchy(formalism::TypeView type)
{
    auto result = ygg::IndexList<formalism::Type> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    auto visit = [&](auto&& self, formalism::TypeView current) -> void
    {
        if (!seen.insert(current.get_index().get_value()).second)
            return;
        result.push_back(as_index(this->self().copy(current, current.get_context())));
        for (auto base : current.get_bases())
            self(self, base);
    };
    visit(visit, type);
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Type> TypeTranslator<Derived>::collect_type_hierarchy(formalism::EntityListView<formalism::Type> types)
{
    auto result = ygg::IndexList<formalism::Type> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    for (auto type : types)
    {
        for (auto collected : this->self().collect_type_hierarchy(type))
        {
            if (seen.insert(collected.get_value()).second)
                result.push_back(collected);
        }
    }
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Type> TypeTranslator<Derived>::maybe_strip_types(const ygg::IndexList<formalism::Type>& types) const
{
    return this->self().removes_typing_now() ? ygg::IndexList<formalism::Type> {} : types;
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> TypeTranslator<Derived>::copy_parameters_without_types(formalism::EntityListView<formalism::Parameter> parameters)
{
    auto result = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : parameters)
    {
        result.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository,
                                                                        as_index(this->self().copy(parameter.get_variable(), parameters.get_context())),
                                                                        ygg::IndexList<formalism::Type> {})
                             .get_index());
    }
    return result;
}

template<typename Derived>
formalism::PredicateView TypeTranslator<Derived>::type_predicate(formalism::TypeView type)
{
    if (auto it = this->m_type_predicates.find(type.get_index().get_value()); it != this->m_type_predicates.end())
        return it->second;

    auto parameter_types = ygg::IndexList<formalism::Type> {};
    if (!this->self().removes_typing_now())
        parameter_types.push_back(as_index(this->self().copy(type, type.get_context())));
    const auto variable = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("arg")).get_index();
    auto parameters = ygg::IndexList<formalism::Parameter> {};
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, variable, std::move(parameter_types)).get_index());

    auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, type.get_name(), std::move(parameters));
    this->m_type_predicates.emplace(type.get_index().get_value(), predicate);
    this->m_used_predicate_names.insert(std::string(type.get_name()));
    return predicate;
}

template<typename Derived>
formalism::LiteralView TypeTranslator<Derived>::type_literal(formalism::TypeView type, ygg::Index<formalism::Term> term)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    terms.push_back(term);
    const auto atom =
        formalism::get_or_create<formalism::Atom>(this->m_storage->repository, as_index(this->self().type_predicate(type)), std::move(terms)).get_index();
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository, atom, true);
}

template<typename Derived>
formalism::ConditionView TypeTranslator<Derived>::type_condition(formalism::TypeView type, formalism::VariableView variable)
{
    const auto term =
        formalism::get_or_create<formalism::Term>(this->m_storage->repository,
                                                  ygg::Data<formalism::Term>::Variant(as_index(this->self().copy(variable, variable.get_context()))))
            .get_index();
    return this->self().wrap_condition(
        formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, as_index(this->self().type_literal(type, term))));
}

template<typename Derived>
ygg::IndexList<formalism::Condition> TypeTranslator<Derived>::type_conditions_for_parameters(formalism::EntityListView<formalism::Parameter> parameters)
{
    auto result = ygg::IndexList<formalism::Condition> {};
    auto add_conditions = [&](auto&& self, formalism::TypeView type, formalism::VariableView variable) -> void
    {
        result.push_back(as_index(this->self().type_condition(type, variable)));
        for (auto base : type.get_bases())
            self(self, base, variable);
    };
    for (auto parameter : parameters)
        for (auto type : parameter.get_types())
            add_conditions(add_conditions, type, parameter.get_variable());
    return result;
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition,
                                                      formalism::EntityListView<formalism::Parameter> parameters)
{
    auto parts = this->self().type_conditions_for_parameters(parameters);
    if (parts.empty())
        return;
    if (condition)
        parts.push_back(*condition);
    condition = as_index(this->self().make_conjunction(std::move(parts)));
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(ygg::Index<formalism::Condition>& condition, formalism::EntityListView<formalism::Parameter> parameters)
{
    auto optional = cista::optional<ygg::Index<formalism::Condition>>(condition);
    this->self().prepend_type_conditions(optional, parameters);
    condition = *optional;
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_predicates_to_domain(ygg::Data<formalism::Domain>& data, formalism::DomainView domain)
{
    auto existing = std::unordered_set<ygg::uint_t> {};
    for (auto predicate : data.predicates)
        existing.insert(predicate.get_value());

    auto add_type = [&](auto&& self, formalism::TypeView type) -> void
    {
        const auto predicate = as_index(this->self().type_predicate(type));
        if (existing.insert(predicate.get_value()).second)
            data.predicates.push_back(predicate);
        for (auto base : type.get_bases())
            self(self, base);
    };

    for (auto type : domain.get_types())
        add_type(add_type, type);
    for (auto object : domain.get_constants())
        for (auto type : object.get_types())
            add_type(add_type, type);

    data.requirements = this->self().strip_typing_requirement(domain.get_requirements());
    if (this->self().removes_typing_now())
        data.types = {};
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals, formalism::ObjectView object)
{
    const auto copied_object = as_index(this->self().copy(object, object.get_context()));
    const auto term = formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(copied_object)).get_index();
    auto add_literal = [&](auto&& self, formalism::TypeView type) -> void
    {
        literals.push_back(as_index(this->self().type_literal(type, term)));
        for (auto base : type.get_bases())
            self(self, base);
    };
    auto has_source_types = false;
    for (auto type : object.get_types())
    {
        has_source_types = true;
        add_literal(add_literal, type);
    }
    if (!has_source_types)
    {
        if (auto it = this->m_storage->object_type_views.find(copied_object.get_value()); it != this->m_storage->object_type_views.end())
        {
            for (auto type : it->second)
                add_literal(add_literal, type);
        }
    }
}

template<typename Derived>
void TypeTranslator<Derived>::initialize_type_literals(ygg::Data<formalism::Task>& data, formalism::TaskView task)
{
    for (auto object : this->m_storage->translated_domain->get_constants())
        this->self().add_type_literals_for_object(data.initial_literals, object);
    for (auto object : task.get_objects())
        this->self().add_type_literals_for_object(data.initial_literals, object);
    data.requirements = this->self().strip_typing_requirement(task.get_requirements());
}

}  // namespace loki::semantic::detail

#endif
