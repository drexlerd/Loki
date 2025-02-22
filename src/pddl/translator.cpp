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

#include "translator/to_negation_normal_form_domain.hpp"
#include "translator/to_negation_normal_form_problem.hpp"

namespace loki
{

DomainTranslationResult::DomainTranslationResult(Domain original_domain, Domain domain) : original_domain(std::move(original_domain)), domain(std::move(domain))
{
}

DomainTranslationResult translate(const Domain& domain)
{
    auto to_negation_normal_form_translator = DomainToNegationNormalFormTranslator();

    to_negation_normal_form_translator.translate_level_0(domain);

    return DomainTranslationResult(domain, domain);
}

ProblemTranslationResult::ProblemTranslationResult(Problem problem) : problem(std::move(problem)) {}

ProblemTranslationResult translate(const Problem& problem, const DomainTranslationResult& result) { return ProblemTranslationResult(problem); }

}
