#ifndef LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_CACHE_HPP_

#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>


namespace loki {

/// @brief A thread-safe reference-counted object cache.
/// Original idea by Herb Sutter.
/// Custom deleter idea: https://stackoverflow.com/questions/49782011/herb-sutters-10-liner-with-cleanup
/// @tparam T The underlying object type.
template<typename T>
class ReferenceCountedObjectFactory : public std::enable_shared_from_this<ReferenceCountedObjectFactory<T>> {
private:
    std::unordered_map<T, std::weak_ptr<T>> m_cache;

    /// @brief For multi-threading purposes
    mutable std::mutex m_mutex;

public:
    ReferenceCountedObjectFactory() { }

    struct InsertResult {
        std::shared_ptr<const T> object;
        bool newly_inserted;
    };

    /// @brief Gets a shared reference to the object of type T with the given arguments.
    ///        If such an object does not exists the it creates one.
    /// @tparam ...Args The arguments that are passed to the constructor of T.
    /// @param ...args
    /// @return
    template<typename... Args>
    InsertResult get_or_create(Args&&... args) {
        /* Must explicitly call the constructor of T to give exclusive access to the factory. */
        auto element = std::make_unique<T>(T(m_cache.size(), args...));
        /* we must declare sp before locking the mutex
           s.t. the deleter is called after the mutex was released in case of stack unwinding. */
        std::shared_ptr<T> sp;
        std::lock_guard<std::mutex> hold(m_mutex);
        auto& cached = m_cache[*element];
        sp = cached.lock();
        bool new_insertion = false;
        if (!sp) {
            new_insertion = true;
            cached = sp = std::shared_ptr<T>(
                element.get(),
                [parent=this->shared_from_this(), original_deleter=element.get_deleter()](T* x)
                {
                    {
                        std::lock_guard<std::mutex> hold(parent->m_mutex);
                        parent->m_cache.erase(*x);
                    }
                    /* After cache removal, we can call the objects destructor
                       and recursively call the deleter of children if their ref count goes to 0 */
                    original_deleter(x);
                }
            );
            element.release();
        }
        return InsertResult{sp, new_insertion};
    }
};

}


#endif