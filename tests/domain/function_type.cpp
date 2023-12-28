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

#include <gtest/gtest.h>

#include "../../src/domain/ast/parser.hpp"
#include "../../include/common/ast/parser_wrapper.hpp"
#include "../../include/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, FunctionTypeTest) {
    ast::FunctionType ast;

    // Number (int)
    EXPECT_NO_THROW(parse_ast("5", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "5");
    EXPECT_NO_THROW(parse_ast("6 7", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "6");
    // TODO: Is this really what we want?
    EXPECT_NO_THROW(parse_ast("1loki", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "1");

    // Number (float)
    EXPECT_NO_THROW(parse_ast("4.2", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "4.2");

    // Type
    EXPECT_NO_THROW(parse_ast("loki", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("(either type1 type2)", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "(either type1 type2)");

    EXPECT_ANY_THROW(parse_ast("(5)", function_type(), ast));
}

}
