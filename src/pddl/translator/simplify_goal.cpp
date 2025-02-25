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

#include "simplify_goal.hpp"

#include "loki/details/pddl/problem_builder.hpp"
#include "loki/details/utils/collections.hpp"
#include "utils.hpp"

using namespace std::string_literals;

namespace loki
{

Condition SimplifyGoalTranslator::simplify_goal_condition(Condition goal_condition,
                                                          Repositories& repositories,
                                                          PredicateList& instantiated_predicates,
                                                          AxiomList& instantiated_axioms,
                                                          size_t& next_axiom_index,
                                                          std::unordered_set<std::string>& problem_and_domain_predicate_names)
{
    if (std::get_if<ConditionLiteral>(&goal_condition->get_condition()))
    {
        return this->translate_level_0(goal_condition, repositories);
    }
    else if (std::get_if<ConditionNumericConstraint>(&goal_condition->get_condition()))
    {
        return this->translate_level_0(goal_condition, repositories);
    }
    else if (const auto condition_and = std::get_if<ConditionAnd>(&goal_condition->get_condition()))
    {
        auto parts = ConditionList {};
        parts.reserve((*condition_and)->get_conditions().size());
        for (const auto& part : (*condition_and)->get_conditions())
        {
            parts.push_back(simplify_goal_condition(part,
                                                    repositories,
                                                    instantiated_predicates,
                                                    instantiated_axioms,
                                                    next_axiom_index,
                                                    problem_and_domain_predicate_names));
        }
        return repositories.get_or_create_condition(repositories.get_or_create_condition_and(parts));
    }

    const auto axiom_name = create_unique_axiom_name(next_axiom_index, problem_and_domain_predicate_names);
    const auto predicate = repositories.get_or_create_predicate(axiom_name, ParameterList {});
    instantiated_predicates.push_back(predicate);
    const auto atom = repositories.get_or_create_atom(predicate, TermList {});
    const auto literal = repositories.get_or_create_literal(false, atom);
    const auto substituted_condition = repositories.get_or_create_condition(repositories.get_or_create_condition_literal(literal));
    const auto axiom = repositories.get_or_create_axiom(ParameterList {}, literal, this->translate_level_0(goal_condition, repositories));
    instantiated_axioms.push_back(axiom);

    return substituted_condition;
}

Problem SimplifyGoalTranslator::translate_level_2(const Problem& problem, ProblemBuilder& builder)
{
    auto problem_and_domain_predicate_names = std::unordered_set<std::string> {};
    for (const auto& predicate : problem->get_problem_and_domain_predicates())
    {
        problem_and_domain_predicate_names.insert(predicate->get_name());
    }

    auto& repositories = builder.get_repositories();

    builder.get_filepath() = problem->get_filepath();
    builder.get_name() = problem->get_name();
    builder.get_requirements() = this->translate_level_0(problem->get_requirements(), repositories);
    builder.get_objects() = this->translate_level_0(problem->get_objects(), repositories);
    builder.get_predicates() = this->translate_level_0(problem->get_predicates(), repositories);
    builder.get_initial_literals() = this->translate_level_0(problem->get_initial_literals(), repositories);
    builder.get_initial_function_values() = this->translate_level_0(problem->get_initial_function_values(), repositories);
    builder.get_optimization_metric() = this->translate_level_0(problem->get_optimization_metric(), repositories);
    builder.get_axioms() = this->translate_level_0(problem->get_axioms(), repositories);

    if (problem->get_goal_condition().has_value())
    {
        auto next_axiom_index = size_t(0);
        auto instantiated_axioms = AxiomList {};
        auto instantiated_predicates = PredicateList {};

        builder.get_goal_condition() = simplify_goal_condition(problem->get_goal_condition().value(),
                                                               repositories,
                                                               instantiated_predicates,
                                                               instantiated_axioms,
                                                               next_axiom_index,
                                                               problem_and_domain_predicate_names);
        builder.get_predicates().insert(builder.get_predicates().end(), instantiated_predicates.begin(), instantiated_predicates.end());
        builder.get_axioms().insert(builder.get_axioms().end(), instantiated_axioms.begin(), instantiated_axioms.end());
    }

    return builder.get_result(problem->get_index());
}

}
