#include "registry_factory_system.hpp"

#include "tactile/core/components/layer.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/components/tileset.hpp"
#include "tactile/core/components/tool.hpp"
#include "tactile/core/map.hpp"
#include "tactile/core/mouse.hpp"
#include "tactile/core/viewport.hpp"

namespace Tactile::Sys {

auto MakeRegistry() -> entt::registry
{
  entt::registry registry;

  registry.set<ActiveLayer>();
  registry.set<ActiveTileset>();
  registry.set<ActivePropertyContext>();
  registry.set<ActiveTool>();
  registry.set<Mouse>();

  auto& map = registry.set<Map>();
  map.row_count = 5;
  map.column_count = 5;

  auto& viewport = registry.set<Viewport>();
  viewport.tile_width = 64;
  viewport.tile_height = 64;
  viewport.x_offset = 0;
  viewport.y_offset = 0;

  auto& context = registry.set<PropertyContext>();
  context.name = "Map";

  registry.set<TilesetContext>(1_tset, 1_tile);

  return registry;
}

}  // namespace Tactile::Sys
