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

TEST(LokiTests, PersistentFactoryTest)
{
    PDDLFactory<ObjectImpl> factory(2);
    EXPECT_EQ(factory.size(), 0);

    // Test uniqueness: insert the same element twice
    const auto object_0_0 = factory.get_or_create<ObjectImpl>("object_0", TypeList());
    EXPECT_EQ(factory.size(), 1);
    EXPECT_EQ(object_0_0->get_identifier(), 0);
    EXPECT_EQ(object_0_0->get_name(), "object_0");

    const auto object_0_1 = factory.get_or_create<ObjectImpl>("object_0", TypeList());
    EXPECT_EQ(factory.size(), 1);
    EXPECT_EQ(object_0_0, object_0_1);

    const auto object_1 = factory.get_or_create<ObjectImpl>("object_1", TypeList());
    EXPECT_EQ(factory.size(), 2);
    EXPECT_NE(object_0_0, object_1);
    EXPECT_EQ(object_1->get_identifier(), 1);
    EXPECT_EQ(object_1->get_name(), "object_1");
}

}
