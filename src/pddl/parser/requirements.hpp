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
#include "loki/details/ast/config.hpp"
#include "loki/details/pddl/parser.hpp"

namespace loki
{

/* Requirements */
extern RequirementEnumSet parse(const ast::Requirements& requirements_node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementStrips& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementTyping& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementNegativePreconditions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementDisjunctivePreconditions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementEquality& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementExistentialPreconditions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementUniversalPreconditions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementQuantifiedPreconditions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementConditionalEffects& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementFluents& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementObjectFluents& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementNumericFluents& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementAdl& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementDurativeActions& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementDerivedPredicates& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementTimedInitialLiterals& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementPreferences& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementConstraints& node, Context& context);
extern RequirementEnumSet parse(const ast::RequirementActionCosts& node, Context& context);
extern RequirementEnumSet parse(const ast::Requirement& node, Context& context);

struct RequirementVisitor : boost::static_visitor<RequirementEnumSet>
{
    Context& context;

    RequirementVisitor(Context& context_);

    template<typename Node>
    RequirementEnumSet operator()(const Node& node) const
    {
        return parse(node, context);
    }
};

}

#endif  // LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
