#include "core/systems/registry_system.hpp"

#include <gtest/gtest.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/map_info.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/components/tools.hpp"
#include "core/components/viewport.hpp"
#include "core/tools/tool_manager.hpp"

using namespace tactile;

TEST(RegistrySystem, NewMapDocumentRegistry)
{
  const auto registry = sys::new_map_document_registry();
  const auto& ctx = registry.ctx();
  ASSERT_TRUE(ctx.find<ToolManager>());
  ASSERT_TRUE(ctx.find<comp::MapInfo>());
  ASSERT_TRUE(ctx.find<comp::ActiveLayer>());
  ASSERT_TRUE(ctx.find<comp::ActiveTileset>());
  ASSERT_TRUE(ctx.find<comp::ActiveContext>());
  ASSERT_TRUE(ctx.find<comp::ActiveObject>());
  ASSERT_TRUE(ctx.find<comp::Viewport>());
  ASSERT_TRUE(ctx.find<comp::Context>());
  ASSERT_TRUE(ctx.find<comp::TilesetContext>());
}