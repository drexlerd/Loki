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

#include "loki/details/pddl/domain.hpp"

#include "formatter.hpp"
#include "loki/details/pddl/action.hpp"
#include "loki/details/pddl/axiom.hpp"
#include "loki/details/pddl/equal_to.hpp"
#include "loki/details/pddl/function_skeleton.hpp"
#include "loki/details/pddl/hash.hpp"
#include "loki/details/pddl/object.hpp"
#include "loki/details/pddl/predicate.hpp"
#include "loki/details/pddl/requirements.hpp"
#include "loki/details/pddl/type.hpp"
#include "loki/details/utils/collections.hpp"

#include <iostream>

using namespace std;

namespace loki
{
DomainImpl::DomainImpl(size_t index,
                       std::optional<fs::path> filepath,
                       std::string name,
                       Requirements requirements,
                       TypeList types,
                       ObjectList constants,
                       PredicateList predicates,
                       FunctionSkeletonList functions,
                       ActionList actions,
                       AxiomList axioms) :
    m_index(index),
    m_filepath(std::move(filepath)),
    m_name(std::move(name)),
    m_requirements(std::move(requirements)),
    m_types(std::move(types)),
    m_constants(std::move(constants)),
    m_predicates(std::move(predicates)),
    m_functions(std::move(functions)),
    m_actions(std::move(actions)),
    m_axioms(std::move(axioms))
{
}

size_t DomainImpl::get_index() const { return m_index; }

const std::optional<fs::path>& DomainImpl::get_filepath() const { return m_filepath; }

const std::string& DomainImpl::get_name() const { return m_name; }

const Requirements& DomainImpl::get_requirements() const { return m_requirements; }

const TypeList& DomainImpl::get_types() const { return m_types; }

const ObjectList& DomainImpl::get_constants() const { return m_constants; }

const PredicateList& DomainImpl::get_predicates() const { return m_predicates; }

const FunctionSkeletonList& DomainImpl::get_functions() const { return m_functions; }

const ActionList& DomainImpl::get_actions() const { return m_actions; }

const AxiomList& DomainImpl::get_axioms() const { return m_axioms; }

size_t UniquePDDLHasher<const DomainImpl*>::operator()(const DomainImpl* e) const
{
    return UniquePDDLHashCombiner()(e->get_name(),
                                    e->get_requirements(),
                                    get_sorted_vector(e->get_types()),
                                    get_sorted_vector(e->get_constants()),
                                    get_sorted_vector(e->get_predicates()),
                                    get_sorted_vector(e->get_functions()),
                                    get_sorted_vector(e->get_actions()),
                                    get_sorted_vector(e->get_axioms()));
}

bool UniquePDDLEqualTo<const DomainImpl*>::operator()(const DomainImpl* l, const DomainImpl* r) const
{
    if (&l != &r)
    {
        return (l->get_name() == r->get_name()) && (l->get_requirements() == r->get_requirements())
               && (get_sorted_vector(l->get_types()) == get_sorted_vector(r->get_types()))
               && (get_sorted_vector(l->get_constants()) == get_sorted_vector(r->get_constants()))
               && (get_sorted_vector(l->get_predicates()) == get_sorted_vector(r->get_predicates()))
               && (get_sorted_vector(l->get_functions()) == get_sorted_vector(r->get_functions()))
               && (get_sorted_vector(l->get_actions()) == get_sorted_vector(r->get_actions()))
               && (get_sorted_vector(l->get_axioms()) == get_sorted_vector(r->get_axioms()));
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const DomainImpl& element)
{
    auto formatter = PDDLFormatter();
    formatter.write(element, out);
    return out;
}

}
