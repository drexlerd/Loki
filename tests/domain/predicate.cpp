#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, PredicateTest) {
    // A predicate is just a name
    ast::Predicate ast;

    EXPECT_NO_THROW(parse_ast("loki", predicate(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("loki kilo", predicate(), ast));
    EXPECT_NO_THROW(parse_ast("loki", predicate(), ast));
    EXPECT_NO_THROW(parse_ast("loki(kilo)", predicate(), ast));
    EXPECT_EQ(parse_text(ast), "loki");

    EXPECT_ANY_THROW(parse_ast("1loki", predicate(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", predicate(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", predicate(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", predicate(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", predicate(), ast));
    EXPECT_ANY_THROW(parse_ast("?loki", predicate(), ast));
}

}
