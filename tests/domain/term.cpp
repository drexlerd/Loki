#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


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
