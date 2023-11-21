#ifndef LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_

#include <unordered_set>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>


namespace loki {

/// @brief A persistent object cache without reference counting
///        TODO: untested, this is just a draft.
template<typename... Ts>
class PersistentObjectFactory {
private:
    // We must compare the underlying object
    template<typename T>
    struct Comparator {
        bool operator==(const std::unique_ptr<const T>& left, const std::unique_ptr<const T>& right) const {
            return *left == *right;
        }
    };

    template<typename T>
    struct PerTypeCache {
        std::unordered_set<std::unique_ptr<const T>, Comparator<T>> data;
    };

    std::tuple<PerTypeCache<Ts>...> m_cache;

    // Identifiers are shared since types can be polymorphic
    int m_count = 0;

public:
    PersistentObjectFactory() { }

    template<typename T>
    struct GetOrCreateResult {
        T const *object;
        bool created;
    };

    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists then it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename T, typename... Args>
    GetOrCreateResult<T> get_or_create(Args&&... args) {
        auto& t_cache = std::get<PerTypeCache<T>>(m_cache);
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        auto element = std::make_unique<T>(T(m_count, args...));
        auto result = t_cache->data.find(element);
        if (result.second) {
            ++m_count;
        }
        return GetOrCreateResult<T>{*result.first.get(), result.second};
    }
};

}


#endif