// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/layer/layer.hpp"

#include <gtest/gtest.h>

#include "tactile/core/layer/layer_id.hpp"

namespace tactile {
namespace {

class LayerTest : public testing::Test
{
 protected:
  const LayerId m_id {1};
  Layer m_layer {m_id};
};

TEST_F(LayerTest, GetAndSetOpacity)
{
  EXPECT_EQ(m_layer.get_opacity(), 1.0f);

  m_layer.set_opacity(0.5f);
  EXPECT_EQ(m_layer.get_opacity(), 0.5f);

  m_layer.set_opacity(1.1f);
  EXPECT_EQ(m_layer.get_opacity(), 1.0f);

  m_layer.set_opacity(-0.1f);
  EXPECT_EQ(m_layer.get_opacity(), 0.0f);
}

TEST_F(LayerTest, GetAndSetVisibility)
{
  EXPECT_TRUE(m_layer.is_visible());

  m_layer.set_visible(false);
  EXPECT_FALSE(m_layer.is_visible());

  m_layer.set_visible(true);
  EXPECT_TRUE(m_layer.is_visible());
}

}  // namespace
}  // namespace tactile
