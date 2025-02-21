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

#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"

namespace loki
{

template<ParsingContext C>
RequirementEnumSet parse(const ast::Requirements& requirements_node, C& context)
{
    auto requirements = RequirementEnumSet();
    auto visitor = RequirementVisitor(context);
    for (const auto& requirement : requirements_node.requirements)
    {
        auto additional_requirements = boost::apply_visitor(visitor, requirement);
        requirements.insert(additional_requirements.begin(), additional_requirements.end());
    }
    return requirements;
}

template RequirementEnumSet parse(const ast::Requirements& requirements_node, DomainParsingContext& context);
template RequirementEnumSet parse(const ast::Requirements& requirements_node, ProblemParsingContext& context);

template<ParsingContext C>
RequirementVisitor<C>::RequirementVisitor(C& context) : context(context)
{
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementStrips&)
{
    // Strips as the minimal requirement must not be tracked.
    return { RequirementEnum::STRIPS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementTyping&)
{
    context.references.track(RequirementEnum::TYPING);
    return { RequirementEnum::TYPING };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementNegativePreconditions&)
{
    context.references.track(RequirementEnum::NEGATIVE_PRECONDITIONS);
    return { RequirementEnum::NEGATIVE_PRECONDITIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementDisjunctivePreconditions&)
{
    context.references.track(RequirementEnum::DISJUNCTIVE_PRECONDITIONS);
    return { RequirementEnum::DISJUNCTIVE_PRECONDITIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementEquality&)
{
    context.references.track(RequirementEnum::EQUALITY);
    return { RequirementEnum::EQUALITY };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementExistentialPreconditions&)
{
    context.references.track(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    return { RequirementEnum::EXISTENTIAL_PRECONDITIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementUniversalPreconditions&)
{
    context.references.track(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementQuantifiedPreconditions&)
{
    context.references.track(RequirementEnum::QUANTIFIED_PRECONDITIONS);
    context.references.track(RequirementEnum::EXISTENTIAL_PRECONDITIONS);
    context.references.track(RequirementEnum::UNIVERSAL_PRECONDITIONS);
    return { RequirementEnum::QUANTIFIED_PRECONDITIONS, RequirementEnum::EXISTENTIAL_PRECONDITIONS, RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementConditionalEffects&)
{
    context.references.track(RequirementEnum::CONDITIONAL_EFFECTS);
    return { RequirementEnum::CONDITIONAL_EFFECTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementFluents&)
{
    context.references.track(RequirementEnum::OBJECT_FLUENTS);
    context.references.track(RequirementEnum::NUMERIC_FLUENTS);
    return { RequirementEnum::FLUENTS, RequirementEnum::OBJECT_FLUENTS, RequirementEnum::NUMERIC_FLUENTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementObjectFluents& node)
{
    context.references.track(RequirementEnum::OBJECT_FLUENTS);
    throw UnsupportedRequirementError(RequirementEnum::OBJECT_FLUENTS, context.scopes.top().get_error_handler()(node, ""));

    return { RequirementEnum::OBJECT_FLUENTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementFunctionValues&)
{
    context.references.track(RequirementEnum::NUMERIC_FLUENTS);
    return { RequirementEnum::NUMERIC_FLUENTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementAdl&)
{
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

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementDurativeActions& node)
{
    throw UnsupportedRequirementError(RequirementEnum::DURATIVE_ACTIONS, context.scopes.top().get_error_handler()(node, ""));

    context.references.track(RequirementEnum::DURATIVE_ACTIONS);
    return { RequirementEnum::DURATIVE_ACTIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementDerivedPredicates&)
{
    context.references.track(RequirementEnum::DERIVED_PREDICATES);
    return { RequirementEnum::DERIVED_PREDICATES };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementTimedInitialLiterals& node)
{
    throw UnsupportedRequirementError(RequirementEnum::TIMED_INITIAL_LITERALS, context.scopes.top().get_error_handler()(node, ""));

    context.references.track(RequirementEnum::TIMED_INITIAL_LITERALS);
    context.references.track(RequirementEnum::DURATIVE_ACTIONS);
    return { RequirementEnum::TIMED_INITIAL_LITERALS, RequirementEnum::DURATIVE_ACTIONS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementPreferences& node)
{
    throw UnsupportedRequirementError(RequirementEnum::PREFERENCES, context.scopes.top().get_error_handler()(node, ""));

    context.references.track(RequirementEnum::PREFERENCES);
    return { RequirementEnum::PREFERENCES };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementConstraints& node)
{
    throw UnsupportedRequirementError(RequirementEnum::CONSTRAINTS, context.scopes.top().get_error_handler()(node, ""));

    context.references.track(RequirementEnum::CONSTRAINTS);
    return { RequirementEnum::CONSTRAINTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementActionCosts&)
{
    context.references.track(RequirementEnum::ACTION_COSTS);
    return { RequirementEnum::ACTION_COSTS };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementNonDeterministic&)
{
    context.references.track(RequirementEnum::NON_DETERMINISTIC);
    return { RequirementEnum::NON_DETERMINISTIC };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::RequirementProbabilisticEffects&)
{
    context.references.track(RequirementEnum::PROBABILISTIC);
    return { RequirementEnum::PROBABILISTIC };
}

template<ParsingContext C>
RequirementEnumSet RequirementVisitor<C>::operator()(const ast::Requirement& node)
{
    return boost::apply_visitor(*this, node);
}

template struct RequirementVisitor<DomainParsingContext>;
template struct RequirementVisitor<ProblemParsingContext>;

}
