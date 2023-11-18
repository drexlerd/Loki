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

pddl::RequirementEnumSet parse(const ast::RequirementStrips&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::STRIPS };
}

pddl::RequirementEnumSet parse(const ast::RequirementTyping&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::TYPING };
}

pddl::RequirementEnumSet parse(const ast::RequirementNegativePreconditions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::NEGATIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDisjunctivePreconditions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementEquality&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::EQUALITY };
}

pddl::RequirementEnumSet parse(const ast::RequirementExistentialPreconditions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementUniversalPreconditions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementQuantifiedPreconditions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::EXISTENTIAL_PRECONDITIONS,
             pddl::RequirementEnum::UNIVERSAL_PRECONDITIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementConditionalEffects&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementFluents&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::FLUENTS,
             pddl::RequirementEnum::OBJECT_FLUENTS,
             pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementObjectFluents&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::OBJECT_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementNumericFluents&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::NUMERIC_FLUENTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementAdl&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::ADL,
             pddl::RequirementEnum::STRIPS,
             pddl::RequirementEnum::TYPING,
             pddl::RequirementEnum::DISJUNCTIVE_PRECONDITIONS,
             pddl::RequirementEnum::EQUALITY,
             pddl::RequirementEnum::QUANTIFIED_PRECONDITIONS,
             pddl::RequirementEnum::CONDITIONAL_EFFECTS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDurativeActions&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementDerivedPredicates&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::DERIVED_PREDICATES };
}

pddl::RequirementEnumSet parse(const ast::RequirementTimedInitialLiterals&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::TIMED_INITIAL_LITERALS,
             pddl::RequirementEnum::DURATIVE_ACTIONS };
}

pddl::RequirementEnumSet parse(const ast::RequirementPreferences&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::PREFERENCES };
}

pddl::RequirementEnumSet parse(const ast::RequirementConstraints&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::CONSTRAINTS };

}

pddl::RequirementEnumSet parse(const ast::RequirementActionCosts&, const error_handler_type&, Context&) {
    return { pddl::RequirementEnum::ACTION_COSTS };
}

pddl::RequirementEnumSet parse(const ast::Requirement& node, const error_handler_type& error_handler, Context& context) {
    return boost::apply_visitor(RequirementVisitor(error_handler, context), node);
}

RequirementVisitor::RequirementVisitor(const error_handler_type& error_handler_, Context& context_)
    : error_handler(error_handler_), context(context_) { }

pddl::Requirements parse(const ast::Requirements& requirements_node, const error_handler_type& error_handler, Context& context) {
    pddl::RequirementEnumSet requirements;
    for (const auto& requirement : requirements_node.requirements) {
        auto additional_requirements = parse(requirement, error_handler, context);
        requirements.insert(additional_requirements.begin(), additional_requirements.end());
    }
    return context.requirements->get_or_create(requirements).object;
}

}
