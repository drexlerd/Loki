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

#ifndef LOKI_SEMANTIC_TRANSLATOR_COMPILE_TYPING_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COMPILE_TYPING_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class CompileTypingTranslator : public CopyTranslatorComponent<Derived, CompileTypingTranslator<Derived>>
{
public:
    explicit CompileTypingTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, CompileTypingTranslator<Derived>>(context) {}

    bool compiles_typing_now() const noexcept;
    std::vector<formalism::TypeView> collect_type_hierarchy(formalism::TypeView type);
    void copy_type_hierarchy(formalism::EntityListView<formalism::Type> types, ygg::IndexList<formalism::Type>& result);
    void copy_parameters_without_types(formalism::EntityListView<formalism::Parameter> parameters, ygg::IndexList<formalism::Parameter>& result);
    formalism::PredicateView type_predicate(formalism::TypeView type);
    formalism::LiteralView type_literal(formalism::TypeView type, ygg::Index<formalism::Term> term);
    formalism::ConditionView type_condition(formalism::TypeView type, formalism::VariableView variable);
    ygg::IndexList<formalism::Condition> type_conditions_for_parameters(formalism::EntityListView<formalism::Parameter> parameters);
    void prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition, formalism::EntityListView<formalism::Parameter> parameters);
    void prepend_type_conditions(ygg::Index<formalism::Condition>& condition, formalism::EntityListView<formalism::Parameter> parameters);
    void compile_typing_to_domain(ygg::Data<formalism::Domain>& data, formalism::DomainView domain);
    void add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals, formalism::ObjectView object);
    void initialize_type_literals(ygg::Data<formalism::Task>& data, formalism::TaskView task);
};

template<typename Derived>
bool CompileTypingTranslator<Derived>::compiles_typing_now() const noexcept
{
    return this->m_compile_typing && this->m_phase == TranslationPhase::CompileTyping;
}

template<typename Derived>
std::vector<formalism::TypeView> CompileTypingTranslator<Derived>::collect_type_hierarchy(formalism::TypeView type)
{
    auto result = std::vector<formalism::TypeView> {};
    auto seen = ygg::UnorderedSet<formalism::TypeView> {};
    auto visit = [&](auto&& self, formalism::TypeView current) -> void
    {
        if (!seen.insert(current).second)
            return;
        result.push_back(current);
        for (auto base : current.get_bases())
            self(self, base);
    };
    visit(visit, type);
    return result;
}

template<typename Derived>
void CompileTypingTranslator<Derived>::copy_type_hierarchy(formalism::EntityListView<formalism::Type> types, ygg::IndexList<formalism::Type>& result)
{
    auto seen = ygg::UnorderedSet<formalism::TypeView> {};
    for (auto type : types)
    {
        for (auto collected : this->self().collect_type_hierarchy(type))
        {
            if (seen.insert(collected).second)
                result.push_back(as_index(this->self().copy(collected)));
        }
    }
}

template<typename Derived>
void CompileTypingTranslator<Derived>::copy_parameters_without_types(formalism::EntityListView<formalism::Parameter> parameters,
                                                                     ygg::IndexList<formalism::Parameter>& result)
{
    for (auto parameter : parameters)
    {
        const auto variable = as_index(this->self().copy(parameter.get_variable()));
        auto data = this->template checkout<formalism::Parameter>();
        data->variable = variable;
        result.push_back(formalism::get_or_create(this->m_storage->repository, *data).get_index());
    }
}

template<typename Derived>
formalism::PredicateView CompileTypingTranslator<Derived>::type_predicate(formalism::TypeView type)
{
    if (auto it = this->m_type_predicates.find(type); it != this->m_type_predicates.end())
        return it->second;

    auto parameter_data = this->template checkout<formalism::Parameter>();
    if (!this->self().compiles_typing_now())
        parameter_data->types.push_back(as_index(this->self().copy(type)));
    auto variable_data = this->template checkout<formalism::Variable>();
    variable_data->name = cista::offset::string("?arg");
    parameter_data->variable = formalism::get_or_create(this->m_storage->repository, *variable_data).get_index();
    auto predicate_data = this->template checkout<formalism::Predicate>();
    predicate_data->name = type.get_name();
    predicate_data->parameters.push_back(formalism::get_or_create(this->m_storage->repository, *parameter_data).get_index());
    auto predicate = formalism::get_or_create(this->m_storage->repository, *predicate_data);
    this->m_type_predicates.emplace(type, predicate);
    this->m_used_predicate_names.insert(std::string(type.get_name()));
    return predicate;
}

template<typename Derived>
formalism::LiteralView CompileTypingTranslator<Derived>::type_literal(formalism::TypeView type, ygg::Index<formalism::Term> term)
{
    const auto predicate = as_index(this->self().type_predicate(type));
    auto atom_data = this->template checkout<formalism::Atom>();
    atom_data->predicate = predicate;
    atom_data->terms.push_back(term);
    const auto atom = formalism::get_or_create(this->m_storage->repository, *atom_data).get_index();
    auto literal_data = this->template checkout<formalism::Literal>();
    literal_data->atom = atom;
    literal_data->m_polarity = true;
    return formalism::get_or_create(this->m_storage->repository, *literal_data);
}

template<typename Derived>
formalism::ConditionView CompileTypingTranslator<Derived>::type_condition(formalism::TypeView type, formalism::VariableView variable)
{
    const auto copied_variable = as_index(this->self().copy(variable));
    auto term_data = this->template checkout<formalism::Term>();
    term_data->value = ygg::Data<formalism::Term>::Variant(copied_variable);
    const auto term = formalism::get_or_create(this->m_storage->repository, *term_data).get_index();
    const auto literal = as_index(this->self().type_literal(type, term));
    auto condition_data = this->template checkout<formalism::ConditionLiteral>();
    condition_data->literal = literal;
    return this->self().wrap_condition(formalism::get_or_create(this->m_storage->repository, *condition_data));
}

template<typename Derived>
ygg::IndexList<formalism::Condition>
CompileTypingTranslator<Derived>::type_conditions_for_parameters(formalism::EntityListView<formalism::Parameter> parameters)
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
void CompileTypingTranslator<Derived>::prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition,
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
void CompileTypingTranslator<Derived>::prepend_type_conditions(ygg::Index<formalism::Condition>& condition,
                                                               formalism::EntityListView<formalism::Parameter> parameters)
{
    auto optional = cista::optional<ygg::Index<formalism::Condition>>(condition);
    this->self().prepend_type_conditions(optional, parameters);
    condition = *optional;
}

template<typename Derived>
void CompileTypingTranslator<Derived>::compile_typing_to_domain(ygg::Data<formalism::Domain>& data, formalism::DomainView domain)
{
    auto existing = ygg::UnorderedSet<formalism::PredicateView> {};
    for (auto predicate : domain.get_predicates())
        existing.insert(predicate);

    auto add_type = [&](auto&& self, formalism::TypeView type) -> void
    {
        const auto predicate = this->self().type_predicate(type);
        if (existing.insert(predicate).second)
            data.predicates.push_back(predicate.get_index());
        for (auto base : type.get_bases())
            self(self, base);
    };

    for (auto type : domain.get_types())
        add_type(add_type, type);
    for (auto object : domain.get_constants())
        for (auto type : object.get_types())
            add_type(add_type, type);

    this->self().strip_typing_requirement(domain.get_requirements(), data.requirements);
    if (this->self().compiles_typing_now())
        data.types.clear();
}

template<typename Derived>
void CompileTypingTranslator<Derived>::add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals, formalism::ObjectView object)
{
    const auto copied_object = this->self().copy(object);
    auto term_data = this->template checkout<formalism::Term>();
    term_data->value = ygg::Data<formalism::Term>::Variant(copied_object.get_index());
    const auto term = formalism::get_or_create(this->m_storage->repository, *term_data).get_index();
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
        if (auto it = this->m_storage->object_type_views.find(copied_object); it != this->m_storage->object_type_views.end())
        {
            for (auto type : it->second)
                add_literal(add_literal, type);
        }
    }
}

template<typename Derived>
void CompileTypingTranslator<Derived>::initialize_type_literals(ygg::Data<formalism::Task>& data, formalism::TaskView task)
{
    for (auto object : this->m_storage->translated_domain->get_constants())
        this->self().add_type_literals_for_object(data.initial_literals, object);
    for (auto object : task.get_objects())
        this->self().add_type_literals_for_object(data.initial_literals, object);
    this->self().strip_typing_requirement(task.get_requirements(), data.requirements);
}

}  // namespace loki::semantic::detail

#endif
