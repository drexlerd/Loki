#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, FunctionTypeTest) {
    // A function type is just a number
    ast::FunctionType ast;

    EXPECT_NO_THROW(parse_ast("5", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "5");
    EXPECT_NO_THROW(parse_ast("4.2", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "4.2");
    EXPECT_NO_THROW(parse_ast("6 7", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "6");
    // TODO: Is this really what we want?
    EXPECT_NO_THROW(parse_ast("1loki", function_type(), ast));
    EXPECT_EQ(parse_text(ast), "1");

    EXPECT_ANY_THROW(parse_ast("loki", function_type(), ast));
    EXPECT_ANY_THROW(parse_ast("(5)", function_type(), ast));
}

}
