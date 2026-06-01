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

    ygg::IndexList<pddl::Type> collect_type_hierarchy(ygg::Index<pddl::Type> type) const;
    ygg::IndexList<pddl::Type> collect_type_hierarchy(const ygg::IndexList<pddl::Type>& types) const;
    ygg::IndexList<pddl::Type> maybe_strip_types(const ygg::IndexList<pddl::Type>& types) const;
    ygg::Index<pddl::Parameter> maybe_strip_parameter(ygg::Index<pddl::Parameter> parameter);
    ygg::IndexList<pddl::Parameter> maybe_strip_parameters(const ygg::IndexList<pddl::Parameter>& parameters);
    ygg::Index<pddl::Object> maybe_strip_object(ygg::Index<pddl::Object> object);
    ygg::Index<pddl::Predicate> type_predicate(ygg::Index<pddl::Type> type);
    ygg::Index<pddl::Literal> type_literal(ygg::Index<pddl::Type> type, ygg::Index<pddl::Term> term);
    ygg::Index<pddl::Condition> type_condition(ygg::Index<pddl::Type> type, ygg::Index<pddl::Variable> variable);
    ygg::IndexList<pddl::Condition> type_conditions_for_parameters(const ygg::IndexList<pddl::Parameter>& parameters);
    void prepend_type_conditions(cista::optional<ygg::Index<pddl::Condition>>& condition, const ygg::IndexList<pddl::Parameter>& parameters);
    void prepend_type_conditions(ygg::Index<pddl::Condition>& condition, const ygg::IndexList<pddl::Parameter>& parameters);
    void add_type_predicates_to_domain(ygg::Data<pddl::Domain>& data);
    void add_type_literals_for_object(ygg::IndexList<pddl::Literal>& literals, ygg::Index<pddl::Object> object);
    void initialize_type_literals(ygg::Data<pddl::Task>& task);
};

template<typename Derived>
ygg::IndexList<pddl::Type> TypeTranslator<Derived>::collect_type_hierarchy(ygg::Index<pddl::Type> type) const
{
    auto result = ygg::IndexList<pddl::Type> {};
    auto seen = std::unordered_set<ygg::uint_t> {};
    auto visit = [&](auto&& self, ygg::Index<pddl::Type> current) -> void
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
ygg::IndexList<pddl::Type> TypeTranslator<Derived>::collect_type_hierarchy(const ygg::IndexList<pddl::Type>& types) const
{
    auto result = ygg::IndexList<pddl::Type> {};
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
ygg::IndexList<pddl::Type> TypeTranslator<Derived>::maybe_strip_types(const ygg::IndexList<pddl::Type>& types) const
{
    return this->m_remove_typing ? ygg::IndexList<pddl::Type> {} : types;
}

template<typename Derived>
ygg::Index<pddl::Parameter> TypeTranslator<Derived>::maybe_strip_parameter(ygg::Index<pddl::Parameter> parameter)
{
    if (!this->m_remove_typing)
        return parameter;
    const auto& data = this->m_storage->repository[parameter];
    return pddl::get_or_create<pddl::Parameter>(this->m_storage->repository, data.variable, ygg::IndexList<pddl::Type> {}).get_index();
}

template<typename Derived>
ygg::IndexList<pddl::Parameter> TypeTranslator<Derived>::maybe_strip_parameters(const ygg::IndexList<pddl::Parameter>& parameters)
{
    auto result = ygg::IndexList<pddl::Parameter> {};
    for (auto parameter : parameters)
        result.push_back(this->self().maybe_strip_parameter(parameter));
    return result;
}

template<typename Derived>
ygg::Index<pddl::Object> TypeTranslator<Derived>::maybe_strip_object(ygg::Index<pddl::Object> object)
{
    if (!this->m_remove_typing)
        return object;
    const auto& data = this->m_storage->repository[object];
    return pddl::get_or_create<pddl::Object>(this->m_storage->repository, data.name, ygg::IndexList<pddl::Type> {}).get_index();
}

template<typename Derived>
ygg::Index<pddl::Predicate> TypeTranslator<Derived>::type_predicate(ygg::Index<pddl::Type> type)
{
    if (auto it = this->m_type_predicates.find(type.get_value()); it != this->m_type_predicates.end())
        return it->second;
    const auto name = std::string(this->m_storage->repository[type].name);
    if (this->m_storage->translated_domain != ygg::Index<pddl::Domain>::max())
    {
        const auto& domain = this->m_storage->repository[this->m_storage->translated_domain];
        for (auto predicate : domain.predicates)
        {
            if (std::string(this->m_storage->repository[predicate].name) == name)
            {
                this->m_type_predicates.emplace(type.get_value(), predicate);
                return predicate;
            }
        }
    }
    auto parameters = ygg::IndexList<pddl::Parameter> {};
    const auto variable = pddl::get_or_create<pddl::Variable>(this->m_storage->repository, cista::offset::string("arg")).get_index();
    auto parameter_types = ygg::IndexList<pddl::Type> {};
    if (!this->m_remove_typing)
        parameter_types.push_back(type);
    parameters.push_back(pddl::get_or_create<pddl::Parameter>(this->m_storage->repository, variable, std::move(parameter_types)).get_index());
    const auto predicate = pddl::get_or_create<pddl::Predicate>(this->m_storage->repository, cista::offset::string(name), std::move(parameters)).get_index();
    this->m_type_predicates.emplace(type.get_value(), predicate);
    return predicate;
}

template<typename Derived>
ygg::Index<pddl::Literal> TypeTranslator<Derived>::type_literal(ygg::Index<pddl::Type> type, ygg::Index<pddl::Term> term)
{
    auto terms = ygg::IndexList<pddl::Term> {};
    terms.push_back(term);
    const auto atom = pddl::get_or_create<pddl::Atom>(this->m_storage->repository, this->self().type_predicate(type), std::move(terms)).get_index();
    return pddl::get_or_create<pddl::Literal>(this->m_storage->repository, true, atom).get_index();
}

template<typename Derived>
ygg::Index<pddl::Condition> TypeTranslator<Derived>::type_condition(ygg::Index<pddl::Type> type, ygg::Index<pddl::Variable> variable)
{
    const auto term = pddl::get_or_create<pddl::Term>(this->m_storage->repository, ygg::Data<pddl::Term>::Variant(variable)).get_index();
    return this->self().wrap_condition(pddl::get_or_create<pddl::ConditionLiteral>(this->m_storage->repository, this->self().type_literal(type, term)).get_index());
}

template<typename Derived>
ygg::IndexList<pddl::Condition> TypeTranslator<Derived>::type_conditions_for_parameters(const ygg::IndexList<pddl::Parameter>& parameters)
{
    auto result = ygg::IndexList<pddl::Condition> {};
    for (auto parameter : parameters)
    {
        const auto& data = this->m_storage->repository[parameter];
        for (auto type : this->self().collect_type_hierarchy(data.types))
            result.push_back(this->self().type_condition(type, data.variable));
    }
    return result;
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(cista::optional<ygg::Index<pddl::Condition>>& condition, const ygg::IndexList<pddl::Parameter>& parameters)
{
    auto parts = this->self().type_conditions_for_parameters(parameters);
    if (parts.empty())
        return;
    if (condition)
        parts.push_back(*condition);
    condition = this->self().make_conjunction(std::move(parts));
}

template<typename Derived>
void TypeTranslator<Derived>::prepend_type_conditions(ygg::Index<pddl::Condition>& condition, const ygg::IndexList<pddl::Parameter>& parameters)
{
    auto optional = cista::optional<ygg::Index<pddl::Condition>>(condition);
    this->self().prepend_type_conditions(optional, parameters);
    condition = *optional;
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_predicates_to_domain(ygg::Data<pddl::Domain>& data)
{
    auto existing = std::unordered_set<std::string> {};
    for (auto predicate : data.predicates)
        existing.insert(std::string(this->m_storage->repository[predicate].name));
    for (auto type : this->self().collect_type_hierarchy(data.types))
    {
        const auto name = std::string(this->m_storage->repository[type].name);
        if (existing.insert(name).second)
            data.predicates.push_back(this->self().type_predicate(type));
    }
    data.requirements = this->self().strip_typing_requirement(data.requirements);
    if (this->m_remove_typing)
        data.types = {};
}

template<typename Derived>
void TypeTranslator<Derived>::add_type_literals_for_object(ygg::IndexList<pddl::Literal>& literals, ygg::Index<pddl::Object> object)
{
    const auto& data = this->m_storage->repository[object];
    const auto term = pddl::get_or_create<pddl::Term>(this->m_storage->repository, ygg::Data<pddl::Term>::Variant(object)).get_index();
    auto types = data.types;
    if (types.empty())
    {
        if (auto it = this->m_storage->object_types_by_name.find(std::string(data.name)); it != this->m_storage->object_types_by_name.end())
            types = it->second;
    }
    for (auto type : this->self().collect_type_hierarchy(types))
        literals.push_back(this->self().type_literal(type, term));
}

template<typename Derived>
void TypeTranslator<Derived>::initialize_type_literals(ygg::Data<pddl::Task>& task)
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
