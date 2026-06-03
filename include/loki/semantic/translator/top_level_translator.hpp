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

    formalism::ActionView copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository);

    formalism::AxiomView copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository);

    formalism::MetricView copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository);

    formalism::InitialFunctionValueView copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository);
};

template<typename Derived>
formalism::ActionView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Action> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Action> out;

    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto renamed = as_index(this->self().rename_action_variables(source, repository));
        const auto previous = this->m_renaming_enabled;
        this->m_renaming_enabled = false;
        out = as_index(this->self().copy(renamed, this->m_storage->repository));
        this->m_renaming_enabled = previous;
        remember(this->m_storage->actions, source, out);
        return ygg::make_view(out, this->m_storage->repository);
    }

    if (find_mapped(this->m_storage->actions, source, out)) return ygg::make_view(out, this->m_storage->repository);
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto precondition = this->self().template copy_optional<formalism::Condition>(data.precondition, repository);
    if (this->m_phase == TranslationPhase::MoveExistentialQuantifiers)
        this->self().lift_top_level_exists(parameters, precondition);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(precondition, parameters);
    const auto out_parameters = this->m_phase == TranslationPhase::AddTypePredicates ? this->self().maybe_strip_parameters(parameters) : parameters;
    out = formalism::get_or_create<formalism::Action>(this->m_storage->repository, data.name, out_parameters, precondition, this->self().template copy_optional<formalism::Effect>(data.effect, repository)).get_index();
    this->self().leave_scope();
    remember(this->m_storage->actions, source, out);
    return ygg::make_view(out, this->m_storage->repository);
}

template<typename Derived>
formalism::AxiomView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Axiom> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Axiom> out;

    if (this->m_phase == TranslationPhase::RenameQuantifiedVariables && this->m_renaming_enabled)
    {
        const auto renamed = as_index(this->self().rename_axiom_variables(source, repository));
        const auto previous = this->m_renaming_enabled;
        this->m_renaming_enabled = false;
        out = as_index(this->self().copy(renamed, this->m_storage->repository));
        this->m_renaming_enabled = previous;
        remember(this->m_storage->axioms, source, out);
        return ygg::make_view(out, this->m_storage->repository);
    }

    if (find_mapped(this->m_storage->axioms, source, out)) return ygg::make_view(out, this->m_storage->repository);
    const auto& data = repository[source];
    this->m_num_quantifications.clear();
    this->self().increment_quantifications(data.parameters, repository);
    auto parameters = this->self().copy_parameters(data.parameters, repository);
    this->self().enter_scope(parameters);
    auto condition = as_index(this->self().copy(data.condition, repository));
    if (this->m_phase == TranslationPhase::MoveExistentialQuantifiers)
        this->self().lift_top_level_exists(parameters, condition);
    if (this->m_phase == TranslationPhase::AddTypePredicates)
        this->self().prepend_type_conditions(condition, parameters);
    const auto out_parameters = this->m_phase == TranslationPhase::AddTypePredicates ? this->self().maybe_strip_parameters(parameters) : parameters;
    out = formalism::get_or_create<formalism::Axiom>(this->m_storage->repository, out_parameters, as_index(this->self().copy(data.head, repository)), condition).get_index();
    this->self().leave_scope();
    remember(this->m_storage->axioms, source, out);
    return ygg::make_view(out, this->m_storage->repository);
}

template<typename Derived>
formalism::MetricView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::Metric> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::Metric> out;
    if (find_mapped(this->m_storage->metrics, source, out)) return ygg::make_view(out, this->m_storage->repository);
    const auto& data = repository[source];
    out = formalism::get_or_create<formalism::Metric>(this->m_storage->repository, data.minimize, as_index(this->self().copy(data.expression, repository))).get_index();
    remember(this->m_storage->metrics, source, out);
    return ygg::make_view(out, this->m_storage->repository);
}

template<typename Derived>
formalism::InitialFunctionValueView TopLevelTranslator<Derived>::copy(ygg::Index<formalism::InitialFunctionValue> source, const formalism::Repository& repository)
{
    ygg::Index<formalism::InitialFunctionValue> out;
    if (find_mapped(this->m_storage->initial_function_values, source, out)) return ygg::make_view(out, this->m_storage->repository);
    const auto& data = repository[source];
    out = formalism::get_or_create<formalism::InitialFunctionValue>(this->m_storage->repository, as_index(this->self().copy(data.function, repository)), as_index(this->self().copy(data.value, repository))).get_index();
    remember(this->m_storage->initial_function_values, source, out);
    return ygg::make_view(out, this->m_storage->repository);
}

} // namespace loki::semantic::detail

#endif
