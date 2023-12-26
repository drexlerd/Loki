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


#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_CONDITIONS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>
#include <variant>


namespace loki {
template<typename... Ts>
class PersistentFactory;
}

namespace loki {
template<typename HolderType>
class PersistentFactory2;
}


namespace loki::pddl {

/* Literal */
class ConditionLiteralImpl : public Base<ConditionLiteralImpl> {
private:
    Literal m_literal;

    ConditionLiteralImpl(int identifier, Literal literal);

    template<typename HolderType>
    friend class loki::PersistentFactory2;

public:
    bool are_equal_impl(const ConditionLiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const Literal& get_literal() const;
};


/* And */
class ConditionAndImpl : public Base<ConditionAndImpl> {
private:
    ConditionList m_conditions;

    ConditionAndImpl(int identifier, ConditionList conditions);

    template<typename HolderType>
    friend class loki::PersistentFactory2;

public:
    bool are_equal_impl(const ConditionAndImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const ConditionList& get_conditions() const;
};


/* Or */
class ConditionOrImpl : public Base<ConditionOrImpl> {
private:
    ConditionList m_conditions;

    ConditionOrImpl(int identifier, ConditionList conditions);

    template<typename HolderType>
    friend class loki::PersistentFactory2;

public:
    bool are_equal_impl(const ConditionOrImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const ConditionList& get_conditions() const;
};


/* Not */
class ConditionNotImpl : public Base<ConditionNotImpl> {
private:
    Condition m_condition;

    ConditionNotImpl(int identifier, Condition condition);

    template<typename HolderType>
    friend class loki::PersistentFactory2;

public:
    bool are_equal_impl(const ConditionNotImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const Condition& get_condition() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Condition>
    {
        bool operator()(const loki::pddl::Condition& left_condition, const loki::pddl::Condition& right_condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionLiteralImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionAndImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionAndImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionOrImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionOrImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::ConditionNotImpl>
    {
        std::size_t operator()(const loki::pddl::ConditionNotImpl& condition) const;
    };
}


#endif
