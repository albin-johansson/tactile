// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/layer/dense_tile_layer.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/sparse_tile_layer.hpp"

namespace {

using namespace tactile;

using LayerTypes =
    testing::Types<DenseTileLayer, SparseTileLayer, ObjectLayer, GroupLayer>;

template <typename T>
class ILayerTest : public testing::Test {};

namespace {

template <typename T>
[[nodiscard]] auto _make_layer() -> T
{
  return T {};
}

template <>
[[nodiscard]] auto _make_layer<DenseTileLayer>() -> DenseTileLayer
{
  return DenseTileLayer {MatrixExtent {5, 5}};
}

template <>
[[nodiscard]] auto _make_layer<SparseTileLayer>() -> SparseTileLayer
{
  return SparseTileLayer {MatrixExtent {5, 5}};
}

}  // namespace

TYPED_TEST_SUITE(ILayerTest, LayerTypes);

/// \tests tactile::ILayer::set_persistent_id
/// \tests tactile::ILayer::get_persistent_id
TYPED_TEST(ILayerTest, PersistentId)
{
  auto layer = _make_layer<TypeParam>();
  EXPECT_FALSE(layer.get_persistent_id().has_value());

  layer.set_persistent_id(42);
  EXPECT_EQ(layer.get_persistent_id(), 42);

  layer.set_persistent_id(kNone);
  EXPECT_FALSE(layer.get_persistent_id().has_value());
}

/// \tests tactile::ILayer::set_opacity
/// \tests tactile::ILayer::get_opacity
TYPED_TEST(ILayerTest, Opacity)
{
  auto layer = _make_layer<TypeParam>();
  EXPECT_EQ(layer.get_opacity(), 1.0f);

  layer.set_opacity(0.5f);
  EXPECT_EQ(layer.get_opacity(), 0.5f);

  layer.set_opacity(-0.1f);
  EXPECT_EQ(layer.get_opacity(), 0.0f);

  layer.set_opacity(1.1f);
  EXPECT_EQ(layer.get_opacity(), 1.0f);
}

/// \tests tactile::ILayer::set_visible
/// \tests tactile::ILayer::is_visible
TYPED_TEST(ILayerTest, Visibility)
{
  auto layer = _make_layer<TypeParam>();
  EXPECT_TRUE(layer.is_visible());

  layer.set_visible(false);
  EXPECT_FALSE(layer.is_visible());

  layer.set_visible(true);
  EXPECT_TRUE(layer.is_visible());
}

/// \tests tactile::ILayer::clone
TYPED_TEST(ILayerTest, Clone)
{
  const int32 id {827};
  const float opacity {0.8f};
  const bool visible {false};

  auto source_layer = _make_layer<TypeParam>();
  source_layer.set_persistent_id(id);
  source_layer.set_opacity(opacity);
  source_layer.set_visible(visible);

  const auto layer_clone = source_layer.clone();

  EXPECT_EQ(source_layer.get_persistent_id(), id);
  EXPECT_EQ(source_layer.get_opacity(), opacity);
  EXPECT_EQ(source_layer.is_visible(), visible);

  EXPECT_FALSE(layer_clone->get_persistent_id().has_value());
  EXPECT_EQ(layer_clone->get_opacity(), source_layer.get_opacity());
  EXPECT_EQ(layer_clone->is_visible(), source_layer.is_visible());

  EXPECT_NE(layer_clone->meta().uuid(), source_layer.meta().uuid());
}

}  // namespace
