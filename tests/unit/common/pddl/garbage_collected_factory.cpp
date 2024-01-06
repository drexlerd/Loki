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
#include "../../../include/loki/common/hash.hpp"
#include "../../../include/loki/domain/pddl/object.hpp"

#include <gtest/gtest.h>

#include <string>


namespace loki::domain::tests {

/// @brief A type to avoid friending in the pddl::Object since this class is not used.
class ObjectImpl {
private:
    int m_identifier;
    std::string m_name;

    ObjectImpl(int identifier, std::string name) : m_identifier(identifier), m_name(std::move(name)) { }

    template<typename... Ts>
    friend class loki::GarbageCollectedFactory;

public:
    bool operator==(const ObjectImpl& other) const {
        return m_name == other.m_name;
    }

    size_t hash() const {
        return hash_combine(m_name);
    }

    int get_identifier() const { return m_identifier; }
    const std::string& get_name() const { return m_name; }
};

using Object = std::shared_ptr<ObjectImpl>;

}


namespace std {
    template<>
    struct hash<loki::domain::tests::ObjectImpl> {
        size_t operator()(const loki::domain::tests::ObjectImpl& object) const {
            return object.hash();
        }
    };
}


namespace loki::domain::tests {

TEST(LokiTests, GarbageCollectedFactoryTest) {
    GarbageCollectedFactory<ObjectImpl> factory;
    EXPECT_EQ(factory.size<ObjectImpl>(), 0);

    {
        const auto object_0_0 = factory.get_or_create<ObjectImpl>("object_0");
        EXPECT_EQ(factory.size<ObjectImpl>(), 1);
        EXPECT_EQ(object_0_0->get_name(), "object_0");

        const auto object_0_1 = factory.get_or_create<ObjectImpl>("object_0");
        EXPECT_EQ(factory.size<ObjectImpl>(), 1);
        EXPECT_EQ(object_0_0, object_0_1);

        const auto object_1 = factory.get_or_create<ObjectImpl>("object_1");
        EXPECT_EQ(factory.size<ObjectImpl>(), 2);

        // destructors are called
    }
    EXPECT_EQ(factory.size<ObjectImpl>(), 0);
}

}
