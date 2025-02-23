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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_INITIALIZE_EQUALITY_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_INITIALIZE_EQUALITY_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{

/**
 * If ":equality" is defined, introduce predicate "=" and ground atoms "(= o o)" to the initial literals for each o in O.
 */
class InitializeEqualityTranslator : public RecursiveCachedBaseTranslator<InitializeEqualityTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<InitializeEqualityTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    Predicate m_equality_predicate = nullptr;

    // We dont have to translate the domain because the parser already added the equality predicate to the predicates section.

    Problem translate_level_2(const Problem& problem, ProblemBuilder& builder);
};
}

#endif