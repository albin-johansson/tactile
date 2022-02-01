#include "core/systems/registry_system.hpp"

#include <gtest/gtest.h>

#include "core/components/attribute_context.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/tileset.hpp"
#include "core/components/tool.hpp"
#include "core/map.hpp"
#include "core/viewport.hpp"

using namespace tactile;

TEST(RegistrySystem, MakeDocumentRegistry)
{
  const auto registry = sys::make_document_registry();
  ASSERT_TRUE(registry.try_ctx<MapInfo>());
  ASSERT_TRUE(registry.try_ctx<comp::active_layer>());
  ASSERT_TRUE(registry.try_ctx<comp::active_tileset>());
  ASSERT_TRUE(registry.try_ctx<comp::active_attribute_context>());
  ASSERT_TRUE(registry.try_ctx<comp::active_tool>());
  ASSERT_TRUE(registry.try_ctx<comp::active_object>());
  ASSERT_TRUE(registry.try_ctx<Viewport>());
  ASSERT_TRUE(registry.try_ctx<comp::attribute_context>());
  ASSERT_TRUE(registry.try_ctx<comp::tileset_context>());
}