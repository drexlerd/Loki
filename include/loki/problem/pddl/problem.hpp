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

#ifndef LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_
#define LOKI_INCLUDE_LOKI_PROBLEM_PDDL_PROBLEM_HPP_

#include "declarations.hpp"

#include "../../common/pddl/base.hpp"


namespace loki {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace loki::pddl {
class ProblemImpl : public Base<ProblemImpl> {
private:
    Domain m_domain;
    std::string m_name;
    Requirements m_requirements;
    ObjectList m_objects;
    GroundLiteralList m_initial_literals;
    Condition m_goal_condition;

    template<typename... Ts>
    friend class loki::ReferenceCountedObjectFactory;

public:
    ProblemImpl(int identifier, const Domain& domain, const std::string& name, const Requirements& requirements, const ObjectList& objects, const GroundLiteralList& initial_literals, const Condition& goal_condition);

    /// @brief Test for semantic equivalence
    bool are_equal_impl(const ProblemImpl& other) const;
    size_t hash_impl() const;
    void str_impl(std::ostringstream& out, const FormattingOptions& options) const;

    const Domain& get_domain() const;
    const std::string& get_name() const;
    const Requirements& get_requirements() const;
    const ObjectList& get_objects() const;
    const GroundLiteralList& get_initial_literals() const;
    const Condition& get_goal_condition() const;
};

}


namespace std {
    // Inject comparison and hash function to make pointers behave appropriately with ordered and unordered datastructures
    template<>
    struct less<loki::pddl::Problem>
    {
        bool operator()(const loki::pddl::Problem& left_problem, const loki::pddl::Problem& right_problem) const;
    };

    template<>
    struct hash<loki::pddl::ProblemImpl>
    {
        std::size_t operator()(const loki::pddl::ProblemImpl& problem) const;
    };
}

#endif
