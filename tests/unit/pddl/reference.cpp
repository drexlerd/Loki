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
#include <loki/details/pddl/reference.hpp>

namespace loki::domain::tests
{

TEST(LokiTests, ReferenceTest)
{
    PDDLFactory<ObjectImpl> factory(2);
    const auto object_0 = factory.get_or_create<ObjectImpl>("object_0", TypeList());
    const auto object_1 = factory.get_or_create<ObjectImpl>("object_1", TypeList());

    ReferencedPDDLObjects references;
    EXPECT_TRUE(!references.exists(object_0));
    EXPECT_TRUE(!references.exists(object_1));
    references.track(object_0);
    EXPECT_TRUE(references.exists(object_0));
    references.track(object_1);
    EXPECT_TRUE(references.exists(object_1));
    references.untrack(object_0);
    EXPECT_TRUE(!references.exists(object_0));
}

}
