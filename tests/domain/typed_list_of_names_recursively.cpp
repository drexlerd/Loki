#include <gtest/gtest.h>

#include "../../include/loki/common/ast/parser_wrapper.hpp"
#include "../../include/loki/domain/ast/parser.hpp"
#include "../../include/loki/domain/ast/printer.hpp"


namespace loki::domain::tests {

TEST(LokiTests, TypedListOfNamesRecursivelyTest) {
    ast::TypedListOfNamesRecursively ast;
    pddl_context_type pddl_context;
    pddl_context.domain_context.requirements.typing = true;

    EXPECT_ANY_THROW(parse_ast("name1 name2 - type1", typed_list_of_names_recursively(), ast));
    EXPECT_NO_THROW(parse_ast("name1 name2 - type1", typed_list_of_names_recursively(), ast, pddl_context));
    EXPECT_EQ(parse_text(ast), "name1 name2 - type1");

    EXPECT_ANY_THROW(parse_ast("name1 name2", typed_list_of_names_recursively(), ast, pddl_context));
    EXPECT_ANY_THROW(parse_ast("?var1 ?var2", typed_list_of_names_recursively(), ast, pddl_context));
}

}
