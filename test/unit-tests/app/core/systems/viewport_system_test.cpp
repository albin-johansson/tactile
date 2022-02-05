#include "core/systems/viewport_system.hpp"

#include <gtest/gtest.h>

#include "core/systems/registry_system.hpp"
#include "core/viewport.hpp"

using namespace tactile;

class ViewportSystemTest : public testing::Test
{
 protected:
  void SetUp() override { mRegistry = sys::make_document_registry(); }

  entt::registry mRegistry;
};

TEST_F(ViewportSystemTest, OffsetViewport)
{
  const auto& viewport = mRegistry.ctx<Viewport>();
  ASSERT_EQ(0, viewport.x_offset);
  ASSERT_EQ(0, viewport.y_offset);

  sys::OffsetViewport(mRegistry, 42.5f, 27.3f);
  ASSERT_EQ(42.5f, viewport.x_offset);
  ASSERT_EQ(27.3f, viewport.y_offset);

  sys::OffsetViewport(mRegistry, -12.0f, -1.9f);
  ASSERT_EQ(42.5f - 12.0f, viewport.x_offset);
  ASSERT_EQ(27.3f - 1.9f, viewport.y_offset);
}

TEST_F(ViewportSystemTest, PanViewportLeft)
{
  const auto& viewport = mRegistry.ctx<Viewport>();
  ASSERT_GT(viewport.tile_width, 0);
  ASSERT_GT(viewport.tile_height, 0);

  sys::PanViewportLeft(mRegistry);
  ASSERT_EQ(viewport.tile_width, viewport.x_offset);
  ASSERT_EQ(0, viewport.y_offset);
}

TEST_F(ViewportSystemTest, PanViewportRight)
{
  const auto& viewport = mRegistry.ctx<Viewport>();
  ASSERT_GT(viewport.tile_width, 0);
  ASSERT_GT(viewport.tile_height, 0);

  sys::PanViewportRight(mRegistry);
  ASSERT_EQ(-viewport.tile_width, viewport.x_offset);
  ASSERT_EQ(0, viewport.y_offset);
}

TEST_F(ViewportSystemTest, PanViewportUp)
{
  const auto& viewport = mRegistry.ctx<Viewport>();
  ASSERT_GT(viewport.tile_width, 0);
  ASSERT_GT(viewport.tile_height, 0);

  sys::PanViewportUp(mRegistry);
  ASSERT_EQ(0, viewport.x_offset);
  ASSERT_EQ(viewport.tile_height, viewport.y_offset);
}

TEST_F(ViewportSystemTest, PanViewportDown)
{
  const auto& viewport = mRegistry.ctx<Viewport>();
  ASSERT_GT(viewport.tile_width, 0);
  ASSERT_GT(viewport.tile_height, 0);

  sys::PanViewportDown(mRegistry);
  ASSERT_EQ(0, viewport.x_offset);
  ASSERT_EQ(-viewport.tile_height, viewport.y_offset);
}

TEST_F(ViewportSystemTest, DecreaseViewportZoom)
{
  const auto& viewport = mRegistry.ctx<Viewport>();

  const auto width = viewport.tile_width;
  const auto height = viewport.tile_height;

  sys::DecreaseViewportZoom(mRegistry, 0, 0);

  ASSERT_LT(viewport.tile_width, width);
  ASSERT_LT(viewport.tile_height, height);
}