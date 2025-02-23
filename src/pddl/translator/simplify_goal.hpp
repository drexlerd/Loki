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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_SIMPLIFY_GOAL_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_SIMPLIFY_GOAL_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{

/**
 * Introduce an axiom for complicated goals, i.e., goals that are not conjunctions of literals.
 */
class SimplifyGoalTranslator : public RecursiveCachedBaseTranslator<SimplifyGoalTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<SimplifyGoalTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::prepare_level_2;
    using RecursiveCachedBaseTranslator::translate_level_2;

    Condition simplify_goal_condition(Condition goal_condition,
                                      Repositories& repositories,
                                      PredicateList& instantiated_predicates,
                                      AxiomList& instantiated_axioms,
                                      size_t& next_axiom_index,
                                      std::unordered_set<std::string>& problem_and_domain_predicate_names);

    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};
}

#endif