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
#include <loki/details/utils/equal_to.hpp>
#include <loki/details/utils/hash.hpp>
#include <unordered_set>

namespace loki::domain::tests
{

TEST(LokiTests, LokiUtilsHashTest)
{
    struct NumericConstraintSplitDistribution
    {
        size_t num_true_elements;
        size_t num_dont_care_elements;

        auto identifying_members() const { return std::forward_as_tuple(std::as_const(num_true_elements), std::as_const(num_dont_care_elements)); }
    };

    struct NumericConstraintSplit
    {
        NumericConstraintSplitDistribution distribution;

        auto identifying_members() const { return std::forward_as_tuple(std::as_const(distribution)); }
    };

    using Split = std::variant<NumericConstraintSplit, int>;

    auto set = std::unordered_set<Split, loki::Hash<Split>, loki::EqualTo<Split>> {};

    auto split = NumericConstraintSplit {};
    set.insert(split);
}
}