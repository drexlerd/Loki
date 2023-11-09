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
