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
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/hash.hpp"
#include "loki/details/utils/filesystem.hpp"

#include <optional>
#include <string>

namespace loki
{

class DomainImpl
{
private:
    size_t m_index;
    std::optional<fs::path> m_filepath;
    std::string m_name;
    Requirements m_requirements;
    TypeList m_types;
    ObjectList m_constants;
    PredicateList m_predicates;
    FunctionSkeletonList m_functions;
    ActionList m_actions;
    AxiomList m_axioms;

    DomainImpl(size_t index,
               std::optional<fs::path> filepath,
               std::string name,
               Requirements requirements,
               TypeList types,
               ObjectList constants,
               PredicateList predicates,
               FunctionSkeletonList functions,
               ActionList actions,
               AxiomList axioms);

    // Give access to the constructor.
    template<typename HolderType, typename Hash, typename EqualTo>
    friend class UniqueFactory;

public:
    // moveable but not copyable
    DomainImpl(const DomainImpl& other) = delete;
    DomainImpl& operator=(const DomainImpl& other) = delete;
    DomainImpl(DomainImpl&& other) = default;
    DomainImpl& operator=(DomainImpl&& other) = default;

    size_t get_index() const;
    const std::optional<fs::path>& get_filepath() const;
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const TypeList& get_types() const;
    const ObjectList& get_constants() const;
    const PredicateList& get_predicates() const;
    const FunctionSkeletonList& get_functions() const;
    const ActionList& get_actions() const;
    const AxiomList& get_axioms() const;
};

template<>
struct UniquePDDLHasher<const DomainImpl*>
{
    size_t operator()(const DomainImpl* e) const;
};

template<>
struct UniquePDDLEqualTo<const DomainImpl*>
{
    bool operator()(const DomainImpl* l, const DomainImpl* r) const;
};

extern std::ostream& operator<<(std::ostream& out, const DomainImpl& element);

}

#endif
