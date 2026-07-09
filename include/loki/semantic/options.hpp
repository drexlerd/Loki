/*
 * Copyright (C) 2024-2026 Dominik Drexler
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

#ifndef LOKI_SEMANTIC_OPTIONS_HPP_
#define LOKI_SEMANTIC_OPTIONS_HPP_

namespace loki::semantic
{

struct ParserOptions
{
    bool strict = false;
    // Complete missing :action-costs artifacts instead of erroring; if the domain does not
    // declare :action-costs, injects the requirement, total-cost, and unit-cost effects.
    bool add_action_costs = true;
};

struct TranslatorOptions
{
    bool compile_typing = true;
    bool compile_conditional_effects = false;
    // Add the = predicate and (= o o) initial literals; turn off for consumers with native equality.
    bool materialize_equality = true;
};

}  // namespace loki::semantic

#endif
