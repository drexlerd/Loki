#include <gtest/gtest.h>

#include "include/loki/domain/parser.hpp"
#include "tests/parser_wrapper.hpp"


namespace loki::domain::tests {

TEST(LokiTests, NameTest) {
    EXPECT_NO_THROW((parse_text("something", name())));
    EXPECT_ANY_THROW((parse_text("1something", name())));
    EXPECT_ANY_THROW((parse_text("-something", name())));
    EXPECT_ANY_THROW((parse_text("+something", name())));
    EXPECT_ANY_THROW((parse_text("*something", name())));
    EXPECT_ANY_THROW((parse_text("/something", name())));
    EXPECT_ANY_THROW((parse_text("?something", name())));
}

}
