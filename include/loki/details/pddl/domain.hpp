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

#ifndef LOKI_INCLUDE_LOKI_PDDL_DOMAIN_HPP_
#define LOKI_INCLUDE_LOKI_PDDL_DOMAIN_HPP_

#include "loki/details/pddl/declarations.hpp"
#include "loki/details/pddl/repositories.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class DomainImpl
{
private:
    Repositories m_repositories;
    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    LiteralList m_static_initial_literals;  ///< May contain literals that represent the type of constants after type translation.
    PredicateList m_predicates;
    FunctionSkeletonList m_function_skeletons;
    ActionList m_actions;
    AxiomList m_axioms;

    DomainImpl(Repositories repositories,
               std::optional<fs::path> filepath,
               std::string name,
               Requirements requirements,
               TypeList types,
               ObjectList constants,
               LiteralList static_initial_literals,
               PredicateList predicates,
               FunctionSkeletonList function_skeletons,
               ActionList actions,
               AxiomList axioms);

    friend class DomainBuilder;

public:
    // moveable but not copyable
    DomainImpl(const DomainImpl& other) = delete;
    DomainImpl& operator=(const DomainImpl& other) = delete;
    DomainImpl(DomainImpl&& other) = default;
    DomainImpl& operator=(DomainImpl&& other) = default;

    const Repositories& get_repositories() const;
    const std::optional<fs::path>& get_filepath() const;
    const std::string& get_name() const;
    Requirements get_requirements() const;
    const TypeList& get_types() const;
    const ObjectList& get_constants() const;
    const LiteralList& get_static_initial_literals() const;
    const PredicateList& get_predicates() const;
    const FunctionSkeletonList& get_function_skeletons() const;
    const ActionList& get_actions() const;
    const AxiomList& get_axioms() const;

    auto identifying_members() const
    {
        return std::tuple(std::cref(get_name()),
                          get_requirements(),
                          std::cref(get_types()),
                          std::cref(get_constants()),
                          std::cref(get_predicates()),
                          std::cref(get_function_skeletons()),
                          std::cref(get_actions()),
                          std::cref(get_axioms()));
    }
};

extern std::ostream& operator<<(std::ostream& out, const DomainImpl& element);

extern std::ostream& operator<<(std::ostream& out, Domain element);
}

#endif
