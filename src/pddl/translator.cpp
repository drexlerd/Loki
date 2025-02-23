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

#include "translator/remove_types.hpp"
#include "translator/to_negation_normal_form.hpp"

namespace loki
{

DomainTranslationResult::DomainTranslationResult(Domain original_domain, Domain translated_domain) :
    original_domain(std::move(original_domain)),
    translated_domain(std::move(translated_domain))
{
}

DomainTranslationResult translate(const Domain& domain)
{
    auto to_negation_normal_form_translator = ToNegationNormalFormTranslator();
    auto builder = DomainBuilder();
    auto translated_domain = to_negation_normal_form_translator.translate_level_0(domain, builder);

    auto remove_types_translator = RemoveTypesTranslator();
    builder = DomainBuilder();
    translated_domain = remove_types_translator.translate_level_0(translated_domain, builder);

    return DomainTranslationResult(domain, translated_domain);
}

const Domain& DomainTranslationResult::get_original_domain() const { return original_domain; }

const Domain& DomainTranslationResult::get_translated_domain() const { return translated_domain; }

Problem translate(const Problem& problem, const DomainTranslationResult& result)
{
    if (result.get_original_domain() != problem->get_domain())
    {
        throw std::runtime_error("translate(problem, result): domain in problem must match original domain in DomainTranslationResult.");
    }

    auto to_negation_normal_form_translator = ToNegationNormalFormTranslator();
    auto builder = ProblemBuilder(result.get_translated_domain());
    auto translated_problem = to_negation_normal_form_translator.translate_level_0(problem, builder);

    auto remove_types_translator = RemoveTypesTranslator();
    builder = ProblemBuilder(result.get_translated_domain());
    translated_problem = remove_types_translator.translate_level_0(translated_problem, builder);

    return translated_problem;
}

}
