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

#ifndef LOKI_INCLUDE_LOKI_COMMON_AST_PRINTER_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_AST_PRINTER_HPP_

#include "ast.hpp"
#include "../../common/printer.hpp"


namespace loki {

// create string representations from ast nodes.
extern std::string parse_text(const common::ast::Word& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Name& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Variable& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Number& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::FunctionSymbol& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::FunctionTerm& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Term& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Predicate& node, const FormattingOptions& options={});
extern std::string parse_text(const common::ast::Undefined& node, const FormattingOptions& options={});

}

#endif