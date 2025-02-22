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

#include "../../../src/ast/parser.hpp"

#include <gtest/gtest.h>
#include <loki/details/ast/ast.hpp>
#include <loki/details/ast/parser_wrapper.hpp>
#include <loki/details/ast/printer.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, LokiPddlAstVariableTest)
{
    ast::Variable ast;

    EXPECT_NO_THROW(parse_ast("?loki", variable(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("?loki ?kilo", variable(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("?loki(?kilo)", variable(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");

    EXPECT_ANY_THROW(parse_ast("loki", variable(), ast));
    EXPECT_ANY_THROW(parse_ast("1loki", variable(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", variable(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", variable(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", variable(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", variable(), ast));
}

}
