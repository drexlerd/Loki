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

#ifndef LOKI_SRC_PDDL_TRANSLATOR_REMOVE_TYPING_HPP_
#define LOKI_SRC_PDDL_TRANSLATOR_REMOVE_TYPING_HPP_

#include "recursive_cached_base.hpp"

namespace loki
{
/**
 * Compile away types.
 */
class RemoveTypingTranslator : public RecursiveCachedBaseTranslator<RemoveTypingTranslator>
{
private:
    /* Implement RecursiveCachedBaseTranslator interface. */
    friend class RecursiveCachedBaseTranslator<RemoveTypingTranslator>;

    // Provide default implementations
    using RecursiveCachedBaseTranslator::translate_level_2;

    /**
     * Translate
     */

    Requirements translate_level_2(Requirements requirements, Repositories& repositories);
    Object translate_level_2(Object object, Repositories& repositories);
    Parameter translate_level_2(Parameter parameter, Repositories& repositories);
    Domain translate_level_2(const Domain& domain, DomainBuilder& builder);
};

}
#endif
