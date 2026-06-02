/*
 * Copyright (C) 2026 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_HPP_
#define LOKI_SEMANTIC_TRANSLATOR_COPY_TRANSLATOR_HPP_

#include "loki/semantic/translator/basic_copy_translator.hpp"
#include "loki/semantic/translator/condition_analysis_translator.hpp"
#include "loki/semantic/translator/condition_builder_translator.hpp"
#include "loki/semantic/translator/condition_dnf_translator.hpp"
#include "loki/semantic/translator/condition_nnf_translator.hpp"
#include "loki/semantic/translator/condition_quantifier_translator.hpp"
#include "loki/semantic/translator/condition_translator.hpp"
#include "loki/semantic/translator/copy_translator_facade.hpp"
#include "loki/semantic/translator/effect_normal_form_translator.hpp"
#include "loki/semantic/translator/effect_translator.hpp"
#include "loki/semantic/translator/equality_translator.hpp"
#include "loki/semantic/translator/goal_simplification_translator.hpp"
#include "loki/semantic/translator/top_level_translator.hpp"
#include "loki/semantic/translator/type_translator.hpp"
#include "loki/semantic/translator/variable_renaming_translator.hpp"

namespace loki::semantic::detail
{

class CopyTranslator :
    private CopyContextOwner,
    public CopyTranslatorFacade<CopyTranslator>,
    public BasicCopyTranslator<CopyTranslator>,
    public ConditionTranslator<CopyTranslator>,
    public ConditionAnalysisTranslator<CopyTranslator>,
    public ConditionBuilderTranslator<CopyTranslator>,
    public ConditionDnfTranslator<CopyTranslator>,
    public ConditionNnfTranslator<CopyTranslator>,
    public ConditionQuantifierTranslator<CopyTranslator>,
    public EffectTranslator<CopyTranslator>,
    public EffectNormalFormTranslator<CopyTranslator>,
    public EqualityTranslator<CopyTranslator>,
    public GoalSimplificationTranslator<CopyTranslator>,
    public TopLevelTranslator<CopyTranslator>,
    public TypeTranslator<CopyTranslator>,
    public VariableRenamingTranslator<CopyTranslator>
{
public:
    explicit CopyTranslator(std::shared_ptr<TranslationStorage> storage, bool remove_typing = true) :
        CopyContextOwner(std::move(storage), remove_typing),
        CopyTranslatorFacade<CopyTranslator>(context()),
        BasicCopyTranslator<CopyTranslator>(context()),
        ConditionTranslator<CopyTranslator>(context()),
        ConditionAnalysisTranslator<CopyTranslator>(context()),
        ConditionBuilderTranslator<CopyTranslator>(context()),
        ConditionDnfTranslator<CopyTranslator>(context()),
        ConditionNnfTranslator<CopyTranslator>(context()),
        ConditionQuantifierTranslator<CopyTranslator>(context()),
        EffectTranslator<CopyTranslator>(context()),
        EffectNormalFormTranslator<CopyTranslator>(context()),
        EqualityTranslator<CopyTranslator>(context()),
        GoalSimplificationTranslator<CopyTranslator>(context()),
        TopLevelTranslator<CopyTranslator>(context()),
        TypeTranslator<CopyTranslator>(context()),
        VariableRenamingTranslator<CopyTranslator>(context())
    {
    }

    using CopyTranslatorFacade<CopyTranslator>::copy_domain;
    using CopyTranslatorFacade<CopyTranslator>::copy_task;
    using CopyTranslatorFacade<CopyTranslator>::public_as_or;
    using CopyTranslatorFacade<CopyTranslator>::split_disjunctive_actions;
    using CopyTranslatorFacade<CopyTranslator>::split_disjunctive_axioms;

    using BasicCopyTranslator<CopyTranslator>::copy;
    using BasicCopyTranslator<CopyTranslator>::copy_list;
    using BasicCopyTranslator<CopyTranslator>::copy_optional;
    using BasicCopyTranslator<CopyTranslator>::copy_parameters;
    using BasicCopyTranslator<CopyTranslator>::increment_quantifications;
    using BasicCopyTranslator<CopyTranslator>::enter_scope;
    using BasicCopyTranslator<CopyTranslator>::leave_scope;
    using BasicCopyTranslator<CopyTranslator>::append_generated_domain_objects;
    using BasicCopyTranslator<CopyTranslator>::update_translated_domain;
    using BasicCopyTranslator<CopyTranslator>::strip_typing_requirement;
    using ConditionTranslator<CopyTranslator>::copy;

    using ConditionAnalysisTranslator<CopyTranslator>::term_from_variable;
    using ConditionAnalysisTranslator<CopyTranslator>::collect_free_variables;
    using ConditionAnalysisTranslator<CopyTranslator>::free_parameters_in_scope;

    using ConditionBuilderTranslator<CopyTranslator>::wrap_condition;
    using ConditionBuilderTranslator<CopyTranslator>::as_or;
    using ConditionBuilderTranslator<CopyTranslator>::flatten_condition;
    using ConditionBuilderTranslator<CopyTranslator>::make_conjunction;
    using ConditionBuilderTranslator<CopyTranslator>::make_disjunction;

    using ConditionDnfTranslator<CopyTranslator>::to_dnf;
    using ConditionDnfTranslator<CopyTranslator>::to_dnf_node;

    using ConditionNnfTranslator<CopyTranslator>::negate_comparator;
    using ConditionNnfTranslator<CopyTranslator>::negate_condition;
    using ConditionNnfTranslator<CopyTranslator>::negate_condition_node;
    using ConditionNnfTranslator<CopyTranslator>::copy_condition_node;

    using ConditionQuantifierTranslator<CopyTranslator>::make_generated_axiom_condition;
    using ConditionQuantifierTranslator<CopyTranslator>::make_generated_positive_condition;
    using ConditionQuantifierTranslator<CopyTranslator>::as_exists;
    using ConditionQuantifierTranslator<CopyTranslator>::move_existentials;
    using ConditionQuantifierTranslator<CopyTranslator>::move_existentials_node;
    using ConditionQuantifierTranslator<CopyTranslator>::lift_top_level_exists;

    using GoalSimplificationTranslator<CopyTranslator>::make_generated_goal_condition;
    using GoalSimplificationTranslator<CopyTranslator>::simplify_goal_condition;
    using GoalSimplificationTranslator<CopyTranslator>::simplify_goal_condition_node;

    using EffectTranslator<CopyTranslator>::copy;

    using EqualityTranslator<CopyTranslator>::has_requirement;
    using EqualityTranslator<CopyTranslator>::equality_required;
    using EqualityTranslator<CopyTranslator>::ensure_equality_predicate;
    using EqualityTranslator<CopyTranslator>::equality_literal;
    using EqualityTranslator<CopyTranslator>::domain_uses_equality;
    using EqualityTranslator<CopyTranslator>::add_equality_predicate_to_domain;
    using EqualityTranslator<CopyTranslator>::initialize_equality;

    using EffectNormalFormTranslator<CopyTranslator>::wrap_effect;
    using EffectNormalFormTranslator<CopyTranslator>::as_effect;
    using EffectNormalFormTranslator<CopyTranslator>::normalize_effect;
    using EffectNormalFormTranslator<CopyTranslator>::normalize_effect_node;

    using TopLevelTranslator<CopyTranslator>::copy;

    using TypeTranslator<CopyTranslator>::collect_type_hierarchy;
    using TypeTranslator<CopyTranslator>::maybe_strip_types;
    using TypeTranslator<CopyTranslator>::maybe_strip_parameter;
    using TypeTranslator<CopyTranslator>::maybe_strip_parameters;
    using TypeTranslator<CopyTranslator>::maybe_strip_object;
    using TypeTranslator<CopyTranslator>::type_predicate;
    using TypeTranslator<CopyTranslator>::type_literal;
    using TypeTranslator<CopyTranslator>::type_condition;
    using TypeTranslator<CopyTranslator>::type_conditions_for_parameters;
    using TypeTranslator<CopyTranslator>::prepend_type_conditions;
    using TypeTranslator<CopyTranslator>::add_type_predicates_to_domain;
    using TypeTranslator<CopyTranslator>::add_type_literals_for_object;
    using TypeTranslator<CopyTranslator>::initialize_type_literals;

    using VariableRenamingTranslator<CopyTranslator>::enter_variable_scope;
    using VariableRenamingTranslator<CopyTranslator>::leave_variable_scope;
    using VariableRenamingTranslator<CopyTranslator>::rename_parameter;
    using VariableRenamingTranslator<CopyTranslator>::rename_parameters;
    using VariableRenamingTranslator<CopyTranslator>::rename_variables;
    using VariableRenamingTranslator<CopyTranslator>::rename_action_variables;
    using VariableRenamingTranslator<CopyTranslator>::rename_axiom_variables;
};

}  // namespace loki::semantic::detail

#endif
