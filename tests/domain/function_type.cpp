#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


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
