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

#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, NumberTest) {
    ast::Number ast;

    EXPECT_NO_THROW(parse_ast("5", number(), ast));
    EXPECT_EQ(parse_text(ast), "5");
    EXPECT_NO_THROW(parse_ast("4.2", number(), ast));
    EXPECT_EQ(parse_text(ast), "4.2");
    EXPECT_NO_THROW(parse_ast("6 7", number(), ast));
    EXPECT_EQ(parse_text(ast), "6");
    // TODO: Is this really what we want?
    EXPECT_NO_THROW(parse_ast("1loki", number(), ast));
    EXPECT_EQ(parse_text(ast), "1");

    EXPECT_ANY_THROW(parse_ast("loki", number(), ast));
    EXPECT_ANY_THROW(parse_ast("(5)", number(), ast));
}

}
