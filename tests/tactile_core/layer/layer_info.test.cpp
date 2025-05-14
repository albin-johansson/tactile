// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import tactile.layer;

namespace tactile::tests {
namespace {

class LayerInfoTest : public testing::Test
{};

TEST_F(LayerInfoTest, Defaults)
{
  constexpr LayerID id {42};
  const LayerInfo info {id};

  EXPECT_EQ(info.id(), id);
  EXPECT_EQ(info.opacity(), 1.0f);
  EXPECT_TRUE(info.visible());
}

TEST_F(LayerInfoTest, SetOpacity)
{
  LayerInfo info {LayerID {42}};

  info.set_opacity(0.5f);
  EXPECT_EQ(info.opacity(), 0.5f);

  info.set_opacity(1.1f);
  EXPECT_EQ(info.opacity(), 1.0f);

  info.set_opacity(-0.1f);
  EXPECT_EQ(info.opacity(), 0.0f);
}

TEST_F(LayerInfoTest, SetVisible)
{
  LayerInfo info {LayerID {42}};

  info.set_visible(false);
  EXPECT_FALSE(info.visible());

  info.set_visible(true);
  EXPECT_TRUE(info.visible());
}

}  // namespace
}  // namespace tactile::tests
