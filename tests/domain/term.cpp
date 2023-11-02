#include <gtest/gtest.h>

#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"
#include "tests/parser_wrapper.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TermTest) {
    ast::Term ast;

    EXPECT_NO_THROW(parse_ast("?loki", term(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("loki", term(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("?loki(?loki)", term(), ast));
    EXPECT_EQ(parse_text(ast), "?loki");
    EXPECT_NO_THROW(parse_ast("loki(loki)", term(), ast));
    EXPECT_EQ(parse_text(ast), "loki");

    EXPECT_ANY_THROW(parse_ast("1loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", term(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", term(), ast));
}

}
