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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_TO_DISJUNCTIVE_NORMAL_FORM_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_TO_DISJUNCTIVE_NORMAL_FORM_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{
/**
 * Translate formulas to disjunctive normal form (DNF) in bottom-up manner using the following rules.
 *
 * First, applies NNF translator follow by:
 *
 * 1. A and (B or C)  =>  A and B or A and C
 * 2. exists(vars, A or B)  =>  exists(vars, A) or exists(vars, B)
 * 3. forall(vars, A or B)  =>  forall(vars, A) or forall(vars, B)
 */
class ToDisjunctiveNormalFormTranslator : public RecursiveCachedBaseTranslator<ToDisjunctiveNormalFormTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<ToDisjunctiveNormalFormTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    /**
     * Translate
     */

    /**
     * Apply conjunctive disjunctive distributivity.
     *
     * 1. A and (B or C)  =>  A and B or A and C
     */
    Condition translate_level_2(ConditionAnd condition, Repositories& repositories);
    /**
     * Apply flattening of conjunctions.
     *
     * 2. A or (B or C)  =>  A or B or C
     */
    Condition translate_level_2(ConditionOr condition, Repositories& repositories);
    /**
     * 3. exists(vars, A or B)  =>  exists(vars, A) or exists(vars, B)
     */
    Condition translate_level_2(ConditionExists condition, Repositories& repositories);
    /**
     * 4. forall(vars, A or B)  =>  forall(vars, A) or forall(vars, B)
     */
    Condition translate_level_2(ConditionForall condition, Repositories& repositories);

    Domain translate_level_2(const Domain& domain, DomainBuilder& builder);
    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};

}

#endif
