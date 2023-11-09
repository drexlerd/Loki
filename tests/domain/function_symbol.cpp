#include <gtest/gtest.h>

#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, FunctionSymbolTest) {
    // A function symbol is just a name
    ast::FunctionSymbol ast;

    EXPECT_NO_THROW(parse_ast("loki", function_symbol(), ast));
    EXPECT_EQ(parse_text(ast), "loki");
    EXPECT_NO_THROW(parse_ast("loki kilo", function_symbol(), ast));
    EXPECT_NO_THROW(parse_ast("loki", function_symbol(), ast));
    EXPECT_NO_THROW(parse_ast("loki(kilo)", function_symbol(), ast));
    EXPECT_EQ(parse_text(ast), "loki");

    EXPECT_ANY_THROW(parse_ast("1loki", function_symbol(), ast));
    EXPECT_ANY_THROW(parse_ast("-loki", function_symbol(), ast));
    EXPECT_ANY_THROW(parse_ast("+loki", function_symbol(), ast));
    EXPECT_ANY_THROW(parse_ast("*loki", function_symbol(), ast));
    EXPECT_ANY_THROW(parse_ast("/loki", function_symbol(), ast));
    EXPECT_ANY_THROW(parse_ast("?loki", function_symbol(), ast));
}

}
