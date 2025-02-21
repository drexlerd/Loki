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

#include "problem.hpp"

#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/requirements_enum.hpp"
#include "loki/details/pddl/scope.hpp"
#include "requirements.hpp"

namespace loki
{
void parse(const ast::Problem& node, ProblemParsingContext& context)
{
    auto requirements_set = RequirementEnumSet { RequirementEnum::STRIPS };
    if (node.requirements.has_value())
    {
        const auto domain_requirements_set = parse(node.requirements.value(), context);
        requirements_set.insert(domain_requirements_set.begin(), domain_requirements_set.end());
    }
    const auto requirements = context.builder.get_or_create_requirements(requirements_set);
    context.builder.get_requirements() = requirements;
    context.requirements = requirements;
}
}
