#include "core/systems/registry_system.hpp"

#include <gtest/gtest.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/components/tools.hpp"
#include "core/components/viewport.hpp"
#include "core/map_info.hpp"

using namespace tactile;

TEST(RegistrySystem, MakeDocumentRegistry)
{
  const auto registry = sys::make_document_registry();
  const auto& ctx = registry.ctx();
  ASSERT_TRUE(ctx.find<MapInfo>());
  ASSERT_TRUE(ctx.find<comp::ActiveLayer>());
  ASSERT_TRUE(ctx.find<comp::ActiveTileset>());
  ASSERT_TRUE(ctx.find<comp::ActiveAttributeContext>());
  ASSERT_TRUE(ctx.find<comp::ActiveTool>());
  ASSERT_TRUE(ctx.find<comp::ActiveObject>());
  ASSERT_TRUE(ctx.find<comp::Viewport>());
  ASSERT_TRUE(ctx.find<comp::AttributeContext>());
  ASSERT_TRUE(ctx.find<comp::TilesetContext>());
}