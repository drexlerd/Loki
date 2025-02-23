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

#include "initialize_equality.hpp"

#include "loki/details/utils/collections.hpp"

namespace loki
{

static Literal get_or_create_equality_literal(Predicate equality_predicate, Object object, Repositories& repositories)
{
    const auto term_object = repositories.get_or_create_term(object);

    return repositories.get_or_create_literal(false, repositories.get_or_create_atom(equality_predicate, TermList { term_object, term_object }));
}

Problem InitializeEqualityTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    for (const auto& predicate : problem->get_domain()->get_predicates())
    {
        if (predicate->get_name() == "=")
        {
            m_equality_predicate = predicate;
            break;
        }
    }

    if (problem->get_requirements()->test(RequirementEnum::EQUALITY) && !m_equality_predicate)
    {
        throw std::runtime_error(
            "InitializeEqualityTranslator::translate_level_2(problem, builder): Expected equality predicate to be declared in the domain.");
    }

    auto& repositories = builder.get_repositories();

    const auto translated_objects = this->translate_level_0(problem->get_objects(), repositories);

    auto translated_initial_literals = this->translate_level_0(problem->get_initial_literals(), repositories);

    if (problem->get_requirements()->test(RequirementEnum::EQUALITY))
    {
        for (const auto& constant : problem->get_domain()->get_constants())
        {
            translated_initial_literals.push_back(get_or_create_equality_literal(m_equality_predicate, constant, repositories));
        }
        for (const auto& object : translated_objects)
        {
            translated_initial_literals.push_back(get_or_create_equality_literal(m_equality_predicate, object, repositories));
        }
    }

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();
    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);

    builder.get_objects().insert(builder.get_objects().end(), translated_objects.begin(), translated_objects.end());
    const auto translated_predicates = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_predicates().insert(builder.get_predicates().end(), translated_predicates.begin(), translated_predicates.end());

    builder.get_initial_literals().insert(builder.get_initial_literals().end(), translated_initial_literals.begin(), translated_initial_literals.end());
    const auto translated_initial_function_values = this->translate_level_0(problem->get_initial_function_values(), repositories);
    builder.get_initial_function_values().insert(builder.get_initial_function_values().end(),
                                                 translated_initial_function_values.begin(),
                                                 translated_initial_function_values.end());
    if (problem->get_goal_condition().has_value())
        builder.get_goal_condition() = this->translate_level_0(problem->get_goal_condition().value(), repositories);
    if (problem->get_optimization_metric().has_value())
        builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric().value(), repositories);
    const auto translated_axioms = this->translate_level_0(problem->get_axioms(), repositories);
    builder.get_axioms().insert(builder.get_axioms().end(), translated_axioms.begin(), translated_axioms.end());

    return builder.get_result(problem->get_index());
}

}