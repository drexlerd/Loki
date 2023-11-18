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

#include <gtest/gtest.h>

#include "../../src/domain/ast/parser.hpp"
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TermTest) {
    ast::Term ast;

    EXPECT_NO_THROW(parse_ast("?loki", term(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("loki", term(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("?loki(?kilo)", term(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("loki(kilo)", term(), ast));
    EXPECT_EQ(parse_text(ast), "loki");

    EXPECT_ANY_THROW(parse_ast("1loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", term(), ast));
}

}
