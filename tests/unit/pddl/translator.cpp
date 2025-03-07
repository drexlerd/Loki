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

#include <gtest/gtest.h>
#include <loki/details/pddl/domain.hpp>
#include <loki/details/pddl/parser.hpp>
#include <loki/details/pddl/problem.hpp>
#include <loki/details/pddl/translator.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, LokiPddlTranslatorTest)
{
    const auto domain_file = fs::path(std::string(DATA_DIR) + "miconic-fulladl/domain.pddl");
    const auto problem_file = fs::path(std::string(DATA_DIR) + "miconic-fulladl/test_problem.pddl");

    auto parser = loki::Parser(domain_file);
    const auto domain = parser.get_domain();
    std::cout << *domain << std::endl << std::endl;
    const auto problem = parser.parse_problem(problem_file);
    std::cout << *problem << std::endl;
    const auto domain_translation_result = loki::translate(domain);
    const auto translated_domain = domain_translation_result.get_translated_domain();
    std::cout << *domain_translation_result.get_translated_domain() << std::endl;
    const auto translated_problem = loki::translate(problem, domain_translation_result);
    std::cout << *translated_problem << std::endl;

    {
        // Check that all domain constants are in the problem
        auto problem_objects = ObjectSet(translated_problem->get_objects().begin(), translated_problem->get_objects().end());
        for (const auto& constant : translated_domain->get_constants())
        {
            EXPECT_TRUE(problem_objects.contains(constant));
        }
    }

    {
        // Check that all problem types are domain types.
        const auto& domain_type_list = boost::hana::at_key(domain->get_repositories().get_hana_repositories(), boost::hana::type<TypeImpl> {});
        auto domain_type_set = TypeSet(domain_type_list.begin(), domain_type_list.end());
        const auto& problem_type_list = boost::hana::at_key(problem->get_repositories().get_hana_repositories(), boost::hana::type<TypeImpl> {});
        for (const auto& type : problem_type_list)
        {
            EXPECT_TRUE(domain_type_set.contains(type));
        }
    }

    {
        // Check that problem does not introduce object with same name as constant in domain.
        auto domain_constants = std::unordered_map<std::string, Object> {};
        for (const auto& constant : translated_domain->get_constants())
        {
            domain_constants.emplace(constant->get_name(), constant);
        }

        for (const auto& object : translated_problem->get_objects())
        {
            if (domain_constants.contains(object->get_name()))
            {
                EXPECT_EQ(object, domain_constants.at(object->get_name()));
            }
        }
    }

    {
        // Check that problem does not introduce predicate with same name as predicate in domain.
        auto domain_predicates = std::unordered_map<std::string, Predicate> {};
        for (const auto& predicate : translated_domain->get_predicates())
        {
            domain_predicates.emplace(predicate->get_name(), predicate);
        }

        for (const auto& predicate : translated_problem->get_predicates())
        {
            if (domain_predicates.contains(predicate->get_name()))
            {
                EXPECT_EQ(predicate, domain_predicates.at(predicate->get_name()));
            }
        }
    }

    {
        // Check that problem does not introduce function skeleton with same name as function skeleton in domain.
        auto domain_function_skeletons = std::unordered_map<std::string, FunctionSkeleton> {};
        for (const auto& function_skeleton : translated_domain->get_function_skeletons())
        {
            domain_function_skeletons.emplace(function_skeleton->get_name(), function_skeleton);
        }

        const auto& problem_function_skeletons =
            boost::hana::at_key(translated_domain->get_repositories().get_hana_repositories(), boost::hana::type<FunctionSkeletonImpl> {});
        for (const auto& function_skeleton : problem_function_skeletons)
        {
            if (domain_function_skeletons.contains(function_skeleton->get_name()))
            {
                EXPECT_EQ(function_skeleton, domain_function_skeletons.at(function_skeleton->get_name()));
            }
        }
    }

    {
        // Check that all constants and objects are untyped
        for (const auto& constant : translated_domain->get_constants())
        {
            EXPECT_EQ(constant->get_bases().size(), 0);
        }
        for (const auto& object : translated_problem->get_objects())
        {
            EXPECT_EQ(object->get_bases().size(), 0);
        }
    }
}

}
