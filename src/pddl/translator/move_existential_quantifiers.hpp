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
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_SRC_PDDL_TRANSLATOR_MOVE_EXISTENTIAL_QUANTIFIERS_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_MOVE_EXISTENTIAL_QUANTIFIERS_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{

/**
 * Move existential quantifiers to the root in bottom up manner using the following rules:
 *
 * 1. and(phi, exists(vars, psi))       =>  exists(vars, and(phi, psi))
 * 2. exists(vars, exists(vars', phi))  =>  exists(vars + vars', phi)
 */
class MoveExistentialQuantifiersTranslator : public RecursiveCachedBaseTranslator<MoveExistentialQuantifiersTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<MoveExistentialQuantifiersTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    /**
     * Push existential quantifiers to the root.
     *
     * 1. and(phi, exists(vars, psi))       =>  exists(vars, and(phi, psi))
     */
    Condition translate_level_2(ConditionAnd condition, Repositories& repositories);
    /**
     * Flatten existential quantifier
     *
     * 2. exists(vars, exists(vars', phi))  =>  exists(vars + vars', phi)
     */
    Condition translate_level_2(ConditionExists condition, Repositories& repositories);

    /**
     * Remove existential from the the root into the action parameters
     */
    Action translate_level_2(Action action, Repositories& repositories);

    /**
     * Remove existential from the the root into the action parameters
     */
    Axiom translate_level_2(Axiom axiom, Repositories& repositories);
};
}

#endif