/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "bucket_tool.hpp"

#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/tool_events.hpp"
#include "model/layers/layer_components.hpp"
#include "model/registry.hpp"
#include "model/tilesets/attached_tileset_ops.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"

namespace tactile {

void BucketTool::on_mouse_pressed(Registry& registry,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(registry)) {
    const auto document_entity = sys::get_active_document(registry);

    const auto& map_document = registry.get<MapDocument>(document_entity);
    const auto& map = registry.get<Map>(map_document.map);

    const auto& attached_tileset = registry.get<AttachedTileset>(map.active_tileset);
    const auto& tileset = registry.get<Tileset>(attached_tileset.tileset);

    const auto selected_pos = attached_tileset.selection->begin;
    const auto new_tile_id =
        attached_tileset.first_tile + sys::tile_index_at(tileset, selected_pos);

    dispatcher.enqueue<FloodEvent>(map.active_layer, mouse.tile_pos, new_tile_id);
  }
}

auto BucketTool::is_available(const Registry& registry) const -> bool
{
  return sys::is_bucket_tool_available(registry);
}

namespace sys {

auto is_bucket_tool_available(const Registry& registry) -> bool
{
  if (const auto* map = try_get_active_map(registry)) {
    if (map->active_tileset != kNullEntity &&  //
        map->active_layer != kNullEntity &&    //
        registry.has<TileLayer>(map->active_layer)) {
      const auto& attached_tileset = registry.get<AttachedTileset>(map->active_tileset);
      return is_single_tile_selected(attached_tileset);
    }
  }

  return false;
}

}  // namespace sys
}  // namespace tactile
