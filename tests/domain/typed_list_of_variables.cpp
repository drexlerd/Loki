#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypedListOfVariablesTest) {
    ast::TypedListOfVariables ast;

    EXPECT_NO_THROW(parse_ast("?var1 ?var2 - type1 ?var3 ?var4 - type2", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "?var1 ?var2 - type1\n?var3 ?var4 - type2");

    EXPECT_NO_THROW(parse_ast("?var1 ?var2", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "?var1 ?var2");

    EXPECT_NO_THROW(parse_ast("name1 name2", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "");

    EXPECT_NO_THROW(parse_ast("- type1", typed_list_of_variables(), ast));
    EXPECT_EQ(parse_text(ast), "");
}

}
