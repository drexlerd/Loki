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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_NEGATION_NORMAL_FORM_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{
class ToNegationNormalFormTranslator : public RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>
{
private:
    friend class RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>::prepare_level_2;
    using RecursiveCachedBaseTranslator<ToNegationNormalFormTranslator>::translate_level_2;

    /**
     * Translate
     */

    /**
     * Eliminate implication
     *
     * 1. A -> B   =>  not A or B
     * 2. A <-> B  =>  (not A or B) and (A or not B)
     */

    Condition translate_level_2(ConditionImply condition, Repositories& repositories);

    /**
     * Push negation inwards.
     *
     * 3. not (A or B)    =>  not A and not B
     * 4. not (A and B)   =>  not A or not B
     * 5. not (not A)     =>  A
     * 6. not exists x A  =>  forall x not A
     * 7. not forall x A  =>  exists x not A
     * 8. not (A -> B)    =>  not (not A or B)  =>  A and not B
     */

    Condition translate_level_2(ConditionNot condition, Repositories& repositories);

    /**
     * Flatten conjunctions.
     *
     * 9. A and (B and C)  =>  A and B and C
     */

    Condition translate_level_2(ConditionAnd condition, Repositories& repositories);

    /**
     * Flatten disjunctions.
     *
     * 10. A or (B or C)  =>  A or B or C
     */

    Condition translate_level_2(ConditionOr condition, Repositories& repositories);

    /**
     * Flatten existential quantifiers.
     *
     * 11. exists(vars1, exists(vars2, A))  =>  exists(vars1+vars2, A)
     */

    Condition translate_level_2(ConditionExists condition, Repositories& repositories);

    /**
     * Flatten universal quantifiers.
     *
     * 12. forall(vars1, forall(vars2, A))  =>  forall(vars1+vars2, A)
     */

    Condition translate_level_2(ConditionForall condition, Repositories& repositories);
};

}

#endif
