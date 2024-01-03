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

#include "../../../include/loki/common/pddl/garbage_collected_factory.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"

#include <gtest/gtest.h>

#include <string>


namespace loki::domain::tests {
/*
class Object {
private:
    int m_identifier;
    std::string m_name;

    Object(int identifier, std::string name) : m_identifier(identifier), m_name(std::move(name)) { }

    template<typename... Ts>
    friend class GarbageCollectedFactory;

public:
    int get_identifier() const { return m_identifier; }
    const std::string& get_name() const { return m_name; }
};

TEST(LokiTests, GarbageCollectedFactoryTest) {
    GarbageCollectedFactory<Object> factory;
    EXPECT_EQ(factory.size<Object>(), 0);

    {
        const auto object_0 = factory.get_or_create<Object>("object_0");
        EXPECT_EQ(factory.size<Object>(), 1);
        EXPECT_EQ(object_0->get_name(), "object_0");
        // destructor is called
    }
    EXPECT_EQ(factory.size<Object>(), 0);
}
*/
}
