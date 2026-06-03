/*
 * Copyright (C) 2026 Dominik Drexler
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
    ygg::IndexList<formalism::Type> collect_type_hierarchy(ygg::Index<formalism::Type> type) const;
    ygg::IndexList<formalism::Type> collect_type_hierarchy(const ygg::IndexList<formalism::Type>& types) const;
    ygg::IndexList<formalism::Type> maybe_strip_types(const ygg::IndexList<formalism::Type>& types) const;
    formalism::ParameterView maybe_strip_parameter(ygg::Index<formalism::Parameter> parameter);
    ygg::IndexList<formalism::Parameter> maybe_strip_parameters(const ygg::IndexList<formalism::Parameter>& parameters);
    formalism::ObjectView maybe_strip_object(ygg::Index<formalism::Object> object);
    formalism::PredicateView type_predicate(ygg::Index<formalism::Type> type);
    formalism::LiteralView type_literal(ygg::Index<formalism::Type> type,ygg::Index<formalism::Term> term);
    formalism::ConditionView type_condition(ygg::Index<formalism::Type> type,ygg::Index<formalism::Variable> variable);
    ygg::IndexList<formalism::Condition> type_conditions_for_parameters(const ygg::IndexList<formalism::Parameter>& parameters);
    void prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition, const ygg::IndexList<formalism::Parameter>& parameters);
    void prepend_type_conditions(ygg::Index<formalism::Condition>& condition, const ygg::IndexList<formalism::Parameter>& parameters);
    void add_type_predicates_to_domain(ygg::Data<formalism::Domain>& data);
    void add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals,ygg::Index<formalism::Object> object);
    void initialize_type_literals(ygg::Data<formalism::Task>& task);
};

template<typename Derived>
bool TypeTranslator<Derived>::removes_typing_now() const noexcept
{
    return this->m_remove_typing && this->m_phase == TranslationPhase::AddTypePredicates;
}

template<typename Derived>
ygg::IndexList<formalism::Type> TypeTranslator<Derived>::collect_type_hierarchy(ygg::Index<formalism::Type> type) const
{
    auto result = ygg::IndexList<formalism::Type> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    auto visit = [&](auto&& self,ygg::Index<formalism::Type> current) -> void
    {
        if (!seen.insert(current.get_value()).second)
            return;
        result.push_back(current);
        for (auto base : this->m_storage->repository[current].bases)
            self(self, base);
    };
    visit(visit, type);
    return result;
}

template<typename Derived>
ygg::IndexList<formalism::Type> TypeTranslator<Derived>::collect_type_hierarchy(const ygg::IndexList<formalism::Type>& types) const
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
formalism::ParameterView TypeTranslator<Derived>::maybe_strip_parameter(ygg::Index<formalism::Parameter> parameter)
{
    if (!this->self().removes_typing_now())
        return ygg::make_view(parameter, this->m_storage->repository);
    const auto& data = this->m_storage->repository[parameter];
    return formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, data.variable, ygg::IndexList<formalism::Type> {});
}

template<typename Derived>
ygg::IndexList<formalism::Parameter> TypeTranslator<Derived>::maybe_strip_parameters(const ygg::IndexList<formalism::Parameter>& parameters)
{
    auto result = ygg::IndexList<formalism::Parameter> {};
    for (auto parameter : parameters)
        result.push_back(as_index(this->self().maybe_strip_parameter(parameter)));
    return result;
}

template<typename Derived>
formalism::ObjectView TypeTranslator<Derived>::maybe_strip_object(ygg::Index<formalism::Object> object)
{
    if (!this->self().removes_typing_now())
        return ygg::make_view(object, this->m_storage->repository);
    const auto& data = this->m_storage->repository[object];
    return formalism::get_or_create<formalism::Object>(this->m_storage->repository, data.name, ygg::IndexList<formalism::Type> {});
}

template<typename Derived>
formalism::PredicateView TypeTranslator<Derived>::type_predicate(ygg::Index<formalism::Type> type)
{
    if (auto it = this->m_type_predicates.find(type.get_value()); it != this->m_type_predicates.end())
        return ygg::make_view(it->second, this->m_storage->repository);

    auto parameters = ygg::IndexList<formalism::Parameter> {};
    const auto variable = formalism::get_or_create<formalism::Variable>(this->m_storage->repository, cista::offset::string("arg")).get_index();
    auto parameter_types = ygg::IndexList<formalism::Type> {};
    if (!this->self().removes_typing_now())
        parameter_types.push_back(type);
    parameters.push_back(formalism::get_or_create<formalism::Parameter>(this->m_storage->repository, variable, std::move(parameter_types)).get_index());

    const auto& type_data = this->m_storage->repository[type];
    const auto predicate = formalism::get_or_create<formalism::Predicate>(this->m_storage->repository, type_data.name, std::move(parameters)).get_index();
    this->m_type_predicates.emplace(type.get_value(), predicate);
    return ygg::make_view(predicate, this->m_storage->repository);
}

template<typename Derived>
formalism::LiteralView TypeTranslator<Derived>::type_literal(ygg::Index<formalism::Type> type,ygg::Index<formalism::Term> term)
{
    auto terms = ygg::IndexList<formalism::Term> {};
    terms.push_back(term);
    const auto atom = formalism::get_or_create<formalism::Atom>(this->m_storage->repository, as_index(this->self().type_predicate(type)), std::move(terms)).get_index();
    return formalism::get_or_create<formalism::Literal>(this->m_storage->repository, true, atom);
}

template<typename Derived>
formalism::ConditionView TypeTranslator<Derived>::type_condition(ygg::Index<formalism::Type> type,ygg::Index<formalism::Variable> variable)
{
    const auto term = formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(variable)).get_index();
    return this->self().wrap_condition(formalism::get_or_create<formalism::ConditionLiteral>(this->m_storage->repository, as_index(this->self().type_literal(type, term))).get_index());
}

template<typename Derived>
ygg::IndexList<formalism::Condition> TypeTranslator<Derived>::type_conditions_for_parameters(const ygg::IndexList<formalism::Parameter>& parameters)
{
    auto result = ygg::IndexList<formalism::Condition> {};
    for (auto parameter : parameters)
    {
        const auto& data = this->m_storage->repository[parameter];
        for (auto type : this->self().collect_type_hierarchy(data.types))
            result.push_back(as_index(this->self().type_condition(type, data.variable)));
    }
    return result;
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(cista::optional<ygg::Index<formalism::Condition>>& condition, const ygg::IndexList<formalism::Parameter>& parameters)
{
    auto parts = this->self().type_conditions_for_parameters(parameters);
    if (parts.empty())
        return;
    if (condition)
        parts.push_back(*condition);
    condition = as_index(this->self().flatten_condition(as_index(this->self().make_conjunction(std::move(parts)))));
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(ygg::Index<formalism::Condition>& condition, const ygg::IndexList<formalism::Parameter>& parameters)
{
    auto optional = cista::optional<ygg::Index<formalism::Condition>>(condition);
    this->self().prepend_type_conditions(optional, parameters);
    condition = *optional;
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_predicates_to_domain(ygg::Data<formalism::Domain>& data)
{
    auto existing = std::unordered_set<ygg::uint_t> {};
    for (auto predicate : data.predicates)
        existing.insert(predicate.get_value());

    auto types = data.types;
    for (auto object : data.constants)
    {
        const auto& object_data = this->m_storage->repository[object];
        if (!object_data.types.empty())
        {
            types.insert(types.end(), object_data.types.begin(), object_data.types.end());
        }
        else if (auto it = this->m_storage->object_types.find(object.get_value()); it != this->m_storage->object_types.end())
        {
            types.insert(types.end(), it->second.begin(), it->second.end());
        }
    }
    for (const auto& [_, object_types] : this->m_storage->object_types)
        types.insert(types.end(), object_types.begin(), object_types.end());

    for (auto type : this->self().collect_type_hierarchy(types))
    {
        const auto predicate = as_index(this->self().type_predicate(type));
        if (existing.insert(predicate.get_value()).second)
            data.predicates.push_back(predicate);
    }
    data.requirements = this->self().strip_typing_requirement(data.requirements);
    if (this->self().removes_typing_now())
        data.types = {};
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_literals_for_object(ygg::IndexList<formalism::Literal>& literals,ygg::Index<formalism::Object> object)
{
    const auto& data = this->m_storage->repository[object];
    const auto term = formalism::get_or_create<formalism::Term>(this->m_storage->repository, ygg::Data<formalism::Term>::Variant(object)).get_index();
    auto types = data.types;
    if (types.empty())
    {
        if (auto it = this->m_storage->object_types.find(object.get_value()); it != this->m_storage->object_types.end())
            types = it->second;
    }
    for (auto type : this->self().collect_type_hierarchy(types))
        literals.push_back(as_index(this->self().type_literal(type, term)));
}

template<typename Derived>
void TypeTranslator<Derived>::initialize_type_literals(ygg::Data<formalism::Task>& task)
{
    const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
    for (auto object : domain.constants)
        this->self().add_type_literals_for_object(task.initial_literals, object);
    for (auto object : task.objects)
        this->self().add_type_literals_for_object(task.initial_literals, object);
    task.requirements = this->self().strip_typing_requirement(task.requirements);
}

} // namespace loki::semantic::detail

#endif
