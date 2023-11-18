#ifndef LOKI_INCLUDE_LOKI_COMMON_COLLECTIONS_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_COLLECTIONS_HPP_

#include <algorithm>
#include <vector>


namespace loki {

/// @brief Returns the sorted vector
/// @tparam T
/// @param vec
/// @return
template<typename T>
std::vector<T> sorted(const std::vector<T>& vec) {
    std::vector<T> result(vec.begin(), vec.end());
    std::sort(result.begin(), result.end());
    return result;
}

}

#endif