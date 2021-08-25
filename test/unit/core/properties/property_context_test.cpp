#include "core/properties/property_context.hpp"

#include <gtest/gtest.h>

#include "core/map/layers/object.hpp"
#include "core/map/layers/object_layer.hpp"
#include "core/map/layers/tile_layer.hpp"
#include "core/map_document.hpp"
#include "core/properties/property_delegate.hpp"
#include "core/tileset/fancy_tile.hpp"
#include "core/tileset/tileset.hpp"
#include "utils/load_texture.hpp"

using namespace Tactile;

// clang-format off

using PropertyContextTypes = testing::Types<PropertyDelegate,
                                            MapDocument,
                                            TileLayer,
                                            ObjectLayer,
                                            Object,
                                            Tileset,
                                            FancyTile>;
// clang-format on

inline Maybe<uint> texture_id;

template <typename T>
class PropertyContextTest : public testing::Test
{
 public:
  static void TearDownTestSuite()
  {
    if (texture_id)
    {
      glDeleteTextures(1, &texture_id.value());
    }
  }
};

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
  static const auto info = LoadTexture("resources/terrain.png");

  assert(info.has_value());
  texture_id = info->texture;

  return Tileset{1_tile, *info, 32, 32};
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
  ASSERT_TRUE(property.IsInt());
  ASSERT_EQ(0, property.AsInt());
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
  ASSERT_TRUE(property.IsBool());
}

TYPED_TEST(PropertyContextTest, SetProperty)
{
  auto context = MakeContext<TypeParam>();

  const auto name = "foo";

  context.AddProperty(name, PropertyType::Integer);
  ASSERT_EQ(0, context.GetProperty(name).AsInt());

  context.SetProperty(name, 123);
  ASSERT_EQ(123, context.GetProperty(name).AsInt());
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
