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

#include "requirements.hpp"

using namespace loki::domain;

namespace loki {

pddl::RequirementEnumSet parse(const ast::RequirementStrips&) {
    return { pddl::RequirementEnum::STRIPS };
}

pddl::RequirementEnumSet parse(const ast::RequirementTyping&) {
    return { pddl::RequirementEnum::TYPING };
}

pddl::RequirementEnumSet parse(const ast::RequirementNegativePreconditions&) {
    return { pddl::RequirementEnum::NEGATIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDisjunctivePreconditions&) {
    return { pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementEquality&) {
    return { pddl::RequirementEnum::EQUALITY };
}

pddl::RequirementEnumSet parse(const ast::RequirementExistentialPreconditions&) {
    return { pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementUniversalPreconditions&) {
    return { pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementQuantifiedPreconditions&) {
    return { pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS,
             pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementConditionalEffects&) {
    return { pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementFluents&) {
    return { pddl::RequirementEnum::FLUENTS,
             pddl::RequirementEnum::OBJECT_FLUENTS,
             pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementObjectFluents&) {
    return { pddl::RequirementEnum::OBJECT_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementNumericFluents&) {
    return { pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementAdl&) {
    return { pddl::RequirementEnum::ADL,
             pddl::RequirementEnum::STRIPS,
             pddl::RequirementEnum::TYPING,
             pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS,
             pddl::RequirementEnum::EQUALITY,
             pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDurativeActions&) {
    return { pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDerivedPredicates&) {
    return { pddl::RequirementEnum::DERIVED_PREDICATES };
}

pddl::RequirementEnumSet parse(const ast::RequirementTimedInitialLiterals&) {
    return { pddl::RequirementEnum::TIMED_INITIAL_LITERALS,
             pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementPreferences&) {
    return { pddl::RequirementEnum::PREFERENCES };
}

pddl::RequirementEnumSet parse(const ast::RequirementConstraints&) {
    return { pddl::RequirementEnum::CONSTRAINTS };

}

pddl::RequirementEnumSet parse(const ast::RequirementActionCosts&) {
    return { pddl::RequirementEnum::ACTION_COSTS };
}

pddl::RequirementEnumSet parse(const ast::Requirement& node) {
    return boost::apply_visitor(RequirementVisitor(), node);
}

pddl::RequirementEnumSet parse(const ast::Requirements& requirements_node) {
    pddl::RequirementEnumSet requirements;
    for (const auto& requirement : requirements_node.requirements) {
        auto additional_requirements = parse(requirement);
        requirements.insert(additional_requirements.begin(), additional_requirements.end());
    }
    return requirements;
}

}
