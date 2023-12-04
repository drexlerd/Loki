#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_

#include "../cache.hpp"

#include "../../domain/pddl/type.hpp"
#include "../../domain/pddl/object.hpp"
#include "../../domain/pddl/predicate.hpp"
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/variable.hpp"

#include <cassert>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <iostream>
#include <tuple>



namespace loki {
/// @brief Contains scoped bindings
class Scope {
    private:
        template<typename T>
        using ElementType = std::shared_ptr<const T>;

        template<typename T>
        using MapType = std::unordered_map<std::string, ElementType<T>>;

        std::shared_ptr<const Scope> m_parent_scope;

        Cache<std::string
            , pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl 
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings;            

    public:
        explicit Scope(std::shared_ptr<const Scope> parent_scope = nullptr) 
            : m_parent_scope(parent_scope) { }

        /// @brief Returns the binding or nullptr if name resolution fails.
        template<typename T>
        ElementType<T> get(const std::string& name) const {
            auto result = bindings.get<T>(name);
            if (result) return result;
            if (m_parent_scope) return m_parent_scope->get<T>(name);
            return nullptr;
        }

        /// @brief Returns all bindings of type T defined in the scope includings its parent scopes.
        template<typename T>
        MapType<T> get() const {
            MapType<T> result = bindings.get<T>();
            if (m_parent_scope) {
                auto parent_bindings = m_parent_scope->get<T>();
                result.emplace(parent_bindings.begin(), parent_bindings.end());
            }
            return result;
        }

        /// @brief Insert binding of type T.
        template<typename T>
        void insert(const std::string& name, const ElementType<T>& binding) {
            assert(!this->get<T>(name));
            bindings.insert<T>(name, binding);
        }
};

}

#endif
