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

#ifndef LOKI_INCLUDE_LOKI_PDDL_PROBLEM_BUILDER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_PROBLEM_BUILDER_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class ProblemBuilder
{
private:
    Repositories m_repositories;

    Domain m_domain;  ///< Immutable planning domain
    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    ObjectList m_objects;
    PredicateList m_predicates;  ///< Translation might introduce additional derived predicates.
    LiteralList m_initial_literals;
    FunctionValueList m_initial_function_values;
    std::optional<Condition> m_goal_condition;
    std::optional<OptimizationMetric> m_optimization_metric;
    AxiomList m_axioms;  ///< Translation might introduce axioms.

public:
    /// @brief Create a `ProblemBuilder` for a given `Domain`.
    /// Copies parts elements from the `Domain` into the `HanaRepositories` and problem specific information to ensure indexing schemas.
    /// @param domain
    explicit ProblemBuilder(Domain domain);

    /// @brief Finalizes the `Problem` and returns it.
    /// The `ProblemBuilder` is in an invalid state afterwards.
    /// @return the resulting `Problem`.
    Problem get_result(size_t problem_index);

    /**
     * Get and modify components of the problem.
     */

    Repositories& get_repositories();
    const Domain& get_domain() const;  ///< Modifying domain is prohibitted!
    std::optional<fs::path>& get_filepath();
    std::string& get_name();
    Requirements& get_requirements();
    ObjectList& get_objects();
    PredicateList& get_predicates();
    LiteralList& get_initial_literals();
    FunctionValueList& get_function_values();
    std::optional<Condition>& get_goal_condition();
    std::optional<OptimizationMetric>& get_optimization_metric();
    AxiomList& get_axioms();
};

using ProblemBuilderList = std::vector<ProblemBuilder>;
}

#endif