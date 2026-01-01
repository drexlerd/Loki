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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_UTILS_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_UTILS_HPP_

#include "loki/details/pddl/declarations.hpp"

#include <random>

namespace loki
{

/**
 * Conditions
 */

/**
 * Flatten conjunctions.
 *
 * A and (B and C)  =>  A and B and C
 */
extern Condition flatten(ConditionAnd condition, Repositories& pddl_repositories);

/**
 * Flatten conjunctions.
 *
 * e1 and (e2 and e3)  =>  e1 and e2 and e3
 */
extern Effect flatten(EffectAnd effect, Repositories& pddl_repositories);

/**
 * Flatten disjunctions.
 *
 * A or (B or C)  =>  A or B or C
 */
extern Condition flatten(ConditionOr condition, Repositories& pddl_repositories);

/**
 * Flatten existential quantifiers.
 *
 * exists(vars1, exists(vars2, A))  =>  exists(vars1+vars2, A)
 */
extern Condition flatten(ConditionExists condition, Repositories& pddl_repositories);

/**
 * Flatten universal quantifiers.
 *
 * forall(vars1, forall(vars2, A))  =>  forall(vars1+vars2, A)
 */
extern Condition flatten(ConditionForall condition, Repositories& pddl_repositories);

/**
 * Flatten condition
 */

extern Condition flatten(Condition condition, Repositories& pddl_repositories);

/**
 * Flatten conditional when
 *
 * A > (B > e)    =>  (A and B) > e
 */
extern Effect flatten(EffectCompositeWhen effect, Repositories& pddl_repositories);

/**
 * Flatten conditional forall
 *
 * forall(vars1, forall(vars2, e))    =>  forall(vars1+vars2, e)
 */
extern Effect flatten(EffectCompositeForall effect, Repositories& pddl_repositories);

/**
 * Flatten effect
 */

extern Effect flatten(Effect effect, Repositories& pddl_repositories);

/**
 * Axioms
 */

extern std::string create_unique_axiom_name(size_t& next_axiom_index, std::unordered_set<std::string>& existing_predicate_names);

}

#endif
