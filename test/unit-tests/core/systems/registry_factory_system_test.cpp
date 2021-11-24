#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/mouse_pos.hpp"
#include "core/systems/registry_system.hpp"
#include "core/viewport.hpp"

using namespace Tactile;

TEST(RegistryFactorySystem, MakeRegistry)
{
  const auto registry = Sys::MakeRegistry();
  ASSERT_TRUE(registry.try_ctx<Map>());
  ASSERT_TRUE(registry.try_ctx<ActiveLayer>());
  ASSERT_TRUE(registry.try_ctx<ActiveTileset>());
  ASSERT_TRUE(registry.try_ctx<ActivePropertyContext>());
  ASSERT_TRUE(registry.try_ctx<Viewport>());
  ASSERT_TRUE(registry.try_ctx<PropertyContext>());
  ASSERT_TRUE(registry.try_ctx<TilesetContext>());
  ASSERT_TRUE(registry.try_ctx<MousePos>());
}