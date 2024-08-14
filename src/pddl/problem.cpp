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
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"
#include "loki/details/pddl/literal.hpp"
#include "loki/details/pddl/metric.hpp"
#include "loki/details/pddl/numeric_fluent.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/utils/collections.hpp"

#include <iostream>

using namespace std;

namespace loki
{
ProblemImpl::ProblemImpl(size_t index,
                         std::optional<fs::path> filepath,
                         Domain domain,
                         std::string name,
                         Requirements requirements,
                         ObjectList objects,
                         PredicateList derived_predicates,
                         LiteralList initial_literals,
                         NumericFluentList numeric_fluents,
                         std::optional<Condition> goal_condition,
                         std::optional<OptimizationMetric> optimization_metric,
                         AxiomList axioms) :
    m_index(index),
    m_filepath(std::move(filepath)),
    m_domain(std::move(domain)),
    m_name(std::move(name)),
    m_requirements(std::move(requirements)),
    m_objects(std::move(objects)),
    m_derived_predicates(std::move(derived_predicates)),
    m_initial_literals(std::move(initial_literals)),
    m_numeric_fluents(std::move(numeric_fluents)),
    m_goal_condition(std::move(goal_condition)),
    m_optimization_metric(std::move(optimization_metric)),
    m_axioms(std::move(axioms))
{
}

size_t ProblemImpl::get_index() const { return m_index; }

const std::optional<fs::path>& ProblemImpl::get_filepath() const { return m_filepath; }

const Domain& ProblemImpl::get_domain() const { return m_domain; }

const std::string& ProblemImpl::get_name() const { return m_name; }

const Requirements& ProblemImpl::get_requirements() const { return m_requirements; }

const ObjectList& ProblemImpl::get_objects() const { return m_objects; }

const PredicateList& ProblemImpl::get_derived_predicates() const { return m_derived_predicates; }

const LiteralList& ProblemImpl::get_initial_literals() const { return m_initial_literals; }

const NumericFluentList& ProblemImpl::get_numeric_fluents() const { return m_numeric_fluents; }

const std::optional<Condition>& ProblemImpl::get_goal_condition() const { return m_goal_condition; }

const std::optional<OptimizationMetric>& ProblemImpl::get_optimization_metric() const { return m_optimization_metric; }

const AxiomList& ProblemImpl::get_axioms() const { return m_axioms; }

size_t UniquePDDLHasher<const ProblemImpl*>::operator()(const ProblemImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(),
                                    e->get_domain(),
                                    get_sorted_vector(e->get_objects()),
                                    get_sorted_vector(e->get_derived_predicates()),
                                    get_sorted_vector(e->get_initial_literals()),
                                    get_sorted_vector(e->get_numeric_fluents()),
                                    e->get_goal_condition(),
                                    e->get_optimization_metric());
}

bool UniquePDDLEqualTo<const ProblemImpl*>::operator()(const ProblemImpl* l, const ProblemImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_domain() == r->get_domain())
               && (get_sorted_vector(l->get_objects()) == get_sorted_vector(r->get_objects()))
               && (get_sorted_vector(l->get_derived_predicates()) == get_sorted_vector(r->get_derived_predicates()))
               && (get_sorted_vector(l->get_initial_literals()) == get_sorted_vector(r->get_initial_literals()))
               && (get_sorted_vector(l->get_numeric_fluents()) == get_sorted_vector(r->get_numeric_fluents()))
               && (l->get_goal_condition() == r->get_goal_condition()) && (l->get_optimization_metric() == r->get_optimization_metric());
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const ProblemImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
