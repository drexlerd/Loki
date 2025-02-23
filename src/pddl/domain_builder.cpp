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

#include "loki/details/pddl/domain_builder.hpp"

#include "loki/details/pddl/domain.hpp"
#include "loki/details/utils/collections.hpp"

namespace loki
{
template<typename T>
static void verify_indexing_scheme(const std::vector<const T*>& elements, const std::string& error_message)
{
    for (size_t i = 0; i < elements.size(); ++i)
    {
        if (elements[i]->get_index() != i)
        {
            throw std::runtime_error(error_message);
        }
    }
}

Domain DomainBuilder::get_result()
{
    m_types = uniquify_elements(m_types);  ///< We explicitly add "object" and "number" which may result in duplicates, so we filter them out.
    std::sort(m_types.begin(), m_types.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_types, "DomainBuilder::get_result: types must follow and indexing scheme");

    std::sort(m_constants.begin(), m_constants.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_constants, "DomainBuilder::get_result: constants must follow and indexing scheme.");

    std::sort(m_predicates.begin(), m_predicates.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_predicates, "DomainBuilder::get_result: predicates must follow and indexing scheme.");

    std::sort(m_function_skeletons.begin(), m_function_skeletons.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_function_skeletons, "DomainBuilder::get_result: functions must follow and indexing scheme.");

    std::sort(m_actions.begin(), m_actions.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_actions, "DomainBuilder::get_result: actions must follow and indexing scheme.");

    std::sort(m_axioms.begin(), m_axioms.end(), [](auto&& lhs, auto&& rhs) { return lhs->get_index() < rhs->get_index(); });
    verify_indexing_scheme(m_axioms, "DomainBuilder::get_result: axioms must follow and indexing scheme.");

    m_requirements = (m_requirements) ? m_requirements : m_repositories.get_or_create_requirements(RequirementEnumSet { RequirementEnum::STRIPS });

    return std::shared_ptr<const DomainImpl>(new DomainImpl(std::move(m_repositories),
                                                            std::move(m_filepath),
                                                            std::move(m_name),
                                                            std::move(m_requirements),
                                                            std::move(m_types),
                                                            std::move(m_constants),
                                                            std::move(m_predicates),
                                                            std::move(m_function_skeletons),
                                                            std::move(m_actions),
                                                            std::move(m_axioms)));
}

Repositories& DomainBuilder::get_repositories() { return m_repositories; }
std::optional<fs::path>& DomainBuilder::get_filepath() { return m_filepath; }
std::string& DomainBuilder::get_name() { return m_name; }
Requirements& DomainBuilder::get_requirements() { return m_requirements; }
TypeList& DomainBuilder::get_types() { return m_types; }
ObjectList& DomainBuilder::get_constants() { return m_constants; }
PredicateList& DomainBuilder::get_predicates() { return m_predicates; }
FunctionSkeletonList& DomainBuilder::get_function_skeletons() { return m_function_skeletons; }
ActionList& DomainBuilder::get_actions() { return m_actions; }
AxiomList& DomainBuilder::get_axioms() { return m_axioms; }
}
