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
#include <optional>


namespace loki {
template<typename... Ts>
class Bindings {
    public:
        using KeyType = std::string;

        template<typename T>
        using BindingType = std::shared_ptr<const T>;

        using PositionType = boost::spirit::x3::position_tagged;

        template<typename T>
        struct ValueType {
            BindingType<T> binding;
            std::optional<PositionType> position;
        };

        template<typename T>
        using MapType = std::unordered_map<KeyType, ValueType<T>>;

        /// @brief Gets a binding of type T. Returns nullptr if it does not exist.
        template<typename T> 
        ValueType<T> get(const KeyType& key) const {
            const auto& t_bindings = std::get<MapType<T>>(bindings);
            auto it = t_bindings.find(key);
            if (it != t_bindings.end()) {
                return it->second;
            }
            return ValueType<T>();
        }

        /// @brief Gets all bindings of type T.
        template<typename T>
        const MapType<T>& get() const {
            return std::get<MapType<T>>(bindings);
        }

        /// @brief Inserts a binding of type T
        template<typename T>
        void insert(const KeyType& key, const BindingType<T>& binding, const std::optional<PositionType>& position = std::optional<PositionType>()) {
            auto& t_bindings = std::get<MapType<T>>(bindings);
            assert(!t_bindings.count(key));
            t_bindings.emplace(key, ValueType<T>{binding, position});
        }

    private:
        std::tuple<MapType<Ts>...> bindings;
};


/// @brief Contains scoped bindings
class Scope {
    private:
        using KeyType = std::string;

        template<typename T>
        using BindingType = std::shared_ptr<const T>;

        using PositionType = boost::spirit::x3::position_tagged;

        template<typename T>
        struct ValueType {
            BindingType<T> binding;
            PositionType position;
        };

        template<typename T>
        using MapType = std::unordered_map<KeyType, ValueType<T>>;

        std::shared_ptr<const Scope> m_parent_scope;

        Bindings<pddl::TypeImpl
            , pddl::ObjectImpl
            , pddl::PredicateImpl 
            , pddl::FunctionSkeletonImpl
            , pddl::VariableImpl> bindings;            

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
        void insert(const KeyType& name, const BindingType<T>& binding, const std::optional<PositionType>& position = std::optional<PositionType>()) {
            assert(!this->get<T>(name));
            bindings.insert<T>(name, binding, position);
        }
};

}

#endif
