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
#include <loki/details/pddl/factory.hpp>
#include <loki/details/pddl/object.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, FactoryIteratorTest)
{
    PDDLFactory<ObjectImpl> factory(2);
    const auto object_0 = factory.get_or_create<ObjectImpl>("object_0", TypeList());
    const auto object_1 = factory.get_or_create<ObjectImpl>("object_1", TypeList());
    const auto object_2 = factory.get_or_create<ObjectImpl>("object_2", TypeList());

    auto objects = ObjectList {};
    for (const auto& object : factory)
    {
        objects.push_back(&object);
    }

    EXPECT_EQ(objects.size(), 3);
    EXPECT_EQ(objects[0], object_0);
    EXPECT_EQ(objects[1], object_1);
    EXPECT_EQ(objects[2], object_2);
}

TEST(LokiTests, FactoryIteratorEmptyTest)
{
    PDDLFactory<ObjectImpl> factory(4);

    auto objects = ObjectList {};
    for (const auto& object : factory)
    {
        objects.push_back(&object);
    }

    EXPECT_EQ(objects.size(), 0);
}

}
