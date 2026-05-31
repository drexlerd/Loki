/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI2_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_
#define LOKI2_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_

#include "loki2/semantic/translator/copy_translator_component.hpp"

namespace loki2::semantic::detail
{

template<typename Derived>
class TopLevelTranslator : public CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>
{
public:
    explicit TopLevelTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>(context) {}

    ygg::Index<pddl::Action> copy(ygg::Index<pddl::Action> source, const pddl::Repository& repository);

    ygg::Index<pddl::Axiom> copy(ygg::Index<pddl::Axiom> source, const pddl::Repository& repository);

    ygg::Index<pddl::Metric> copy(ygg::Index<pddl::Metric> source, const pddl::Repository& repository);

    ygg::Index<pddl::InitialFunctionValue> copy(ygg::Index<pddl::InitialFunctionValue> source, const pddl::Repository& repository);
};

template<typename Derived>
ygg::Index<pddl::Action> TopLevelTranslator<Derived>::copy(ygg::Index<pddl::Action> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Action> out;
    if (find_mapped(this->m_storage->actions, source, out)) return out;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto precondition = this->self().template copy_optional<pddl::Condition>(data.precondition, repository);
    this->self().lift_top_level_exists(parameters, precondition);
    this->self().prepend_type_conditions(precondition, parameters);
    out = this->m_storage->builder.action(this->m_storage->repository, data.name, this->self().maybe_strip_parameters(parameters), precondition, this->self().template copy_optional<pddl::Effect>(data.effect, repository)).get_index();
    this->self().leave_scope();
    remember(this->m_storage->actions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Axiom> TopLevelTranslator<Derived>::copy(ygg::Index<pddl::Axiom> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Axiom> out;
    if (find_mapped(this->m_storage->axioms, source, out)) return out;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = this->self().copy(data.condition, repository);
    this->self().lift_top_level_exists(parameters, condition);
    this->self().prepend_type_conditions(condition, parameters);
    out = this->m_storage->builder.axiom(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), this->self().copy(data.head, repository), condition).get_index();
    this->self().leave_scope();
    remember(this->m_storage->axioms, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::Metric> TopLevelTranslator<Derived>::copy(ygg::Index<pddl::Metric> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::Metric> out;
    if (find_mapped(this->m_storage->metrics, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.metric(this->m_storage->repository, data.minimize, this->self().copy(data.expression, repository)).get_index();
    remember(this->m_storage->metrics, source, out);
    return out;
}

template<typename Derived>
ygg::Index<pddl::InitialFunctionValue> TopLevelTranslator<Derived>::copy(ygg::Index<pddl::InitialFunctionValue> source, const pddl::Repository& repository)
{
    ygg::Index<pddl::InitialFunctionValue> out;
    if (find_mapped(this->m_storage->initial_function_values, source, out)) return out;
    const auto& data = repository[source];
    out = this->m_storage->builder.initial_function_value(this->m_storage->repository, this->self().copy(data.function, repository), this->self().copy(data.value, repository)).get_index();
    remember(this->m_storage->initial_function_values, source, out);
    return out;
}

} // namespace loki2::semantic::detail

#endif
