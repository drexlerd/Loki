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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DOMAIN_BUILDER_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DOMAIN_BUILDER_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class DomainBuilder
{
private:
    Repositories m_repositories;

    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    LiteralList m_static_initial_literals;
    PredicateList m_predicates;
    FunctionSkeletonList m_function_skeletons;
    ActionList m_actions;
    AxiomList m_axioms;

public:
    /// @brief Create an empty builder.
    DomainBuilder() = default;

    /// @brief Finalizes the `Domain` and returns it.
    /// The `DomainBuilder` is in an invalid state afterwards.
    /// @return the resulting `Domain`.
    Domain get_result();

    /**
     * Get and modify components of the domain.
     */

    Repositories& get_repositories();
    std::optional<fs::path>& get_filepath();
    std::string& get_name();
    Requirements& get_requirements();
    TypeList& get_types();
    ObjectList& get_constants();
    LiteralList& get_static_initial_literals();
    PredicateList& get_predicates();
    FunctionSkeletonList& get_function_skeletons();
    ActionList& get_actions();
    AxiomList& get_axioms();
};
}

#endif
