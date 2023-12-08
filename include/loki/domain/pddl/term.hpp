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

#ifndef LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TERM_HPP_
#define LOKI_INCLUDE_LOKI_DOMAIN_PDDL_TERM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class TermVisitor {
public:
    virtual ~TermVisitor() = default;

    virtual void visit(const std::shared_ptr<const TermConstantImpl>& term) = 0;
    virtual void visit(const std::shared_ptr<const TermVariableImpl>& term) = 0;
};


class TermImpl : public Base<TermImpl> {
protected:
public:
    TermImpl(int identifier);
    virtual ~TermImpl();

    /// @brief Test for semantic equivalence
    virtual bool are_equal_impl(const TermImpl& other) const = 0;
    virtual size_t hash_impl() const = 0;
    virtual void str_impl(std::stringstream& out, const FormattingOptions& options) const = 0;

    virtual void accept(TermVisitor& visitor) const = 0;
};


class TermConstantImpl : public TermImpl, public std::enable_shared_from_this<TermConstantImpl> {
private:
    Object m_object;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    TermConstantImpl(int identifier, const Object& object);

    bool are_equal_impl(const TermImpl& other) const override;
    size_t hash_impl() const override;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const override;

    void accept(TermVisitor& visitor) const override;

    const Object& get_object() const;
};


class TermVariableImpl : public TermImpl, public std::enable_shared_from_this<TermVariableImpl> {
private:
    Variable m_variable;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    TermVariableImpl(int identifier, const Variable& variable);

    bool are_equal_impl(const TermImpl& other) const override;
    size_t hash_impl() const override;
    void str_impl(std::stringstream& out, const FormattingOptions& options) const override;

    void accept(TermVisitor& visitor) const override;

    const Variable& get_variable() const;
};

}



namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Term>
    {
        bool operator()(const loki::pddl::Term& left_term, const loki::pddl::Term& right_term) const;
    };

    template<>
    struct hash<loki::pddl::TermConstantImpl>
    {
        std::size_t operator()(const loki::pddl::TermConstantImpl& term) const;
    };

    template<>
    struct hash<loki::pddl::TermVariableImpl>
    {
        std::size_t operator()(const loki::pddl::TermVariableImpl& term) const;
    };
}

#endif
