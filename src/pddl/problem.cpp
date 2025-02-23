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

#include "loki/details/pddl/problem.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/conditions.hpp"
#include "loki/details/pddl/domain.hpp"
#include "loki/details/pddl/function_value.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/type.hpp"

#include <iostream>

using namespace std;

namespace loki
{
ProblemImpl::ProblemImpl(size_t index,
                         Repositories repositories,
                         std::optional<fs::path> filepath,
                         Domain domain,
                         std::string name,
                         Requirements requirements,
                         ObjectList objects,
                         ObjectList problem_and_domain_objects,
                         PredicateList predicates,
                         PredicateList problem_and_domain_predicates,
                         LiteralList initial_literals,
                         FunctionValueList function_values,
                         std::optional<Condition> goal_condition,
                         std::optional<OptimizationMetric> optimization_metric,
                         AxiomList axioms,
                         AxiomList problem_and_domain_axioms) :
    m_index(index),
    m_repositories(std::move(repositories)),
    m_filepath(std::move(filepath)),
    m_domain(std::move(domain)),
    m_name(std::move(name)),
    m_requirements(std::move(requirements)),
    m_objects(std::move(objects)),
    m_problem_and_domain_objects(std::move(problem_and_domain_objects)),
    m_predicates(std::move(predicates)),
    m_problem_and_domain_predicates(std::move(problem_and_domain_predicates)),
    m_initial_literals(std::move(initial_literals)),
    m_function_values(std::move(function_values)),
    m_goal_condition(std::move(goal_condition)),
    m_optimization_metric(std::move(optimization_metric)),
    m_axioms(std::move(axioms)),
    m_problem_and_domain_axioms(std::move(problem_and_domain_axioms))
{
}

size_t ProblemImpl::get_index() const { return m_index; }

const Repositories& ProblemImpl::get_repositories() const { return m_repositories; }

const std::optional<fs::path>& ProblemImpl::get_filepath() const { return m_filepath; }

const Domain& ProblemImpl::get_domain() const { return m_domain; }

const std::string& ProblemImpl::get_name() const { return m_name; }

const Requirements& ProblemImpl::get_requirements() const { return m_requirements; }

const ObjectList& ProblemImpl::get_objects() const { return m_objects; }

const ObjectList& ProblemImpl::get_problem_and_domain_objects() const { return m_problem_and_domain_objects; }

const PredicateList& ProblemImpl::get_predicates() const { return m_predicates; }

const PredicateList& ProblemImpl::get_problem_and_domain_predicates() const { return m_problem_and_domain_predicates; }

const LiteralList& ProblemImpl::get_initial_literals() const { return m_initial_literals; }

const FunctionValueList& ProblemImpl::get_function_values() const { return m_function_values; }

const std::optional<Condition>& ProblemImpl::get_goal_condition() const { return m_goal_condition; }

const std::optional<OptimizationMetric>& ProblemImpl::get_optimization_metric() const { return m_optimization_metric; }

const AxiomList& ProblemImpl::get_axioms() const { return m_axioms; }

const AxiomList& ProblemImpl::get_problem_and_domain_axioms() const { return m_problem_and_domain_axioms; }

std::ostream& operator<<(std::ostream& out, const ProblemImpl& element)
{
    write<StringTag>(element, out);
    return out;
}

std::ostream& operator<<(std::ostream& out, Problem element)
{
    write<AddressTag>(*element, out);
    return out;
}

}
