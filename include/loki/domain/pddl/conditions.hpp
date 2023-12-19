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


namespace loki {
template<typename... Ts>
class PersistentFactory;
}


namespace loki::pddl {
/// @brief Defines an interface for visiting nodes in a DAG of conditions.
class ConditionVisitor {
public:
    virtual ~ConditionVisitor() = default;

    virtual void visit(const ConditionLiteral& condition) = 0;
    virtual void visit(const ConditionAnd& condition) = 0;
    virtual void visit(const ConditionOr& condition) = 0;
    virtual void visit(const ConditionNot& condition) = 0;
};


/* BaseCondition */
/// @brief Defines the common base class for conditions.
///        We use polymorphism instead of variant,
///        since we wrap conditions into shared_ptr.
class ConditionImpl : public Base<ConditionImpl> {
protected:
    ConditionImpl(int identifier);

    // protected copy/move to prevent accidental object slicing when passed by value
    ConditionImpl(const ConditionImpl& other) = default;
    ConditionImpl& operator=(const ConditionImpl& other) = default;
    ConditionImpl(ConditionImpl&& other) = default;
    ConditionImpl& operator=(ConditionImpl&& other) = default;

public:
    virtual ~ConditionImpl();

    /// @brief Test for structural equivalence
    virtual bool are_equal_impl(const ConditionImpl& other) const = 0;
    virtual void str_impl(std::ostringstream& out, const FormattingOptions& options) const = 0;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(ConditionVisitor& visitor) const = 0;
};


/* Literal */
class ConditionLiteralImpl : public ConditionImpl {
private:
    Literal m_literal;

    ConditionLiteralImpl(int identifier, Literal literal);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const ConditionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(ConditionVisitor& visitor) const override;

    const Literal& get_literal() const;
};


/* And */
class ConditionAndImpl : public ConditionImpl {
private:
    ConditionList m_conditions;

    ConditionAndImpl(int identifier, ConditionList conditions);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const ConditionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(ConditionVisitor& visitor) const override;

    const ConditionList& get_conditions() const;
};


/* Or */
class ConditionOrImpl : public ConditionImpl {
private:
    ConditionList m_conditions;

    ConditionOrImpl(int identifier, ConditionList conditions);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const ConditionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(ConditionVisitor& visitor) const override;

    const ConditionList& get_conditions() const;
};


/* Not */
class ConditionNotImpl : public ConditionImpl {
private:
    Condition m_condition;

    ConditionNotImpl(int identifier, Condition condition);

    template<typename... Ts>
    friend class loki::PersistentFactory;

public:
    bool are_equal_impl(const ConditionImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const override;

    void accept(ConditionVisitor& visitor) const override;

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
