#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypeTest) {
    ast::Type ast;

    EXPECT_NO_THROW(parse_ast("(either loki kilo)", type(), ast));
    EXPECT_EQ(parse_text(ast), "(either loki kilo)");
    EXPECT_NO_THROW(parse_ast("(either loki (either kilo liko))", type(), ast));
    EXPECT_EQ(parse_text(ast), "(either loki (either kilo liko))");

    EXPECT_NO_THROW(parse_ast("either", type(), ast));  // type either expects parenthesis around
    EXPECT_ANY_THROW(parse_ast("(either either (either kilo)", type(), ast));
}

}
