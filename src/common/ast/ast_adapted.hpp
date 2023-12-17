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

#ifndef LOKI_SRC_DOMAIN_AST_AST_ADAPTED_HPP_
#define LOKI_SRC_DOMAIN_AST_AST_ADAPTED_HPP_

#include "../../../include/loki/common/ast/ast.hpp"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

// We need to tell fusion about our rexpr and rexpr_key_value
// to make them a first-class fusion citizens
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::Name, alpha, suffix)
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::Variable, question_mark, name)
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::FunctionSymbol, name)
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::FunctionTerm, function_symbol, terms)
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::Predicate, name)
BOOST_FUSION_ADAPT_STRUCT(loki::common::ast::Number, value)


#endif
