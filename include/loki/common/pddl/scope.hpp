/*
 * Copyright (C) 2023 Dominik Drexler
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_
#define LOKI_INCLUDE_LOKI_COMMON_PDDL_SCOPE_HPP_

#include "../../domain/pddl/type.hpp"
#include "../../domain/pddl/object.hpp"
#include "../../domain/pddl/predicate.hpp"
#include "../../domain/pddl/function_skeleton.hpp"
#include "../../domain/pddl/variable.hpp"

#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <cassert>
#include <unordered_map>
#include <memory>
#include <tuple>


namespace loki {

template<template<typename> class MapTypeFunctor, typename... Ts>
class KeyValueMap {
private:
    using KeyType = std::string;

    template<typename T>
    using MapType = typename MapTypeFunctor<T>::type;

    std::tuple<MapType<Ts>...> storage;

    public:
        template<typename T> 
        auto get(const KeyType& key) const {
            const auto& t_storage = std::get<MapType<T>>(storage);
            auto it = t_storage.find(key);
            if (it != t_storage.end()) {
                return it->second;
            }
            return typename MapType<T>::mapped_type{}; // Default-constructed value
        }

        template<typename T>
        void insert(const KeyType& key, const typename MapType<T>::mapped_type& value) {
            auto& t_storage = std::get<MapType<T>>(storage);
            assert(!t_storage.count(key));
            t_storage.emplace(key, value);
        }

        template<typename T>
        const MapType<T>& get() const {
            return std::get<MapType<T>>(storage);
        }
};

// Functor to define map type for Positions, ignoring the template parameter
template<typename T>
struct PositionMapType {
    using type = std::unordered_map<std::string, boost::spirit::x3::position_tagged>;
};

// Functor to define map type for Bindings
template<typename T>
struct BindingMapType {
    using type = std::unordered_map<std::string, std::shared_ptr<const T>>;
};

// Specialized Storage Classes
template<typename... Ts>
using Positions = KeyValueMap<PositionMapType, Ts...>;

template<typename... Ts>
using Bindings = KeyValueMap<BindingMapType, Ts...>;


/// @brief Contains scoped bindings
class Scope {
    private:
        using KeyType = std::string;

        template<typename T>
        using ValueType = std::shared_ptr<const T>;

        template<typename T>
        using MapType = std::unordered_map<KeyType, ValueType<T>>;

        std::shared_ptr<const Scope> m_parent_scope;

        Bindings<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl 
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings; 

        Positions<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl 
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> positions;           

    public:
        explicit Scope(std::shared_ptr<const Scope> parent_scope = nullptr) 
            : m_parent_scope(parent_scope) { }

        /// @brief Returns the binding or nullptr if name resolution fails.
        template<typename T>
        ValueType<T> get(const KeyType& name) const {
            auto result = bindings.get<T>(name);
            if (result) return result;
            if (m_parent_scope) return m_parent_scope->get<T>(name);
            return nullptr;  // indicates failure of name resolution.
        }

        /// @brief Returns all bindings of type T defined in the scope includings its parent scopes.
        template<typename T>
        MapType<T> get() const {
            MapType<T> result = bindings.get<T>();
            if (m_parent_scope) {
                auto parent_bindings = m_parent_scope->get<T>();
                result.insert(parent_bindings.begin(), parent_bindings.end());
            }
            return result;
        }

        /// @brief Insert binding of type T.
        template<typename T>
        void insert(const KeyType& name, const ValueType<T>& binding) {
            assert(!get<T>(name));
            bindings.insert<T>(name, binding);
        }
};

}

#endif
