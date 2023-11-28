#ifndef LOKI_INCLUDE_LOKI_COMMON_HASH_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_HASH_HPP_

#include <functional>
#include <cstddef>
#include <set>
#include <vector>


namespace loki {

// --------------
// Hash functions
// --------------

template<typename T>
inline void hash_combine(size_t& seed, const T& val)
{
    seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<>
inline void hash_combine(size_t& seed, const std::size_t& val)
{
    seed ^= val + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename... Types>
inline size_t hash_combine(const Types&... args)
{
    size_t seed = 0;
    (hash_combine(seed, args), ...);
    return seed;
}

template<class T>
inline std::size_t hash_vector(const std::vector<T>& vector)
{
    const auto hash_function = std::hash<T>();
    std::size_t aggregated_hash = 0;
    for (const auto& item : vector)
    {
        const auto item_hash = hash_function(item);
        hash_combine(aggregated_hash, item_hash);
    }
    return aggregated_hash;
}

template<typename T>
struct hash_vector_type {
    size_t operator()(const std::vector<T>& vector) const {
        return hash_vector(vector);
    }
};

template<class T>
inline std::size_t hash_set(const std::set<T>& vector)
{
    const auto hash_function = std::hash<T>();
    std::size_t aggregated_hash = 0;
    for (const auto& item : vector)
    {
        const auto item_hash = hash_function(item);
        hash_combine(aggregated_hash, item_hash);
    }
    return aggregated_hash;
}

}


#endif