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

#ifndef LOKI_SEMANTIC_PARSER_RUNTIME_HPP_
#define LOKI_SEMANTIC_PARSER_RUNTIME_HPP_

#include "loki/ast/ast_fwd.hpp"
#include "loki/parser/diagnostic.hpp"

namespace loki::parser::detail
{

bool parse_domain_full(Iterator& first, Iterator last, ast::Domain& out, ErrorHandlerType& error_handler);
bool parse_task_full(Iterator& first, Iterator last, ast::Task& out, ErrorHandlerType& error_handler);

}  // namespace loki::parser::detail

#endif
