#include "core/systems/registry_system.hpp"

#include <gtest/gtest.h>

#include "core/components/attributes.hpp"
#include "core/components/layers.hpp"
#include "core/components/objects.hpp"
#include "core/components/tiles.hpp"
#include "core/components/tools.hpp"
#include "core/components/viewport.hpp"
#include "core/map.hpp"

using namespace tactile;

TEST(RegistrySystem, MakeDocumentRegistry)
{
  const auto registry = sys::make_document_registry();
  ASSERT_TRUE(registry.try_ctx<MapInfo>());
  ASSERT_TRUE(registry.try_ctx<comp::ActiveLayer>());
  ASSERT_TRUE(registry.try_ctx<comp::ActiveTileset>());
  ASSERT_TRUE(registry.try_ctx<comp::ActiveAttributeContext>());
  ASSERT_TRUE(registry.try_ctx<comp::ActiveTool>());
  ASSERT_TRUE(registry.try_ctx<comp::ActiveObject>());
  ASSERT_TRUE(registry.try_ctx<Viewport>());
  ASSERT_TRUE(registry.try_ctx<comp::AttributeContext>());
  ASSERT_TRUE(registry.try_ctx<comp::TilesetContext>());
}