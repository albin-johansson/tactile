// Copyright (C) 2025 Albin Johansson

#include <gmock/gmock.h>
#include <gtest/gtest.h>

import tactile.core.layer;

namespace tactile::tests {
namespace {

using testing::Const;

using LayerTypes = testing::Types<GroupLayer, TileLayer, AnnotationLayer>;

template <typename T>
class LayerTest : public testing::Test
{};

TYPED_TEST_SUITE(LayerTest, LayerTypes);

TYPED_TEST(LayerTest, Info)
{
  constexpr LayerID id {42};
  TypeParam layer {id};

  EXPECT_EQ(layer.info().id(), id);
  EXPECT_EQ(layer.info().opacity(), 1.0f);
  EXPECT_TRUE(layer.info().visible());

  EXPECT_EQ(Const(layer).info().id(), id);
  EXPECT_EQ(Const(layer).info().opacity(), 1.0f);
  EXPECT_TRUE(Const(layer).info().visible());
}

}  // namespace
}  // namespace tactile::tests
