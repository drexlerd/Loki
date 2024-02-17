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

#include <loki/common/pddl/persistent_factory.hpp>
#include <loki/common/pddl/reference.hpp>
#include <loki/domain/pddl/object.hpp>

#include <gtest/gtest.h>


namespace loki::domain::tests {

TEST(LokiTests, ReferenceTest) {
    PersistentFactory<pddl::ObjectImpl, 2> factory;
    const auto object_0 = factory.get_or_create<pddl::ObjectImpl>("object_0", pddl::TypeList());
    const auto object_1 = factory.get_or_create<pddl::ObjectImpl>("object_1", pddl::TypeList());

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
