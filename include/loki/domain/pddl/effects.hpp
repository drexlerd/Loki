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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_EFFECTS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
enum class AssignOperatorEnum {
    ASSIGN,
    SCALE_UP,
    SCALE_DOWN,
    INCREASE,
    DECREASE
};


/// @brief Defines an interface for visiting nodes in a DAG of conditions.
class EffectVisitor {
public:
    virtual void visit(const EffectLiteral& condition) = 0;
    virtual void visit(const EffectAnd& condition) = 0;
};


/* BaseEffect */
/// @brief Defines the common base class for effects.
///        We use polymorphism instead of variant,
///        since we wrap effects into shared_ptr.
class EffectImpl : public Base<EffectImpl> {
protected:
    EffectImpl(int identifier);

public:
    // delete copy/move to prevent accidental object slicing when passed by value
    EffectImpl(const EffectImpl& other) = delete;
    EffectImpl& operator=(const EffectImpl& other) = delete;
    EffectImpl(EffectImpl&& other) = delete;
    EffectImpl& operator=(EffectImpl&& other) = delete;
    virtual ~EffectImpl();

    /// @brief Test for structural equivalence
    virtual bool are_equal_impl(const EffectImpl& other) const = 0;

    virtual void str_impl(std::stringstream& out, const FormattingOptions& options) const = 0;

    /// @brief Accepts the visitor by calling the visit overload.
    virtual void accept(EffectVisitor& visitor) const = 0;
};


/* Literal */
class EffectLiteralImpl : public EffectImpl, public std::enable_shared_from_this<EffectLiteralImpl> {
private:
    Literal m_literal;

    EffectLiteralImpl(int identifier, const Literal& literal);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const EffectImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const override;

    void accept(EffectVisitor& visitor) const override;

    const Literal& get_literal() const;
};


/* And */
class EffectAndImpl : public EffectImpl, public std::enable_shared_from_this<EffectAndImpl> {
private:
    EffectList m_effects;

    EffectAndImpl(int identifier, const EffectList& effects);

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const EffectImpl& other) const override;
    size_t hash_impl() const;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const override;

    void accept(EffectVisitor& visitor) const override;

    const EffectList& get_effects() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Effect>
    {
        bool operator()(const loki::pddl::Effect& left_effect, const loki::pddl::Effect& right_effect) const;
    };

    template<>
    struct hash<loki::pddl::EffectLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::EffectLiteralImpl& effect) const;
    };

    template<>
    struct hash<loki::pddl::EffectAndImpl>
    {
        std::size_t operator()(const loki::pddl::EffectAndImpl& effect) const;
    };
}


#endif
