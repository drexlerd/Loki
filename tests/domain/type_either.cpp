#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypeEitherTest) {
    ast::TypeEither ast;

    EXPECT_NO_THROW(parse_ast("(either loki kilo)", type_either(), ast));
    EXPECT_EQ(parse_text(ast), "(either loki kilo)");
    EXPECT_NO_THROW(parse_ast("(either loki (either kilo liko))", type_either(), ast));
    EXPECT_EQ(parse_text(ast), "(either loki (either kilo liko))");

    EXPECT_ANY_THROW(parse_ast("either", type_either(), ast));  // can be parsed into name
    EXPECT_ANY_THROW(parse_ast("(either either (either kilo)", type_either(), ast));
}

}
