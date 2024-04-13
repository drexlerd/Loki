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

#include "loki/pddl/exceptions.hpp"

namespace loki
{

void track_variable_references(const ParameterList& parameter_list, Context& context)
{
    for (const auto& parameter : parameter_list)
    {
        context.references.track(parameter->get_variable());
    }
}

void test_variable_references(const ParameterList& parameter_list, const Context& context)
{
    for (const auto& parameter : parameter_list)
    {
        if (context.references.exists(parameter->get_variable()))
        {
            const auto [variable, position, error_handler] = context.scopes.get<VariableImpl>(parameter->get_variable()->get_name()).value();
            throw UnusedVariableError(variable->get_name(), error_handler(position.value(), ""));
        }
    }
}

void track_predicate_references(const PredicateList& predicate_list, Context& context)
{
    for (const auto& predicate : predicate_list)
    {
        context.references.track(predicate);
    }
}

void test_predicate_references(const PredicateList& predicate_list, const Context& context)
{
    for (const auto& predicate : predicate_list)
    {
        if (context.references.exists(predicate))
        {
            const auto [_predicate, position, error_handler] = context.scopes.get<PredicateImpl>(predicate->get_name()).value();
            throw UnusedPredicateError(predicate->get_name(), error_handler(position.value(), ""));
        }
    }
}

void track_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, Context& context)
{
    for (const auto& function_skeleton : function_skeleton_list)
    {
        context.references.track(function_skeleton);
    }
}

void test_function_skeleton_references(const FunctionSkeletonList& function_skeleton_list, const Context& context)
{
    for (const auto& function_skeleton : function_skeleton_list)
    {
        if (context.references.exists(function_skeleton))
        {
            const auto [_function_skeleton, position, error_handler] = context.scopes.get<FunctionSkeletonImpl>(function_skeleton->get_name()).value();
            throw UnusedFunctionSkeletonError(function_skeleton->get_name(), error_handler(position.value(), ""));
        }
    }
}

void track_object_references(const ObjectList& object_list, Context& context)
{
    for (const auto& object : object_list)
    {
        context.references.track(object);
    }
}

void test_object_references(const ObjectList& object_list, const Context& context)
{
    for (const auto& object : object_list)
    {
        if (context.references.exists(object))
        {
            const auto [_object, position, error_handler] = context.scopes.get<ObjectImpl>(object->get_name()).value();
            throw UnusedObjectError(object->get_name(), error_handler(position.value(), ""));
        }
    }
}

}