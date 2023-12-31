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

#include "../../../../include/domain/pddl/exceptions.hpp"

using namespace loki::domain;


namespace loki {

pddl::RequirementEnumSet parse(const ast::RequirementStrips&, Context& /*context*/) {
    // Strips as the minimal requirement must not be tracked.
    return { pddl::RequirementEnum::STRIPS };
}

pddl::RequirementEnumSet parse(const ast::RequirementTyping&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::TYPING);
    return { pddl::RequirementEnum::TYPING };
}

pddl::RequirementEnumSet parse(const ast::RequirementNegativePreconditions&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS);
    return { pddl::RequirementEnum::NEGATIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDisjunctivePreconditions&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    return { pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementEquality&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::EQUALITY);
    return { pddl::RequirementEnum::EQUALITY };
}

pddl::RequirementEnumSet parse(const ast::RequirementExistentialPreconditions&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    return { pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementUniversalPreconditions&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementQuantifiedPreconditions&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS,
             pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementConditionalEffects&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::CONDITIONAL_EFFECTS);
    return { pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementFluents&, Context& context) {
    // Track
    // FLUENTS as a composite must not be tracked
    context.referenced_values.track(pddl::RequirementEnum::OBJECT_FLUENTS);
    context.referenced_values.track(pddl::RequirementEnum::NUMERIC_FLUENTS);
    return { pddl::RequirementEnum::FLUENTS,
             pddl::RequirementEnum::OBJECT_FLUENTS,
             pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementObjectFluents& node, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::OBJECT_FLUENTS);
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::OBJECT_FLUENTS,
        context.scopes.get_error_handler()(node, ""));

    return { pddl::RequirementEnum::OBJECT_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementNumericFluents&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::NUMERIC_FLUENTS);
    return { pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementAdl&, Context& context) {
    // Track
    // Strips as the minimal requirement must not be tracked.
    // ADL as a composite must not be tracked
    context.referenced_values.track(pddl::RequirementEnum::TYPING);
    context.referenced_values.track(pddl::RequirementEnum::NEGATIVE_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::EQUALITY);
    context.referenced_values.track(pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS);
    context.referenced_values.track(pddl::RequirementEnum::CONDITIONAL_EFFECTS);
    return { pddl::RequirementEnum::ADL,
             pddl::RequirementEnum::STRIPS,
             pddl::RequirementEnum::TYPING,
             pddl::RequirementEnum::NEGATIVE_PRECONDITIONS,
             pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS,
             pddl::RequirementEnum::EQUALITY,
             pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS,
             pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS,
             pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDurativeActions& node, Context& context) {
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::DURATIVE_ACTIONS,
        context.scopes.get_error_handler()(node, ""));

    // Track
    context.referenced_values.track(pddl::RequirementEnum::DURATIVE_ACTIONS);
    return { pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDerivedPredicates& node, Context& context) {
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::DERIVED_PREDICATES,
        context.scopes.get_error_handler()(node, ""));

    // Track
    context.referenced_values.track(pddl::RequirementEnum::DERIVED_PREDICATES);
    return { pddl::RequirementEnum::DERIVED_PREDICATES };
}

pddl::RequirementEnumSet parse(const ast::RequirementTimedInitialLiterals& node, Context& context) {
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::TIMED_INITIAL_LITERALS,
        context.scopes.get_error_handler()(node, ""));

    // Track
    context.referenced_values.track(pddl::RequirementEnum::TIMED_INITIAL_LITERALS);
    context.referenced_values.track(pddl::RequirementEnum::DURATIVE_ACTIONS);
    return { pddl::RequirementEnum::TIMED_INITIAL_LITERALS,
             pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementPreferences& node, Context& context) {
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::PREFERENCES,
        context.scopes.get_error_handler()(node, ""));

    // Track
    context.referenced_values.track(pddl::RequirementEnum::PREFERENCES);
    return { pddl::RequirementEnum::PREFERENCES };
}

pddl::RequirementEnumSet parse(const ast::RequirementConstraints& node, Context& context) {
    throw UnsupportedRequirementError(
        pddl::RequirementEnum::CONSTRAINTS,
        context.scopes.get_error_handler()(node, ""));

    // Track
    context.referenced_values.track(pddl::RequirementEnum::CONSTRAINTS);
    return { pddl::RequirementEnum::CONSTRAINTS };

}

pddl::RequirementEnumSet parse(const ast::RequirementActionCosts&, Context& context) {
    // Track
    context.referenced_values.track(pddl::RequirementEnum::ACTION_COSTS);
    return { pddl::RequirementEnum::ACTION_COSTS };
}

pddl::RequirementEnumSet parse(const ast::Requirement& node, Context& context) {
    return boost::apply_visitor(RequirementVisitor(context), node);
}

RequirementVisitor::RequirementVisitor(Context& context_) : context(context_) { }

pddl::RequirementEnumSet parse(const ast::Requirements& requirements_node, Context& context) {
    auto requirements = pddl::RequirementEnumSet();
    for (const auto& requirement : requirements_node.requirements) {
        auto additional_requirements = parse(requirement, context);
        requirements.insert(additional_requirements.begin(), additional_requirements.end());
    }
    return requirements;
}

}
