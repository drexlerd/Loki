#include <gtest/gtest.h>

#include "include/loki/common/parser_wrapper.hpp"
#include "include/loki/domain/parser.hpp"
#include "include/loki/domain/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypedListOfNamesRecursivelyTest) {
    ast::TypedListOfNamesRecursively ast;

    EXPECT_NO_THROW(parse_ast("kilo liko - loki", typed_list_of_names_recursively(), ast));
    EXPECT_EQ(parse_text(ast), " kilo liko - loki");  // printer adds leading whitespace

    EXPECT_ANY_THROW(parse_ast("kilo liko", typed_list_of_names_recursively(), ast));
    EXPECT_ANY_THROW(parse_ast("?kilo ?liko", typed_list_of_names_recursively(), ast));
}

}
