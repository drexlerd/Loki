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

#include "loki/details/pddl/exceptions.hpp"

namespace loki
{

RequirementEnumSet parse(const ast::RequirementStrips&, Context& /*context*/)
{
    // Strips as the minimal requirement must not be tracked.
    return { RequirementEnum::STRIPS };
}

RequirementEnumSet parse(const ast::RequirementTyping&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::TYPING);
    return { RequirementEnum::TYPING };
}

RequirementEnumSet parse(const ast::RequirementNegativePreconditions&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::NEGATIVE_PRECONDITIONS);
    return { RequirementEnum::NEGATIVE_PRECONDITIONS };
}

RequirementEnumSet parse(const ast::RequirementDisjunctivePreconditions&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    return { RequirementEnum::DISJUNCTIVE_PRECONDITIONS };
}

RequirementEnumSet parse(const ast::RequirementEquality&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::EQUALITY);
    return { RequirementEnum::EQUALITY };
}

RequirementEnumSet parse(const ast::RequirementExistentialPreconditions&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    return { RequirementEnum::EXISTENTIAL_PRECONDITIONS };
}

RequirementEnumSet parse(const ast::RequirementUniversalPreconditions&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

RequirementEnumSet parse(const ast::RequirementQuantifiedPreconditions&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::QUANTIFIED_PRECONDITIONS);
    context.references.track(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.references.track(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { RequirementEnum::QUANTIFIED_PRECONDITIONS, RequirementEnum::EXISTENTIAL_PRECONDITIONS, RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

RequirementEnumSet parse(const ast::RequirementConditionalEffects&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::CONDITIONAL_EFFECTS);
    return { RequirementEnum::CONDITIONAL_EFFECTS };
}

RequirementEnumSet parse(const ast::RequirementFluents&, Context& context)
{
    // Track
    // FLUENTS as a composite must not be tracked
    context.references.track(RequirementEnum::OBJECT_FLUENTS);
    context.references.track(RequirementEnum::NUMERIC_FLUENTS);
    return { RequirementEnum::FLUENTS, RequirementEnum::OBJECT_FLUENTS, RequirementEnum::NUMERIC_FLUENTS };
}

RequirementEnumSet parse(const ast::RequirementObjectFluents& node, Context& context)
{
    // Track
    context.references.track(RequirementEnum::OBJECT_FLUENTS);
    throw UnsupportedRequirementError(RequirementEnum::OBJECT_FLUENTS, context.scopes.top().get_error_handler()(node, ""));

    return { RequirementEnum::OBJECT_FLUENTS };
}

RequirementEnumSet parse(const ast::RequirementNumericFluents&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::NUMERIC_FLUENTS);
    return { RequirementEnum::NUMERIC_FLUENTS };
}

RequirementEnumSet parse(const ast::RequirementAdl&, Context& context)
{
    // Track
    // Strips as the minimal requirement must not be tracked.
    // ADL as a composite must not be tracked
    context.references.track(RequirementEnum::TYPING);
    context.references.track(RequirementEnum::NEGATIVE_PRECONDITIONS);
    context.references.track(RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    context.references.track(RequirementEnum::EQUALITY);
    context.references.track(RequirementEnum::QUANTIFIED_PRECONDITIONS);
    context.references.track(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.references.track(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    context.references.track(RequirementEnum::CONDITIONAL_EFFECTS);
    return { RequirementEnum::ADL,
             RequirementEnum::STRIPS,
             RequirementEnum::TYPING,
             RequirementEnum::NEGATIVE_PRECONDITIONS,
             RequirementEnum::DISJUNCTIVE_PRECONDITIONS,
             RequirementEnum::EQUALITY,
             RequirementEnum::QUANTIFIED_PRECONDITIONS,
             RequirementEnum::EXISTENTIAL_PRECONDITIONS,
             RequirementEnum::UNIVERSAL_PRECONDITIONS,
             RequirementEnum::CONDITIONAL_EFFECTS };
}

RequirementEnumSet parse(const ast::RequirementDurativeActions& node, Context& context)
{
    throw UnsupportedRequirementError(RequirementEnum::DURATIVE_ACTIONS, context.scopes.top().get_error_handler()(node, ""));

    // Track
    context.references.track(RequirementEnum::DURATIVE_ACTIONS);
    return { RequirementEnum::DURATIVE_ACTIONS };
}

RequirementEnumSet parse(const ast::RequirementDerivedPredicates&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::DERIVED_PREDICATES);
    return { RequirementEnum::DERIVED_PREDICATES };
}

RequirementEnumSet parse(const ast::RequirementTimedInitialLiterals& node, Context& context)
{
    throw UnsupportedRequirementError(RequirementEnum::TIMED_INITIAL_LITERALS, context.scopes.top().get_error_handler()(node, ""));

    // Track
    context.references.track(RequirementEnum::TIMED_INITIAL_LITERALS);
    context.references.track(RequirementEnum::DURATIVE_ACTIONS);
    return { RequirementEnum::TIMED_INITIAL_LITERALS, RequirementEnum::DURATIVE_ACTIONS };
}

RequirementEnumSet parse(const ast::RequirementPreferences& node, Context& context)
{
    throw UnsupportedRequirementError(RequirementEnum::PREFERENCES, context.scopes.top().get_error_handler()(node, ""));

    // Track
    context.references.track(RequirementEnum::PREFERENCES);
    return { RequirementEnum::PREFERENCES };
}

RequirementEnumSet parse(const ast::RequirementConstraints& node, Context& context)
{
    throw UnsupportedRequirementError(RequirementEnum::CONSTRAINTS, context.scopes.top().get_error_handler()(node, ""));

    // Track
    context.references.track(RequirementEnum::CONSTRAINTS);
    return { RequirementEnum::CONSTRAINTS };
}

RequirementEnumSet parse(const ast::RequirementActionCosts&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::ACTION_COSTS);
    return { RequirementEnum::ACTION_COSTS };
}

RequirementEnumSet parse(const ast::RequirementNonDeterministic&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::NON_DETERMINISTIC);
    return { RequirementEnum::NON_DETERMINISTIC };
}

RequirementEnumSet parse(const ast::RequirementProbabilisticEffects&, Context& context)
{
    // Track
    context.references.track(RequirementEnum::PROBABILISTIC);
    return { RequirementEnum::PROBABILISTIC };
}

RequirementEnumSet parse(const ast::Requirement& node, Context& context) { return boost::apply_visitor(RequirementVisitor(context), node); }

RequirementVisitor::RequirementVisitor(Context& context_) : context(context_) {}

RequirementEnumSet parse(const ast::Requirements& requirements_node, Context& context)
{
    auto requirements = RequirementEnumSet();
    for (const auto& requirement : requirements_node.requirements)
    {
        auto additional_requirements = parse(requirement, context);
        requirements.insert(additional_requirements.begin(), additional_requirements.end());
    }
    return requirements;
}

}
