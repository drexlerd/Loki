#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypedListOfNamesTest) {
    ast::TypedListOfNames ast;

    EXPECT_NO_THROW(parse_ast("name1 name2 - type1 name3 name4 - type2", typed_list_of_names(), ast));
    EXPECT_EQ(parse_text(ast), "name1 name2 - type1\nname3 name4 - type2");  // printer adds leading whitespace

    EXPECT_NO_THROW(parse_ast("name1 name2", typed_list_of_names(), ast));
    EXPECT_EQ(parse_text(ast), "name1 name2");

    EXPECT_NO_THROW(parse_ast("?var1 ?var2", typed_list_of_names(), ast));
    EXPECT_EQ(parse_text(ast), "");

    EXPECT_NO_THROW(parse_ast("- type1", typed_list_of_names(), ast));
    EXPECT_EQ(parse_text(ast), "");
}

}
