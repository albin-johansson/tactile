#include "core/systems/viewport_system.hpp"

#include <gtest/gtest.h>

#include "core/components/viewport.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

class ViewportSystemTest : public testing::Test
{
 protected:
  void SetUp() override { mRegistry = sys::new_map_document_registry(); }

  entt::registry mRegistry;
};

TEST_F(ViewportSystemTest, OffsetViewport)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();
  ASSERT_EQ(0, viewport.offset.x);
  ASSERT_EQ(0, viewport.offset.y);

  sys::offset_viewport(mRegistry, {42.5f, 27.3f});
  ASSERT_EQ(42.5f, viewport.offset.x);
  ASSERT_EQ(27.3f, viewport.offset.y);

  sys::offset_viewport(mRegistry, {-12.0f, -1.9f});
  ASSERT_EQ(42.5f - 12.0f, viewport.offset.x);
  ASSERT_EQ(27.3f - 1.9f, viewport.offset.y);
}

TEST_F(ViewportSystemTest, PanViewportLeft)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();
  ASSERT_GT(viewport.tile_size.x, 0);
  ASSERT_GT(viewport.tile_size.y, 0);

  sys::pan_viewport_left(mRegistry);
  ASSERT_EQ(viewport.tile_size.x, viewport.offset.x);
  ASSERT_EQ(0, viewport.offset.y);
}

TEST_F(ViewportSystemTest, PanViewportRight)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();
  ASSERT_GT(viewport.tile_size.x, 0);
  ASSERT_GT(viewport.tile_size.y, 0);

  sys::pan_viewport_right(mRegistry);
  ASSERT_EQ(-viewport.tile_size.x, viewport.offset.x);
  ASSERT_EQ(0, viewport.offset.y);
}

TEST_F(ViewportSystemTest, PanViewportUp)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();
  ASSERT_GT(viewport.tile_size.x, 0);
  ASSERT_GT(viewport.tile_size.y, 0);

  sys::pan_viewport_up(mRegistry);
  ASSERT_EQ(0, viewport.offset.x);
  ASSERT_EQ(viewport.tile_size.y, viewport.offset.y);
}

TEST_F(ViewportSystemTest, PanViewportDown)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();
  ASSERT_GT(viewport.tile_size.x, 0);
  ASSERT_GT(viewport.tile_size.y, 0);

  sys::pan_viewport_down(mRegistry);
  ASSERT_EQ(0, viewport.offset.x);
  ASSERT_EQ(-viewport.tile_size.y, viewport.offset.y);
}

TEST_F(ViewportSystemTest, DecreaseViewportZoom)
{
  const auto& viewport = mRegistry.ctx().at<comp::Viewport>();

  const auto width = viewport.tile_size.x;
  const auto height = viewport.tile_size.y;

  sys::decrease_viewport_zoom(mRegistry, {0, 0});

  ASSERT_LT(viewport.tile_size.x, width);
  ASSERT_LT(viewport.tile_size.y, height);
}