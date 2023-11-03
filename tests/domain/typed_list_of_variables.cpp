#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypedListOfVariablesTest) {
    ast::TypedListOfVariables ast;

    EXPECT_NO_THROW(parse_ast("?kilo ?liko - loki ?olik ?ikol - kilo", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), " ?kilo ?liko - loki ?olik ?ikol - kilo");  // printer adds leading whitespace

    EXPECT_NO_THROW(parse_ast("?kilo ?liko", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "?kilo ?liko");

    EXPECT_NO_THROW(parse_ast("kilo liko", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "");

    EXPECT_NO_THROW(parse_ast("- kilo", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "");
}

}
