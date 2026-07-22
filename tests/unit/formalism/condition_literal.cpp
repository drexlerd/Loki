#include "loki/formalism/condition_literal_data.hpp"
#include "loki/formalism/condition_literal_index.hpp"
#include "loki/formalism/condition_literal_view.hpp"
#include "loki/formalism/repository.hpp"

#include <concepts>

namespace f = loki::formalism;
using Index = ygg::Index<f::ConditionLiteral>;
using Data = ygg::Data<f::ConditionLiteral>;
using View = ygg::View<Index, f::Repository>;

static_assert(std::constructible_from<Index, ygg::uint_t>);
static_assert(std::totally_ordered<Index>);
static_assert(std::totally_ordered<Data>);
static_assert(std::totally_ordered<View>);
static_assert(std::same_as<View, f::ConditionLiteralView>);
static_assert(requires(Data& data) {
    data.index;
    data.literal;
    data.clear();
    { data == data } -> std::same_as<bool>;
});
static_assert(requires(const View& view) {
    view.get_index();
    view.get_literal();
    { view == view } -> std::same_as<bool>;
    { view < view } -> std::same_as<bool>;
});
