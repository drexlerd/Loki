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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PROBLEM_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PROBLEM_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{
class ProblemImpl
{
private:
    size_t m_index;
    Repositories m_repositories;
    std::optional<fs::path> m_filepath;
    Domain m_domain;
    std::string m_name;
    Requirements m_requirements;
    ObjectList m_objects;
    ObjectList m_problem_and_domain_objects;
    PredicateList m_predicates;
    PredicateList m_problem_and_domain_predicates;
    LiteralList m_initial_literals;
    FunctionValueList m_function_values;
    std::optional<Condition> m_goal_condition;
    std::optional<OptimizationMetric> m_optimization_metric;
    AxiomList m_axioms;
    AxiomList m_problem_and_domain_axioms;

    ProblemImpl(size_t index,
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
                AxiomList problem_and_domain_axioms);

    friend class ProblemBuilder;

public:
    // moveable but not copyable
    ProblemImpl(const ProblemImpl& other) = delete;
    ProblemImpl& operator=(const ProblemImpl& other) = delete;
    ProblemImpl(ProblemImpl&& other) = default;
    ProblemImpl& operator=(ProblemImpl&& other) = default;

    size_t get_index() const;
    const Repositories& get_repositories() const;
    const std::optional<fs::path>& get_filepath() const;
    const Domain& get_domain() const;
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const ObjectList& get_objects() const;
    const ObjectList& get_problem_and_domain_objects() const;
    const PredicateList& get_predicates() const;
    const PredicateList& get_problem_and_domain_predicates() const;
    const LiteralList& get_initial_literals() const;
    const FunctionValueList& get_function_values() const;
    const std::optional<Condition>& get_goal_condition() const;
    const std::optional<OptimizationMetric>& get_optimization_metric() const;
    const AxiomList& get_axioms() const;
    const AxiomList& get_problem_and_domain_axioms() const;

    auto identifying_members() const
    {
        return std::forward_as_tuple(std::as_const(m_domain),
                                     std::as_const(m_name),
                                     std::as_const(m_requirements),
                                     std::as_const(m_objects),
                                     std::as_const(m_predicates),
                                     std::as_const(m_initial_literals),
                                     std::as_const(m_function_values),
                                     std::as_const(m_goal_condition),
                                     std::as_const(m_optimization_metric),
                                     std::as_const(m_axioms));
    }
};

extern std::ostream& operator<<(std::ostream& out, const ProblemImpl& element);

extern std::ostream& operator<<(std::ostream& out, Problem element);
}

#endif
