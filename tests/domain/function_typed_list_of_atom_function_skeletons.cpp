#include <gtest/gtest.h>

#include "include/loki/common/syntactic/parser_wrapper.hpp"
#include "include/loki/domain/syntactic/parser.hpp"
#include "include/loki/domain/syntactic/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, FunctionTypedListOfAtomicFunctionSkeletonsTest) {
    ast::FunctionTypedListOfAtomicFunctionSkeletons ast;

    EXPECT_NO_THROW(parse_ast("(function-symbol1 ?var1 ?var2) - function-type1", function_typed_list_of_atomic_function_skeletons(), ast));
    EXPECT_EQ(parse_text(ast), "(function-symbol1 ?var1 ?var2) - function-type1");


}

}
