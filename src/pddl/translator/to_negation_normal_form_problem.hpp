/*
 * Copyright (C) 2023 Dominik Drexler and Simon Stahlberg
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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_PROBLEM_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_PROBLEM_HPP_

#include "loki/details/pddl/problem.hpp"
#include "to_negation_normal_form_base.hpp"

namespace loki
{

class ProblemToNegationNormalFormTranslator : public ToNegationNormalFormTranslatorBase<ProblemToNegationNormalFormTranslator>
{
private:
    const DomainTranslationResult& m_domain_translation_result;

    friend class ToNegationNormalFormTranslatorBase<ProblemToNegationNormalFormTranslator>;

    // Provide default implementations
    using ToNegationNormalFormTranslatorBase<ProblemToNegationNormalFormTranslator>::prepare_level_2;
    using ToNegationNormalFormTranslatorBase<ProblemToNegationNormalFormTranslator>::translate_level_2;

public:
    explicit ProblemToNegationNormalFormTranslator(const DomainTranslationResult& domain_translation_result) :
        m_domain_translation_result(domain_translation_result)
    {
    }
};
}

#endif
