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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_DOMAIN_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_DOMAIN_HPP_

#include "loki/details/pddl/domain.hpp"
#include "to_negation_normal_form_base.hpp"

namespace loki
{
class DomainToNegationNormalFormTranslator : public ToNegationNormalFormTranslatorBase<DomainToNegationNormalFormTranslator>
{
private:
    friend class ToNegationNormalFormTranslatorBase<DomainToNegationNormalFormTranslator>;

    // Provide default implementations
    using ToNegationNormalFormTranslatorBase<DomainToNegationNormalFormTranslator>::prepare_level_2;
    using ToNegationNormalFormTranslatorBase<DomainToNegationNormalFormTranslator>::translate_level_2;
};
}

#endif
