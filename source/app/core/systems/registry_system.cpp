/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "registry_system.hpp"

#include "context_system.hpp"
#include "core/components/attributes.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/tiles.hpp"
#include "core/components/tool.hpp"
#include "core/map.hpp"
#include "core/viewport.hpp"
#include "property_system.hpp"

namespace tactile::sys {

auto make_document_registry() -> entt::registry
{
  entt::registry registry;

  registry.set<comp::ActiveLayer>();
  registry.set<comp::ActiveTileset>();
  registry.set<comp::ActiveAttributeContext>();
  registry.set<comp::active_tool>();
  registry.set<comp::active_object>();

  auto& map = registry.set<MapInfo>();
  map.row_count = 5;
  map.column_count = 5;

  auto& viewport = registry.set<Viewport>();
  viewport.tile_width = 64;
  viewport.tile_height = 64;
  viewport.x_offset = 0;
  viewport.y_offset = 0;

  auto& context = registry.set<comp::AttributeContext>();
  context.id = get_and_update_next_context_id();
  context.name = "Map";

  auto& tilesets = registry.set<comp::TilesetContext>();
  tilesets.next_id = 1;
  tilesets.next_tile_id = 1;

  return registry;
}

}  // namespace tactile::sys
