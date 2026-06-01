/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_CONDITION_ANALYSIS_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_CONDITION_ANALYSIS_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class ConditionAnalysisTranslator : public CopyTranslatorComponent<Derived, ConditionAnalysisTranslator<Derived>>
{
public:
    explicit ConditionAnalysisTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, ConditionAnalysisTranslator<Derived>>(context) {}

    ygg::Index<pddl::Term> term_from_variable(ygg::Index<pddl::Variable> variable);
    void collect_free_variables(ygg::Index<pddl::Term> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::Atom> atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::Literal> literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::Condition> condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionLiteral> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionNumericConstraint>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const;
    void collect_free_variables(ygg::Index<pddl::ConditionNot> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionImply> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionAnd> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionOr> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionExists> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    void collect_free_variables(ygg::Index<pddl::ConditionForall> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const;
    ygg::IndexList<pddl::Parameter> free_parameters_in_scope(ygg::Index<pddl::Condition> condition) const;
};

template<typename Derived>
ygg::Index<pddl::Term> ConditionAnalysisTranslator<Derived>::term_from_variable(ygg::Index<pddl::Variable> variable)
{
    return pddl::get_or_create<pddl::Term>(this->m_storage->repository, ygg::Data<pddl::Term>::Variant(variable)).get_index();
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::Term> term, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    std::visit([&](const auto& value)
    {
        using Value = std::decay_t<decltype(value)>;
        if constexpr (std::is_same_v<Value, ygg::Index<pddl::Variable>>)
        {
            if (!bound.contains(value.get_value()))
                free.insert(value.get_value());
        }
    }, this->m_storage->repository[term].value);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::Atom> atom, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto term : this->m_storage->repository[atom].terms)
        this->self().collect_free_variables(term, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::Literal> literal, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(this->m_storage->repository[literal].atom, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::Condition> condition, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    std::visit([&](const auto& node) { this->self().collect_free_variables(node, bound, free); }, this->m_storage->repository[condition].value);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionLiteral> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { this->self().collect_free_variables(this->m_storage->repository[node].literal, bound, free); }

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionNumericConstraint>, std::unordered_set<ygg::uint_t>&, std::unordered_set<ygg::uint_t>&) const {}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionNot> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const { this->self().collect_free_variables(this->m_storage->repository[node].condition, bound, free); }

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionImply> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    this->self().collect_free_variables(this->m_storage->repository[node].left, bound, free);
    this->self().collect_free_variables(this->m_storage->repository[node].right, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionAnd> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : this->m_storage->repository[node].conditions)
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionOr> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    for (auto condition : this->m_storage->repository[node].conditions)
        this->self().collect_free_variables(condition, bound, free);
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionExists> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[node];
    for (auto parameter : data.parameters)
        bound.insert(this->m_storage->repository[parameter].variable.get_value());
    this->self().collect_free_variables(data.condition, bound, free);
    for (auto parameter : data.parameters)
        bound.erase(this->m_storage->repository[parameter].variable.get_value());
}

template<typename Derived>
void ConditionAnalysisTranslator<Derived>::collect_free_variables(ygg::Index<pddl::ConditionForall> node, std::unordered_set<ygg::uint_t>& bound, std::unordered_set<ygg::uint_t>& free) const
{
    const auto& data = this->m_storage->repository[node];
    for (auto parameter : data.parameters)
        bound.insert(this->m_storage->repository[parameter].variable.get_value());
    this->self().collect_free_variables(data.condition, bound, free);
    for (auto parameter : data.parameters)
        bound.erase(this->m_storage->repository[parameter].variable.get_value());
}

template<typename Derived>
ygg::IndexList<pddl::Parameter> ConditionAnalysisTranslator<Derived>::free_parameters_in_scope(ygg::Index<pddl::Condition> condition) const
{
    auto bound = std::unordered_set<ygg::uint_t> {};
    auto free = std::unordered_set<ygg::uint_t> {};
    this->self().collect_free_variables(condition, bound, free);

    auto result = ygg::IndexList<pddl::Parameter> {};
    for (const auto& scope : this->m_active_parameters)
    {
        for (auto parameter : scope)
        {
            if (free.contains(this->m_storage->repository[parameter].variable.get_value()))
                result.push_back(parameter);
        }
    }
    return result;
}

} // namespace loki::semantic::detail

#endif
