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

#ifndef LOKI_SRC_PDDL_PARSER_REQUIREMENTS_HPP_
#define LOKI_SRC_PDDL_PARSER_REQUIREMENTS_HPP_

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/concepts.hpp"
#include "loki/details/pddl/reference.hpp"

namespace loki
{

template<ParsingContext C>
extern RequirementEnumSet parse(const ast::Requirements& requirements_node, C& context);

template<ParsingContext C>
struct RequirementVisitor : boost::static_visitor<RequirementEnumSet>
{
    C& context;

    RequirementVisitor(C& context);

    RequirementEnumSet operator()(const ast::RequirementStrips& node);
    RequirementEnumSet operator()(const ast::RequirementTyping& node);
    RequirementEnumSet operator()(const ast::RequirementNegativePreconditions& node);
    RequirementEnumSet operator()(const ast::RequirementDisjunctivePreconditions& node);
    RequirementEnumSet operator()(const ast::RequirementEquality& node);
    RequirementEnumSet operator()(const ast::RequirementExistentialPreconditions& node);
    RequirementEnumSet operator()(const ast::RequirementUniversalPreconditions& node);
    RequirementEnumSet operator()(const ast::RequirementQuantifiedPreconditions& node);
    RequirementEnumSet operator()(const ast::RequirementConditionalEffects& node);
    RequirementEnumSet operator()(const ast::RequirementFluents& node);
    RequirementEnumSet operator()(const ast::RequirementObjectFluents& node);
    RequirementEnumSet operator()(const ast::RequirementFunctionValues& node);
    RequirementEnumSet operator()(const ast::RequirementAdl& node);
    RequirementEnumSet operator()(const ast::RequirementDurativeActions& node);
    RequirementEnumSet operator()(const ast::RequirementDerivedPredicates& node);
    RequirementEnumSet operator()(const ast::RequirementTimedInitialLiterals& node);
    RequirementEnumSet operator()(const ast::RequirementPreferences& node);
    RequirementEnumSet operator()(const ast::RequirementConstraints& node);
    RequirementEnumSet operator()(const ast::RequirementActionCosts& node);
    RequirementEnumSet operator()(const ast::RequirementNonDeterministic& node);
    RequirementEnumSet operator()(const ast::RequirementProbabilisticEffects& node);
    RequirementEnumSet operator()(const ast::Requirement& node);
};

}

#endif
