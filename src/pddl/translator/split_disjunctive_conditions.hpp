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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_SPLIT_DISJUNCTIVE_CONDITIONS_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_SPLIT_DISJUNCTIVE_CONDITIONS_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{

/**
 * Split actions and axioms at the disjunction at the root of a condition
 * and conditional effects using the following rules:
 *
 * 1. (phi or psi) > e  => (phi > e) and (psi > e)
 * 2. <(phi or psi), e>  => <phi, e> and <psi, e>
 */
class SplitDisjunctiveConditionsTranslator : public RecursiveCachedBaseTranslator<SplitDisjunctiveConditionsTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<SplitDisjunctiveConditionsTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    ActionList split_actions_at_disjunction(const ActionList& actions, Repositories& repositories);
    AxiomList split_axioms_at_disjunction(const AxiomList& axioms, Repositories& repositories);

    /**
     * Split conditional effects
     *
     * 1. (phi or psi) > e  => (phi > e) and (psi > e)
     */
    Effect translate_level_2(EffectCompositeWhen effect, Repositories& repositories);
    /**
     * Split actions and actions at conditions
     *
     * 2. <(phi or psi), e>  => <phi, e> and <psi, e>
     */
    Domain translate_level_2(const Domain& domain, DomainBuilder& builder);
    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};

}

#endif
