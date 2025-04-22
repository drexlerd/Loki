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
#include <loki/details/pddl/object.hpp>
#include <loki/details/pddl/term.hpp>
#include <loki/details/pddl/type.hpp>
#include <loki/details/pddl/variable.hpp>
#include <loki/details/utils/equal_to.hpp>
#include <loki/details/utils/hash.hpp>
#include <loki/details/utils/segmented_repository.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, LokiUtilsSegmentedRepositoryIteratorTest)
{
    SegmentedRepository<ObjectImpl> factory(2);

    const auto object_0 = factory.get_or_create("object_0", TypeList());
    const auto object_1 = factory.get_or_create("object_1", TypeList());
    const auto object_2 = factory.get_or_create("object_2", TypeList());

    auto objects = ObjectList {};
    for (const auto& object : factory)
    {
        objects.push_back(object);
    }

    EXPECT_EQ(objects.size(), 3);
    EXPECT_EQ(objects[0], object_0);
    EXPECT_EQ(objects[1], object_1);
    EXPECT_EQ(objects[2], object_2);
}

TEST(LokiTests, UtilsSegmentedRepositoryIteratorEmptyTest)
{
    SegmentedRepository<ObjectImpl> factory(4);

    auto objects = ObjectList {};
    for (const auto& object : factory)
    {
        objects.push_back(object);
    }

    EXPECT_EQ(objects.size(), 0);
}

TEST(LokiTests, UtilsSegmentedRepositoryVariantTest)
{
    SegmentedRepository<ObjectImpl> objects(2);
    SegmentedRepository<TermImpl> terms(2);
    const auto object_0 = objects.get_or_create("object_0", TypeList());
    const auto object_1 = objects.get_or_create("object_1", TypeList());

    const auto term_0_object_0 = terms.get_or_create(object_0);
    const auto term_1_object_0 = terms.get_or_create(object_0);
    const auto term_2_object_1 = terms.get_or_create(object_1);

    EXPECT_EQ(term_0_object_0, term_1_object_0);
    EXPECT_NE(term_0_object_0, term_2_object_1);
}

TEST(LokiTests, UtilsSegmentedRepositoryTest)
{
    SegmentedRepository<ObjectImpl> factory(2);
    EXPECT_EQ(factory.size(), 0);

    // Test uniqueness: insert the same element twice
    const auto object_0_0 = factory.get_or_create("object_0", TypeList());
    EXPECT_EQ(factory.size(), 1);
    EXPECT_EQ(object_0_0->get_index(), 0);
    EXPECT_EQ(object_0_0->get_name(), "object_0");

    const auto object_0_1 = factory.get_or_create("object_0", TypeList());
    EXPECT_EQ(factory.size(), 1);
    EXPECT_EQ(object_0_0, object_0_1);
    EXPECT_EQ((Hash<ObjectImpl>()(*object_0_0)), (Hash<ObjectImpl>()(*object_0_1)));
    EXPECT_TRUE((EqualTo<ObjectImpl>()(*object_0_0, *object_0_1)));

    const auto object_1 = factory.get_or_create("object_1", TypeList());
    EXPECT_EQ(factory.size(), 2);
    EXPECT_NE(object_0_0, object_1);
    EXPECT_EQ(object_1->get_index(), 1);
    EXPECT_EQ(object_1->get_name(), "object_1");
    EXPECT_NE((Hash<ObjectImpl>()(*object_0_0)), (Hash<ObjectImpl>()(*object_1)));
    EXPECT_FALSE((EqualTo<ObjectImpl>()(*object_0_0, *object_1)));
}

}
