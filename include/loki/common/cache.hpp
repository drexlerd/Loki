#ifndef LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_

#include <cassert>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>


namespace loki {
template<typename KeyType, typename... Ts>
class Cache {
    private:
        template<typename T>
        using ElementType = std::shared_ptr<const T>;

        template<typename T>
        using MapType = std::unordered_map<KeyType, ElementType<T>>;

        std::tuple<MapType<Ts>...> bindings;

    public:
        /// @brief Gets a binding of type T. Returns nullptr if it does not exist.
        template<typename T> 
        ElementType<T> get(const KeyType& key) const {
            const auto& t_bindings = std::get<MapType<T>>(bindings);
            auto it = t_bindings.find(key);
            if (it != t_bindings.end()) {
                return it->second;
            }
            return nullptr;
        }

        template<typename T>
        const MapType<T>& get() const {
            return std::get<MapType<T>>(bindings);
        }

        /// @brief Inserts a binding of type T
        template<typename T>
        void insert(const KeyType& key, const ElementType<T>& binding) {
            auto& t_bindings = std::get<MapType<T>>(bindings);
            assert(!t_bindings.count(key));
            t_bindings.emplace(key, binding);
        }
};

}


#endif