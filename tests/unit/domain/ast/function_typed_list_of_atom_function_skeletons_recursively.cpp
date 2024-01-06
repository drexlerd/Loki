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

#include "../../../../src/domain/ast/parser.hpp"
#include "../../../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../../../include/loki/domain/ast/ast.hpp"
#include "../../../../include/loki/domain/ast/printer.hpp"

#include <gtest/gtest.h>


namespace loki::domain::tests {

TEST(LokiTests, FunctionTypedListOfAtomicFunctionSkeletonsRecursivelyTest) {
    ast::FunctionTypedListOfAtomicFunctionSkeletonsRecursively ast;

    // recursive
    EXPECT_NO_THROW(parse_ast("(function-symbol1 ?var1 ?var2) - number", function_typed_list_of_atomic_function_skeletons_recursively(), ast));
    EXPECT_EQ(parse_text(ast), "(function-symbol1 ?var1 ?var2) - number");

    // implicit "number" type
    EXPECT_ANY_THROW(parse_ast("(function-symbol1 ?var1 ?var2)", function_typed_list_of_atomic_function_skeletons_recursively(), ast));

    // function type does not match "number"
    EXPECT_ANY_THROW(parse_ast("(function-symbol1 ?var1 ?var2) - wrong", function_typed_list_of_atomic_function_skeletons_recursively(), ast));
}

}
