#include "loki/formalism/builder.hpp"
#include "loki/formalism/multi_function_expression_data.hpp"
#include "loki/formalism/multi_function_expression_index.hpp"
#include "loki/formalism/multi_function_expression_view.hpp"
#include "loki/formalism/repository.hpp"

#include <concepts>
#include <gtest/gtest.h>
#include <stdexcept>

namespace f = loki::formalism;
using Index = ygg::Index<f::MultiFunctionExpression>;
using Data = ygg::Data<f::MultiFunctionExpression>;
using View = ygg::View<Index, f::Repository>;

static_assert(std::constructible_from<Index, ygg::uint_t>);
static_assert(std::totally_ordered<Index>);
static_assert(std::totally_ordered<Data>);
static_assert(std::totally_ordered<View>);
static_assert(std::same_as<View, f::MultiFunctionExpressionView>);
static_assert(std::constructible_from<Data, f::MultiArithmeticOperator, ygg::Index<f::FunctionExpression>, ygg::Index<f::FunctionExpression>>);
static_assert(!std::constructible_from<Data, f::MultiArithmeticOperator, ygg::IndexList<f::FunctionExpression>>);
static_assert(requires(Data& data) {
    data.index;
    data.op;
    data.first;
    data.second;
    data.remaining;
    data.clear();
    { data == data } -> std::same_as<bool>;
});
static_assert(requires(const View& view) {
    view.get_index();
    view.get_operator();
    view.get_first();
    view.get_second();
    view.get_remaining();
    { view == view } -> std::same_as<bool>;
    { view < view } -> std::same_as<bool>;
});

TEST(LokiTests, MultiFunctionExpressionRejectsMissingRequiredOperands)
{
    auto repository = f::Repository(0);
    EXPECT_THROW(static_cast<void>(f::get_or_create(repository, Data {})), std::invalid_argument);
}
