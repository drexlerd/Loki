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

#ifndef LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
#define LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_

#include "../../../../include/loki/common/ast/config.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/pddl/parser.hpp"

using namespace loki::domain;

namespace loki {

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementStrips& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementTyping& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementNegativePreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDisjunctivePreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementEquality& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementExistentialPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementUniversalPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementQuantifiedPreconditions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementConditionalEffects& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementObjectFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementNumericFluents& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementAdl& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDurativeActions& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementDerivedPredicates& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementTimedInitialLiterals& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementPreferences& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementConstraints& node);

extern pddl::RequirementEnumSet parse(const domain::ast::RequirementActionCosts& node);

extern pddl::RequirementEnumSet parse(const domain::ast::Requirement& node);

struct RequirementVisitor : boost::static_visitor<pddl::RequirementEnumSet> {
    template<typename Node>
    pddl::RequirementEnumSet operator()(const Node& node) const {
        return parse(node);
    }
};

extern pddl::Requirements parse(const domain::ast::Requirements& requirements_node, Context& context);

}

#endif // LOKI_SRC_DOMAIN_PDDL_PARSER_REQUIREMENTS_HPP_
