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
#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, AtomicFunctionSkeletonTest) {
    ast::AtomicFunctionSkeleton ast;

    EXPECT_NO_THROW(parse_ast("(function-symbol1 ?var1 ?var2)", atomic_function_skeleton(), ast));
    EXPECT_EQ(parse_text(ast), "(function-symbol1 ?var1 ?var2)");

    EXPECT_NO_THROW(parse_ast("(function-symbol1 ?var1 - type1 ?var2 - type2)", atomic_function_skeleton(), ast));
    EXPECT_EQ(parse_text(ast), "(function-symbol1 ?var1 - type1\n?var2 - type2)");

    EXPECT_NO_THROW(parse_ast("(function-symbol1 ?var1 ?var2 - type1)", atomic_function_skeleton(), ast));
    EXPECT_EQ(parse_text(ast), "(function-symbol1 ?var1 ?var2 - type1)");

    EXPECT_ANY_THROW(parse_ast("(?var1 ?var2 - type1)", atomic_function_skeleton(), ast));
}

}
