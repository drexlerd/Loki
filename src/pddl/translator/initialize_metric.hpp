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
 *<
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_SRC_PDDL_TRANSLATOR_INITIALIZE_METRIC_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_INITIALIZE_METRIC_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{

/**
 * If ":metric" is undefined, introduce metric "(min (total-cost))".
 * If additionally, action_costs is disabled, we must add (increase (total-cost) 1) to each conjunctive effect.
 */
class InitializeMetricTranslator : public RecursiveCachedBaseTranslator<InitializeMetricTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<InitializeMetricTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    bool m_has_metric_defined;
    bool m_action_costs_enabled;

    Action translate_level_2(Action action, Repositories& repositories);

    Domain translate_level_2(const Domain& domain, DomainBuilder& builder);
    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};
}

#endif