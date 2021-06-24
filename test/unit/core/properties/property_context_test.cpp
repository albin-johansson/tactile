#include "core/properties/property_context.hpp"

#include <gtest/gtest.h>

#include "core/map/layers/object.hpp"
#include "core/map/layers/object_layer.hpp"
#include "core/map/layers/tile_layer.hpp"
#include "core/map_document.hpp"
#include "core/properties/property_delegate.hpp"
#include "core/tileset/tileset.hpp"

using namespace tactile;

using PropertyContextTypes = testing::Types<PropertyDelegate,
                                            MapDocument,
                                            TileLayer,
                                            ObjectLayer,
                                            Object,
                                            Tileset>;

template <typename T>
class PropertyContextTest : public testing::Test
{};

template <typename T>
auto MakeContext() -> T
{
  return T{};
}

template <>
inline auto MakeContext() -> Object
{
  return Object{ObjectType::Rectangle};
}

template <>
inline auto MakeContext() -> Tileset
{
  return Tileset{1_tile, "resources/terrain.png", 32, 32};
}

TYPED_TEST_SUITE(PropertyContextTest, PropertyContextTypes);

TYPED_TEST(PropertyContextTest, Defaults)
{
  auto context = MakeContext<TypeParam>();

  ASSERT_EQ(0, context.GetPropertyCount());
  ASSERT_TRUE(context.GetProperties().empty());
}

TYPED_TEST(PropertyContextTest, AddProperty)
{
  auto context = MakeContext<TypeParam>();
  ASSERT_FALSE(context.HasProperty("foo"));
  ASSERT_EQ(0, context.GetPropertyCount());

  context.AddProperty("foo", PropertyType::Integer);
  ASSERT_TRUE(context.HasProperty("foo"));
  ASSERT_EQ(1, context.GetPropertyCount());

  const auto& property = context.GetProperty("foo");
  ASSERT_TRUE(property.IsInteger());
  ASSERT_EQ(0, property.AsInteger());
}

TYPED_TEST(PropertyContextTest, RemoveProperty)
{
  auto context = MakeContext<TypeParam>();

  context.AddProperty("foo", PropertyType::Integer);
  ASSERT_TRUE(context.HasProperty("foo"));
  ASSERT_EQ(1, context.GetPropertyCount());

  context.RemoveProperty("foo");
  ASSERT_FALSE(context.HasProperty("foo"));
  ASSERT_EQ(0, context.GetPropertyCount());
}

TYPED_TEST(PropertyContextTest, RenameProperty)
{
  auto context = MakeContext<TypeParam>();

  const auto oldName = "foo";
  const auto newName = "bar";

  context.AddProperty(oldName, PropertyType::Boolean);
  ASSERT_TRUE(context.HasProperty(oldName));
  ASSERT_FALSE(context.HasProperty(newName));

  ASSERT_NO_THROW(context.RenameProperty(oldName, newName));
  ASSERT_FALSE(context.HasProperty(oldName));
  ASSERT_TRUE(context.HasProperty(newName));

  const auto& property = context.GetProperty(newName);
  ASSERT_TRUE(property.IsBoolean());
}

TYPED_TEST(PropertyContextTest, SetProperty)
{
  auto context = MakeContext<TypeParam>();

  const auto name = "foo";

  context.AddProperty(name, PropertyType::Integer);
  ASSERT_EQ(0, context.GetProperty(name).AsInteger());

  context.SetProperty(name, 123);
  ASSERT_EQ(123, context.GetProperty(name).AsInteger());
}

TYPED_TEST(PropertyContextTest, GetProperty)
{
  auto context = MakeContext<TypeParam>();

  const auto name = "foo";
  ASSERT_ANY_THROW(context.GetProperty(name));

  context.AddProperty(name, PropertyType::String);
  ASSERT_NO_THROW(context.GetProperty(name));
}

TYPED_TEST(PropertyContextTest, GetPropertyCount)
{
  auto context = MakeContext<TypeParam>();
  ASSERT_EQ(0, context.GetPropertyCount());

  context.AddProperty("foo", PropertyType::String);
  ASSERT_EQ(1, context.GetPropertyCount());

  context.AddProperty("bar", PropertyType::Integer);
  ASSERT_EQ(2, context.GetPropertyCount());

  context.RemoveProperty("bar");
  ASSERT_EQ(1, context.GetPropertyCount());

  context.RemoveProperty("foo");
  ASSERT_EQ(0, context.GetPropertyCount());
}
