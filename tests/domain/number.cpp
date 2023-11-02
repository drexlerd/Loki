#include <gtest/gtest.h>

#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"
#include "tests/parser_wrapper.hpp"


namespace loki::domain::tests {

TEST(LokiTests, NumberTest) {
    ast::Number ast;

    EXPECT_NO_THROW(parse_ast("5", number(), ast));
    EXPECT_EQ(parse_text(ast), "5");
    EXPECT_NO_THROW(parse_ast("4.2", number(), ast));
    EXPECT_EQ(parse_text(ast), "4.2");
    EXPECT_NO_THROW(parse_ast("1something", number(), ast));
    EXPECT_EQ(parse_text(ast), "1");

    EXPECT_ANY_THROW(parse_ast("something", number(), ast));
    EXPECT_ANY_THROW(parse_ast("(5)", number(), ast));
}

}
