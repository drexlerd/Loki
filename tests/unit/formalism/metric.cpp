#include "loki/formalism/metric_data.hpp"
#include "loki/formalism/metric_index.hpp"
#include "loki/formalism/metric_view.hpp"
#include "loki/formalism/repository.hpp"

#include <concepts>

namespace f = loki::formalism;
using Index = ygg::Index<f::Metric>;
using Data = ygg::Data<f::Metric>;
using View = ygg::View<Index, f::Repository>;

static_assert(std::constructible_from<Index, ygg::uint_t>);
static_assert(std::totally_ordered<Index>);
static_assert(std::totally_ordered<Data>);
static_assert(std::totally_ordered<View>);
static_assert(std::same_as<View, f::MetricView>);
static_assert(requires(Data& data) {
    data.index;
    data.minimize;
    data.expression;
    data.clear();
    { data == data } -> std::same_as<bool>;
});
static_assert(requires(const View& view) {
    view.get_index();
    view.is_minimize();
    view.get_expression();
    { view == view } -> std::same_as<bool>;
    { view < view } -> std::same_as<bool>;
});
