#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypeEitherTest) {
    ast::TypeEither ast;

    EXPECT_NO_THROW(parse_ast("(either type1 type2)", type_either(), ast));
    EXPECT_EQ(parse_text(ast), "(either type1 type2)");
    EXPECT_NO_THROW(parse_ast("(either type1 (either type2 type3))", type_either(), ast));
    EXPECT_EQ(parse_text(ast), "(either type1 (either type2 type3))");

    EXPECT_ANY_THROW(parse_ast("either", type_either(), ast));  // can be parsed into name
}

}
