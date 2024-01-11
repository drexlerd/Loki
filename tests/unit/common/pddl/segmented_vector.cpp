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

#include <loki/common/pddl/segmented_vector.hpp>

#include <gtest/gtest.h>


namespace loki::domain::tests {

TEST(LokiTests, SegmentedVectorTest) {
    SegmentedVector<int, 2> vec;
    EXPECT_EQ(vec.size(), 0);
    EXPECT_EQ(vec.capacity(), 0);

    vec.push_back(2);
    EXPECT_EQ(vec.size(), 1);
    EXPECT_EQ(vec.capacity(), 2);
    EXPECT_EQ(vec[0], 2);

    vec.push_back(1);
    EXPECT_EQ(vec.size(), 2);
    EXPECT_EQ(vec[1], 1);
    EXPECT_EQ(vec.capacity(), 2);

    vec.push_back(0);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[2], 0);
    EXPECT_EQ(vec.capacity(), 4);
}

}
