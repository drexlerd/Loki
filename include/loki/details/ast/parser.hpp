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

#ifndef LOKI_INCLUDE_LOKI_AST_PARSER_HPP_
#define LOKI_INCLUDE_LOKI_AST_PARSER_HPP_

#include "loki/details/ast/ast.hpp"

#include <boost/spirit/home/x3.hpp>

namespace loki
{
namespace x3 = boost::spirit::x3;

///////////////////////////////////////////////////////////////////////////
// parser public interface
///////////////////////////////////////////////////////////////////////////
namespace parser
{
struct DomainClass;
struct ProblemClass;

typedef x3::rule<DomainClass, ast::Domain> domain_type;
typedef x3::rule<ProblemClass, ast::Problem> problem_type;

BOOST_SPIRIT_DECLARE(domain_type)
BOOST_SPIRIT_DECLARE(problem_type)
}

parser::domain_type const& domain();
parser::problem_type const& problem();

}

#endif