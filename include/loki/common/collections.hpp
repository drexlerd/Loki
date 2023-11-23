#ifndef LOKI_INCLUDE_LOKI_COMMON_COLLECTIONS_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_COLLECTIONS_HPP_

#include <algorithm>
#include <vector>


namespace loki {

/// @brief Returns the sorted vector
/// @tparam T
/// @param vec
/// @return
template<typename Collection>
std::vector<typename Collection::value_type> get_sorted_vector(const Collection& collection) {
    std::vector<typename Collection::value_type> result(collection.begin(), collection.end());
    std::sort(result.begin(), result.end());
    return result;
}

}

#endif