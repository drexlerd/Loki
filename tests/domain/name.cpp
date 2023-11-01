#include <gtest/gtest.h>

#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"
#include "tests/parser_wrapper.hpp"


namespace loki::domain::tests {

TEST(LokiTests, NameTest) {
    ast::Name ast;
    EXPECT_NO_THROW(parse_ast("something", name(), ast));
    EXPECT_EQ(parse_text(ast), "something");
    EXPECT_ANY_THROW((parse_ast("1something", name(), ast)));
    EXPECT_ANY_THROW((parse_ast("-something", name(), ast)));
    EXPECT_ANY_THROW((parse_ast("+something", name(), ast)));
    EXPECT_ANY_THROW((parse_ast("*something", name(), ast)));
    EXPECT_ANY_THROW((parse_ast("/something", name(), ast)));
    EXPECT_ANY_THROW((parse_ast("?something", name(), ast)));
}

}
