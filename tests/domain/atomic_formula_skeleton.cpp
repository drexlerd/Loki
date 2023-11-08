#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"
#include "tests/common/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, AtomicFormulaSkeletonTest) {
    ast::AtomicFormulaSkeleton ast;

    EXPECT_NO_THROW(parse_ast("(predicate1 ?var1 ?var2)", atomic_formula_skeleton(), ast));
    EXPECT_EQ(parse_text(ast, get_test_options()), "(predicate1 ?var1 ?var2)");

    EXPECT_NO_THROW(parse_ast("(predicate1 ?var1 - type1 ?var2 - type2)", atomic_formula_skeleton(), ast));
    EXPECT_EQ(parse_text(ast, get_test_options()), "(predicate1 ?var1 - type1\n?var2 - type2)");

    EXPECT_NO_THROW(parse_ast("(predicate1 ?var1 ?var2 - type1)", atomic_formula_skeleton(), ast));
    EXPECT_EQ(parse_text(ast, get_test_options()), "(predicate1 ?var1 ?var2 - type1)");

    EXPECT_ANY_THROW(parse_ast("(?var1 ?var2 - type1)", atomic_formula_skeleton(), ast));
}

}
