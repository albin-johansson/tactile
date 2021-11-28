#include "registry_system.hpp"

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "core/components/tileset.hpp"
#include "core/components/tool.hpp"
#include "core/map.hpp"
#include "core/mouse_pos.hpp"
#include "core/viewport.hpp"

namespace Tactile::Sys {

auto MakeRegistry() -> entt::registry
{
  entt::registry registry;

  registry.set<ActiveLayer>();
  registry.set<ActiveTileset>();
  registry.set<ActivePropertyContext>();
  registry.set<ActiveTool>();
  registry.set<ActiveObject>();
  registry.set<MousePos>();

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

  auto& tilesets = registry.set<TilesetContext>();
  tilesets.next_id = 1;
  tilesets.next_tile_id = 1;

  return registry;
}

}  // namespace Tactile::Sys