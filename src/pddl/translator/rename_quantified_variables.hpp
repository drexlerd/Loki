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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_RENAME_QUANTIFIED_VARIABLES_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_RENAME_QUANTIFIED_VARIABLES_HPP_

#include "recursive_base.hpp"

#include <deque>

namespace loki
{

/**
 * Rename all variables to be able to move quantifiers around.
 */
class RenameQuantifiedVariablesTranslator : public RecursiveBaseTranslator<RenameQuantifiedVariablesTranslator>
{
private:
    /* Implement RecursiveBaseTranslator interface. */
    friend class RecursiveBaseTranslator<RenameQuantifiedVariablesTranslator>;

    // Provide default implementations
    // Note: for context dependent translations, we cannot use the BaseCachedRecurseTranslator.
    using RecursiveBaseTranslator::translate_level_2;

    /* Translate step */

    std::unordered_map<Variable, size_t> m_num_quantifications;

    bool m_renaming_enabled;

    Variable translate_level_2(Variable variable, Repositories& repositories);

    /**
     * Keep variable names of parameters in these constructs, i.e., turn off renaming
     */
    Predicate translate_level_2(Predicate predicate, Repositories& repositories);
    FunctionSkeleton translate_level_2(FunctionSkeleton function_skeleton, Repositories& repositories);

    /**
     * Rename variables names of parameters in these constructs, i.e., turn on renaming
     */
    Condition translate_level_2(ConditionExists condition, Repositories& repositories);
    Condition translate_level_2(ConditionForall condition, Repositories& repositories);
    Effect translate_level_2(EffectCompositeForall effect, Repositories& repositories);
    Action translate_level_2(Action action, Repositories& repositories);
    Axiom translate_level_2(Axiom axiom, Repositories& repositories);

public:
    RenameQuantifiedVariablesTranslator();
};
}

#endif