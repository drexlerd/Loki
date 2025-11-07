/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "loki/details/pddl/translator.hpp"

#include "translator/add_type_predicates.hpp"
#include "translator/initialize_equality.hpp"
#include "translator/move_existential_quantifiers.hpp"
#include "translator/remove_universal_quantifiers.hpp"
#include "translator/rename_quantified_variables.hpp"
#include "translator/simplify_goal.hpp"
#include "translator/split_disjunctive_conditions.hpp"
#include "translator/to_disjunctive_normal_form.hpp"
#include "translator/to_effect_normal_form.hpp"
#include "translator/to_negation_normal_form.hpp"

namespace loki
{

DomainTranslationResult::DomainTranslationResult(Domain original_domain, Domain translated_typed_domain, Domain translated_domain) :
    original_domain(std::move(original_domain)),
    translated_typed_domain(std::move(translated_typed_domain)),
    translated_domain(std::move(translated_domain))
{
}

DomainTranslationResult translate(const Domain& domain, const TranslatorOptions& options)
{
    // std::cout << "Given domain" << std::endl;
    // std::cout << *domain << std::endl;

    auto to_negation_normal_form_translator = ToNegationNormalFormTranslator();
    auto builder = DomainBuilder();
    auto translated_domain = to_negation_normal_form_translator.translate_level_0(domain, builder);

    // std::cout << "ToNegationNormalFormTranslator result:" << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto rename_quantified_variables_translator = RenameQuantifiedVariablesTranslator();
    builder = DomainBuilder();
    translated_domain = rename_quantified_variables_translator.translate_level_0(translated_domain, builder);

    // std::cout << "RenameQuantifiedVariablesTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto remove_universal_quantifiers_translator = RemoveUniversalQuantifiersTranslator();
    builder = DomainBuilder();
    translated_domain = remove_universal_quantifiers_translator.translate_level_0(translated_domain, builder);

    // std::cout << "RemoveUniversalQuantifiersTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto to_disjunctive_normal_form_translator = ToDisjunctiveNormalFormTranslator();
    builder = DomainBuilder();
    translated_domain = to_disjunctive_normal_form_translator.translate_level_0(translated_domain, builder);

    // std::cout << "ToDisjunctiveNormalFormTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto split_disjunctive_conditions_translator = SplitDisjunctiveConditionsTranslator();
    builder = DomainBuilder();
    translated_domain = split_disjunctive_conditions_translator.translate_level_0(translated_domain, builder);

    // std::cout << "SplitDisjunctiveConditionsTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto move_existential_quantifiers_translator = MoveExistentialQuantifiersTranslator();
    builder = DomainBuilder();
    translated_domain = move_existential_quantifiers_translator.translate_level_0(translated_domain, builder);

    // std::cout << "MoveExistentialQuantifiersTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto to_effect_normal_form_translator = ToEffectNormalFormTranslator();
    builder = DomainBuilder();
    translated_domain = to_effect_normal_form_translator.translate_level_0(translated_domain, builder);

    // std::cout << "ToEffectNormalFormTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    auto initialize_equality_translator = InitializeEqualityTranslator();
    builder = DomainBuilder();
    translated_domain = initialize_equality_translator.translate_level_0(translated_domain, builder);
    auto translated_typed_domain = translated_domain;

    // std::cout << "InitializeEqualityTranslator result: " << std::endl;
    // std::cout << *translated_domain << std::endl;

    /* Type translator cannot come before removal of universal quantifiers because of negations. */
    auto remove_types_translator = AddTypePredicatesTranslator(options.remove_typing);
    builder = DomainBuilder();
    translated_domain = remove_types_translator.translate_level_0(translated_domain, builder);

    // std::cout << "AddTypePredicatesTranslator result:" << std::endl;
    // std::cout << *translated_domain << std::endl;

    return DomainTranslationResult(domain, translated_typed_domain, translated_domain);
}

const Domain& DomainTranslationResult::get_original_domain() const { return original_domain; }

const Domain& DomainTranslationResult::get_translated_typed_domain() const { return translated_typed_domain; }

const Domain& DomainTranslationResult::get_translated_domain() const { return translated_domain; }

Problem translate(const Problem& problem, const DomainTranslationResult& result, const TranslatorOptions& options)
{
    if (result.get_original_domain() != problem->get_domain())
    {
        throw std::runtime_error("translate(problem, result): domain in problem must match original domain in DomainTranslationResult.");
    }

    auto translated_problem = problem;

    auto to_negation_normal_form_translator = ToNegationNormalFormTranslator();
    auto builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = to_negation_normal_form_translator.translate_level_0(translated_problem, builder);

    // std::cout << "ToNegationNormalFormTranslator result: " << std::endl;

    auto rename_quantified_variables_translator = RenameQuantifiedVariablesTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = rename_quantified_variables_translator.translate_level_0(translated_problem, builder);

    // std::cout << "RenameQuantifiedVariablesTranslator result: " << std::endl;

    auto remove_universal_quantifiers_translator = RemoveUniversalQuantifiersTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = remove_universal_quantifiers_translator.translate_level_0(translated_problem, builder);

    // std::cout << "RemoveUniversalQuantifiersTranslator result: " << std::endl;

    auto simplify_goal_translator = SimplifyGoalTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = simplify_goal_translator.translate_level_0(translated_problem, builder);

    // std::cout << "SimplifyGoalTranslator result: " << std::endl;

    auto to_disjunctive_normal_form_translator = ToDisjunctiveNormalFormTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = to_disjunctive_normal_form_translator.translate_level_0(translated_problem, builder);

    // std::cout << "ToDisjunctiveNormalFormTranslator result: " << std::endl;

    auto split_disjunctive_conditions_translator = SplitDisjunctiveConditionsTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = split_disjunctive_conditions_translator.translate_level_0(translated_problem, builder);

    // std::cout << "SplitDisjunctiveConditionsTranslator result: " << std::endl;

    auto move_existential_quantifiers_translator = MoveExistentialQuantifiersTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = move_existential_quantifiers_translator.translate_level_0(translated_problem, builder);

    // std::cout << "MoveExistentialQuantifiersTranslator result: " << std::endl;

    auto to_effect_normal_form_translator = ToEffectNormalFormTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = to_effect_normal_form_translator.translate_level_0(translated_problem, builder);

    // std::cout << "ToEffectNormalFormTranslator result: " << std::endl;

    auto initialize_equality_translator = InitializeEqualityTranslator();
    builder = ProblemBuilder(result.get_translated_typed_domain());
    translated_problem = initialize_equality_translator.translate_level_0(translated_problem, builder);

    // std::cout << "InitializeEqualityTranslator result: " << std::endl;

    auto remove_types_translator = AddTypePredicatesTranslator(options.remove_typing);
    builder = ProblemBuilder(result.get_translated_domain());
    translated_problem = remove_types_translator.translate_level_0(translated_problem, builder);

    // std::cout << "AddTypePredicatesTranslator result: " << std::endl;

    return translated_problem;
}

}
