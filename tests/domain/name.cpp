#include <gtest/gtest.h>

#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, NameTest) {
    ast::Name ast;

    EXPECT_NO_THROW(parse_ast("loki", name(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("loki kilo", name(), ast));
    EXPECT_NO_THROW(parse_ast("loki", name(), ast));
    EXPECT_NO_THROW(parse_ast("loki(kilo)", name(), ast));
    EXPECT_EQ(parse_text(ast), "loki");

    EXPECT_ANY_THROW(parse_ast("1loki", name(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", name(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", name(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", name(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", name(), ast));
    EXPECT_ANY_THROW(parse_ast("?loki", name(), ast));
}

}
