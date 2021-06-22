#include <gtest/gtest.h>

#include "core/map/layers/tile_layer.hpp"
#include "gtest_macros.hpp"

using namespace tactile;

// TODO test layer APIs for both TileLayer and ObjectLayer

using LayerTypes = testing::Types<TileLayer>;

template <typename T>
class LayerTest : public testing::Test
{};

TYPED_TEST_SUITE(LayerTest, LayerTypes);

TYPED_TEST(LayerTest, Defaults)
{
  const TypeParam layer;
  ASSERT_EQ(1.0f, layer.GetOpacity());
  ASSERT_TRUE(layer.IsVisible());
}

TYPED_TEST(LayerTest, SetVisible)
{
  TypeParam layer;

  layer.SetVisible(false);
  ASSERT_FALSE(layer.IsVisible());

  layer.SetVisible(true);
  ASSERT_TRUE(layer.IsVisible());
}

TYPED_TEST(LayerTest, SetOpacity)
{
  TypeParam layer;

  // Underflow opacity
  ASSERT_NO_THROW(layer.SetOpacity(-1));
  ASSERT_EQ(0, layer.GetOpacity());

  // OK opacity
  ASSERT_NO_THROW(layer.SetOpacity(0.5f));
  ASSERT_EQ(0.5f, layer.GetOpacity());

  // Overflow opacity
  ASSERT_NO_THROW(layer.SetOpacity(1.2f));
  ASSERT_EQ(1.0f, layer.GetOpacity());
}

TYPED_TEST(LayerTest, SetName)
{
  TypeParam layer;

  layer.SetName("foo");
  ASSERT_EQ("foo", layer.GetName());
}

TYPED_TEST(LayerTest, Clone)
{
  const auto opacity = 0.4f;
  const auto isVisible = false;
  const auto name = "foo";

  TypeParam layer;
  layer.SetOpacity(opacity);
  layer.SetVisible(isVisible);
  layer.SetName(name);

  const auto clone = layer.Clone();
  ASSERT_EQ(opacity, clone->GetOpacity());
  ASSERT_EQ(isVisible, clone->IsVisible());
  ASSERT_EQ(name, clone->GetName());
}
