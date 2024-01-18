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


#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_CONDITIONS_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_GROUND_CONDITIONS_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"

#include <string>


namespace loki {
template<typename HolderType, ElementsPerSegment N>
class PersistentFactory;
}


namespace loki::pddl {

/* Literal */
class GroundConditionLiteralImpl : public Base<GroundConditionLiteralImpl> {
private:
    GroundLiteral m_literal;

    GroundConditionLiteralImpl(int identifier, GroundLiteral literal);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionLiteralImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionLiteralImpl>;

public:
    const GroundLiteral& get_literal() const;
};


/* And */
class GroundConditionAndImpl : public Base<GroundConditionAndImpl> {
private:
    GroundConditionList m_conditions;

    GroundConditionAndImpl(int identifier, GroundConditionList conditions);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionAndImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionAndImpl>;

public:
    const GroundConditionList& get_conditions() const;
};


/* Or */
class GroundConditionOrImpl : public Base<GroundConditionOrImpl> {
private:
    GroundConditionList m_conditions;

    GroundConditionOrImpl(int identifier, GroundConditionList conditions);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionOrImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionOrImpl>;

public:
    const GroundConditionList& get_conditions() const;
};


/* Not */
class GroundConditionNotImpl : public Base<GroundConditionNotImpl> {
private:
    GroundCondition m_condition;

    GroundConditionNotImpl(int identifier, GroundCondition condition);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionNotImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionNotImpl>;

public:
    const GroundCondition& get_condition() const;
};


/* Imply */
class GroundConditionImplyImpl : public Base<GroundConditionNotImpl> {
private:
    GroundCondition m_condition_left;
    GroundCondition m_condition_right;

    GroundConditionImplyImpl(int identifier, GroundCondition condition_left, GroundCondition condition_right);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionImplyImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionImplyImpl>;

public:
    const GroundCondition& get_condition_left() const;
    const GroundCondition& get_condition_right() const;
};


/* Exists */
class GroundConditionExistsImpl : public Base<GroundConditionExistsImpl> {
private:
    ParameterList m_parameters;
    GroundCondition m_condition;

    GroundConditionExistsImpl(int identifier, ParameterList parameters, GroundCondition condition);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionExistsImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionExistsImpl>;

public:
    const ParameterList& get_parameters() const;
    const GroundCondition& get_condition() const;
};


/* Forall */
class GroundConditionForallImpl : public Base<GroundConditionForallImpl> {
private:
    ParameterList m_parameters;
    GroundCondition m_condition;

    GroundConditionForallImpl(int identifier, ParameterList parameters, GroundCondition condition);

    // Give access to the constructor.
    template<typename HolderType, ElementsPerSegment N>
    friend class loki::PersistentFactory;

    bool is_structurally_equivalent_to_impl(const GroundConditionForallImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    // Give access to the private interface implementations.
    friend class Base<GroundConditionForallImpl>;

public:
    const ParameterList& get_parameters() const;
    const GroundCondition& get_condition() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::GroundCondition>
    {
        bool operator()(const loki::pddl::GroundCondition& left_condition, const loki::pddl::GroundCondition& right_condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionLiteralImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionLiteralImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionAndImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionAndImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionOrImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionOrImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionNotImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionNotImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionImplyImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionImplyImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionExistsImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionExistsImpl& condition) const;
    };

    template<>
    struct hash<loki::pddl::GroundConditionForallImpl>
    {
        std::size_t operator()(const loki::pddl::GroundConditionForallImpl& condition) const;
    };
}


#endif
