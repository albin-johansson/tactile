// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/object.hpp"

#include <gtest/gtest.h>

#include "tactile/core/entity/registry.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

/**
 * \trace tactile::is_object
 */
TEST(Object, IsObject)
{
  Registry registry {};

  EXPECT_FALSE(is_object(registry, kInvalidEntity));
  EXPECT_FALSE(is_object(registry, registry.make_entity()));

  const auto entity = registry.make_entity();
  registry.add<CMeta>(entity);
  registry.add<CObject>(entity);

  EXPECT_TRUE(is_object(registry, entity));
}

/**
 * \trace tactile::make_object
 */
TEST(Object, MakeObject)
{
  Registry registry {};

  const ObjectID object_id {42};
  const auto object_type = ObjectType::kRect;
  const auto object_entity = make_object(registry, object_id, object_type);

  ASSERT_TRUE(is_object(registry, object_entity));
  ASSERT_TRUE(registry.has<CMeta>(object_entity));
  ASSERT_TRUE(registry.has<CObject>(object_entity));

  const auto& meta = registry.get<CMeta>(object_entity);
  const auto& object = registry.get<CObject>(object_entity);

  EXPECT_EQ(meta.name, "");
  EXPECT_EQ(meta.properties.size(), 0);

  EXPECT_EQ(object.id, object_id);
  EXPECT_EQ(object.type, object_type);
  EXPECT_EQ(object.position.x(), 0.0f);
  EXPECT_EQ(object.position.y(), 0.0f);
  EXPECT_EQ(object.size.x(), 0.0f);
  EXPECT_EQ(object.size.y(), 0.0f);
  EXPECT_EQ(object.tag, "");
  EXPECT_EQ(object.is_visible, true);
}

}  // namespace tactile
