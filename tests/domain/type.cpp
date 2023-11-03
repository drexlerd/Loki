#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypeTest) {
    ast::Type ast;

    EXPECT_NO_THROW(parse_ast("(either type1 type2)", type(), ast));
    EXPECT_EQ(parse_text(ast), "(either type1 type2)");
    EXPECT_NO_THROW(parse_ast("(either type1 (either type2 type3))", type(), ast));
    EXPECT_EQ(parse_text(ast), "(either type1 (either type2 type3))");

    EXPECT_NO_THROW(parse_ast("either", type(), ast));  // type either expects parenthesis around
}

}
