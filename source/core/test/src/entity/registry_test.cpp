// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/entity/registry.hpp"

#include <gtest/gtest.h>

#include "tactile/base/container/array.hpp"
#include "tactile/base/container/hash_map.hpp"

namespace tactile {

/**
 * \trace tactile::Registry::make_entity
 */
TEST(Registry, MakeEntity)
{
  Registry registry {};

  const auto a = registry.make_entity();
  const auto b = registry.make_entity();
  const auto c = registry.make_entity();

  EXPECT_NE(a, b);
  EXPECT_NE(b, c);

  EXPECT_TRUE(registry.is_valid(a));
  EXPECT_TRUE(registry.is_valid(b));
  EXPECT_TRUE(registry.is_valid(c));
}

/**
 * \trace tactile::Registry::destroy
 * \trace tactile::Registry::is_valid
 */
TEST(Registry, Destroy)
{
  Registry registry {};

  const auto entity = registry.make_entity();
  EXPECT_TRUE(registry.is_valid(entity));

  EXPECT_TRUE(registry.destroy(entity));
  EXPECT_FALSE(registry.destroy(entity));

  EXPECT_FALSE(registry.is_valid(entity));
}

/**
 * \trace tactile::Registry::add
 * \trace tactile::Registry::has
 */
TEST(Registry, Add)
{
  Registry registry {};
  const auto a = registry.make_entity();
  const auto b = registry.make_entity();

  EXPECT_EQ(registry.add<int>(a, 42), 42);
  EXPECT_TRUE(registry.has<int>(a));
  EXPECT_FALSE(registry.has<int>(b));

  EXPECT_EQ(registry.add<int>(a, 15), 15);
  EXPECT_TRUE(registry.has<int>(a));
  EXPECT_FALSE(registry.has<int>(b));
}

/**
 * \trace tactile::Registry::erase
 */
TEST(Registry, Erase)
{
  Registry registry {};
  const auto entity = registry.make_entity();

  registry.add<int>(entity, 42);
  EXPECT_TRUE(registry.has<int>(entity));

  registry.erase<int>(entity);
  EXPECT_FALSE(registry.has<int>(entity));

  EXPECT_NO_THROW(registry.erase<int>(entity));
  EXPECT_TRUE(registry.is_valid(entity));
}

/**
 * \trace tactile::Registry::detach
 */
TEST(Registry, Detach)
{
  Registry registry {};
  const auto entity = registry.make_entity();

  registry.add<float>(entity, 1.5f);
  EXPECT_TRUE(registry.has<float>(entity));

  EXPECT_EQ(registry.detach<float>(entity), 1.5f);
  EXPECT_FALSE(registry.has<float>(entity));

  EXPECT_EQ(registry.detach<float>(entity), nothing);
}

/**
 * \trace tactile::Registry::get
 */
TEST(Registry, Get)
{
  Registry registry {};
  const auto a = registry.make_entity();
  const auto b = registry.make_entity();

  registry.add<int>(a, 10);
  registry.add<int>(b, 20);

  const auto& const_registry = registry;

  EXPECT_EQ(registry.get<int>(a), 10);
  EXPECT_EQ(registry.get<int>(b), 20);
  EXPECT_EQ(const_registry.get<int>(a), 10);
  EXPECT_EQ(const_registry.get<int>(b), 20);

  EXPECT_THROW((void) registry.get<float>(a), Exception);
  EXPECT_THROW((void) const_registry.get<float>(b), Exception);
}

/**
 * \trace tactile::Registry::find
 */
TEST(Registry, Find)
{
  Registry registry {};
  const auto a = registry.make_entity();
  const auto b = registry.make_entity();

  registry.add<float>(a, 5.0f);
  registry.add<float>(b, 8.0f);

  const auto& const_registry = registry;

  EXPECT_EQ(registry.find<float>(a), &registry.get<float>(a));
  EXPECT_EQ(registry.find<float>(b), &registry.get<float>(b));
  EXPECT_EQ(const_registry.find<float>(a), &const_registry.get<float>(a));
  EXPECT_EQ(const_registry.find<float>(b), &const_registry.get<float>(b));

  EXPECT_EQ(registry.find<bool>(a), nullptr);
  EXPECT_EQ(const_registry.find<bool>(b), nullptr);
}

/**
 * \trace tactile::Registry::each
 */
TEST(Registry, Each)
{
  Registry registry {};

  const Array<EntityID, 4> entities = {
    registry.make_entity(),
    registry.make_entity(),
    registry.make_entity(),
    registry.make_entity(),
  };

  registry.add<int>(entities[0], 1);
  registry.add<int>(entities[1], 2);
  registry.add<int>(entities[2], 3);
  registry.add<float>(entities[3], 4.0f);

  HashMap<EntityID, int> observed_ints {};

  for (auto [entity, value] : registry.each<int>()) {
    observed_ints[entity] = value;
  }

  ASSERT_TRUE(observed_ints.contains(entities[0]));
  ASSERT_TRUE(observed_ints.contains(entities[1]));
  ASSERT_TRUE(observed_ints.contains(entities[2]));
  ASSERT_FALSE(observed_ints.contains(entities[3]));
  EXPECT_EQ(observed_ints.at(entities[0]), 1);
  EXPECT_EQ(observed_ints.at(entities[1]), 2);
  EXPECT_EQ(observed_ints.at(entities[2]), 3);

  const auto& const_registry = registry;
  observed_ints.clear();

  for (auto [entity, value] : const_registry.each<int>()) {
    observed_ints[entity] = value;
  }

  ASSERT_TRUE(observed_ints.contains(entities[0]));
  ASSERT_TRUE(observed_ints.contains(entities[1]));
  ASSERT_TRUE(observed_ints.contains(entities[2]));
  ASSERT_FALSE(observed_ints.contains(entities[3]));
  EXPECT_EQ(observed_ints.at(entities[0]), 1);
  EXPECT_EQ(observed_ints.at(entities[1]), 2);
  EXPECT_EQ(observed_ints.at(entities[2]), 3);
}

/**
 * \trace tactile::Registry::count [T]
 */
TEST(Registry, CountForSpecificComponent)
{
  Registry registry {};
  EXPECT_EQ(registry.count<int>(), 0);

  registry.add<int>(registry.make_entity());
  registry.add<int>(registry.make_entity());

  EXPECT_EQ(registry.count<int>(), 2);
  EXPECT_EQ(registry.count<float>(), 0);
}

/**
 * \trace tactile::Registry::count
 */
TEST(Registry, Count)
{
  Registry registry {};
  EXPECT_EQ(registry.count(), 0);

  const auto a = registry.make_entity();
  const auto b = registry.make_entity();
  const auto c = registry.make_entity();
  const auto d = registry.make_entity();

  registry.add<int>(a);
  registry.add<int>(b);
  registry.add<int>(c);
  registry.add<int>(d);
  registry.add<float>(a);
  registry.add<float>(b);
  registry.add<bool>(c);

  EXPECT_EQ(registry.count(), 7);
}

}  // namespace tactile
