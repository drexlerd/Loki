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
    std::cout << *domain_translation_result.get_translated_domain() << std::endl;
    const auto translated_problem = loki::translate(problem, domain_translation_result);
    std::cout << *translated_problem << std::endl;

    // Check that all domain constants are in the problem
    auto problem_objects = ObjectSet(translated_problem->get_objects().begin(), translated_problem->get_objects().end());
    for (const auto& constant : domain->get_constants())
    {
        EXPECT_TRUE(problem_objects.contains(constant));
    }

    // Check that all problem types are domain types.
    const auto& domain_type_list = boost::hana::at_key(domain->get_repositories().get_hana_repositories(), boost::hana::type<TypeImpl> {});
    auto domain_type_set = TypeSet(domain_type_list.begin(), domain_type_list.end());
    const auto& problem_type_list = boost::hana::at_key(problem->get_repositories().get_hana_repositories(), boost::hana::type<TypeImpl> {});
    for (const auto& type : problem_type_list)
    {
        EXPECT_TRUE(domain_type_set.contains(type));
    }
}

}
