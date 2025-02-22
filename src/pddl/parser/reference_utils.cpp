/*
 * Copyright (C) 2023 Dominik Drexler
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

#include "reference_utils.hpp"

#include "loki/details/ast/ast.hpp"
#include "loki/details/pddl/domain_parsing_context.hpp"
#include "loki/details/pddl/exceptions.hpp"
#include "loki/details/pddl/problem_parsing_context.hpp"
#include "loki/details/pddl/scope.hpp"

namespace loki
{

template<ParsingContext C>
void track_variable_references(const ParameterList& parameter_list, C& context)
{
    for (const auto& parameter : parameter_list)
    {
        context.references.track(parameter->get_variable());
    }
}

template void track_variable_references(const ParameterList& parameter_list, DomainParsingContext& context);
template void track_variable_references(const ParameterList& parameter_list, ProblemParsingContext& context);

template<ParsingContext C>
void track_predicate_references(const PredicateList& predicate_list, C& context)
{
    for (const auto& predicate : predicate_list)
    {
        context.references.track(predicate);
    }
}

template void track_predicate_references(const PredicateList& predicate_list, DomainParsingContext& context);
template void track_predicate_references(const PredicateList& predicate_list, ProblemParsingContext& context);

template<ParsingContext C>
void track_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, C& context)
{
    for (const auto& function_skeleton : function_skeleton_list)
    {
        context.references.track(function_skeleton);
    }
}

template void track_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, DomainParsingContext& context);
template void track_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, ProblemParsingContext& context);

template<ParsingContext C>
void track_object_references(const ObjectList& object_list, C& context)
{
    for (const auto& object : object_list)
    {
        context.references.track(object);
    }
}

template void track_object_references(const ObjectList& object_list, DomainParsingContext& context);
template void track_object_references(const ObjectList& object_list, ProblemParsingContext& context);

}