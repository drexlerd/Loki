/*
 * Copyright (C) 2026 Dominik Drexler
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_TOP_LEVEL_TRANSLATOR_HPP_

#include "loki/semantic/translator/copy_translator_component.hpp"

namespace loki::semantic::detail
{

template<typename Derived>
class TopLevelTranslator : public CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>
{
public:
    explicit TopLevelTranslator(CopyContext& context) : CopyTranslatorComponent<Derived, TopLevelTranslator<Derived>>(context) {}

    ygg::Index<formalism::Action> copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository);

    ygg::Index<formalism::Axiom> copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository);

    ygg::Index<formalism::Metric> copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository);

    ygg::Index<formalism::InitialFunctionValue> copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository);
};

template<typename Derived>
ygg::Index<formalism::Action> TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Action> out;
    if (find_mapped(this->m_storage->actions, source, out)) return out;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto precondition = this->self().template copy_optional<formalism::Condition>(data.precondition, repository);
    this->self().lift_top_level_exists(parameters, precondition);
    this->self().prepend_type_conditions(precondition, parameters);
    out = formalism::get_or_create<formalism::Action>(this->m_storage->repository, data.name, this->self().maybe_strip_parameters(parameters), precondition, this->self().template copy_optional<formalism::Effect>(data.effect, repository)).get_index();
    this->self().leave_scope();
    remember(this->m_storage->actions, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Axiom> TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Axiom> out;
    if (find_mapped(this->m_storage->axioms, source, out)) return out;
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = this->self().copy(data.condition, repository);
    this->self().lift_top_level_exists(parameters, condition);
    this->self().prepend_type_conditions(condition, parameters);
    out = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, this->self().maybe_strip_parameters(parameters), this->self().copy(data.head, repository), condition).get_index();
    this->self().leave_scope();
    remember(this->m_storage->axioms, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::Metric> TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Metric> out;
    if (find_mapped(this->m_storage->metrics, source, out)) return out;
    const auto& data = repository[source];
    out = formalism::get_or_create<formalism::Metric>(this->m_storage->repository, data.minimize, this->self().copy(data.expression, repository)).get_index();
    remember(this->m_storage->metrics, source, out);
    return out;
}

template<typename Derived>
ygg::Index<formalism::InitialFunctionValue> TopLevelTranslator<Derived>::copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::InitialFunctionValue> out;
    if (find_mapped(this->m_storage->initial_function_values, source, out)) return out;
    const auto& data = repository[source];
    out = formalism::get_or_create<formalism::InitialFunctionValue>(this->m_storage->repository, this->self().copy(data.function, repository), this->self().copy(data.value, repository)).get_index();
    remember(this->m_storage->initial_function_values, source, out);
    return out;
}

} // namespace loki::semantic::detail

#endif
