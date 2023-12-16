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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_AST_STAGE_1_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_AST_STAGE_1_PRINTER_HPP_

#include "ast.hpp"
#include "../../../common/printer.hpp"


namespace loki::domain::stage_1 {

extern std::string parse_text(const ast::TextOrNode& node, const FormattingOptions& options={});
extern std::string parse_text(const ast::Node& node, const FormattingOptions& options={});

}

#endif
