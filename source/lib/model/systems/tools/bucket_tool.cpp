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

#include "components/document.hpp"
#include "components/layer.hpp"
#include "components/tileset.hpp"
#include "components/tool.hpp"
#include "model/event/tool_events.hpp"
#include "model/systems/document_system.hpp"

namespace tactile::sys {

auto create_bucket_tool(Model& model) -> Entity
{
  const auto bucket_tool_entity = model.create_entity();

  auto& bucket_tool = model.add<Tool>(bucket_tool_entity);
  bucket_tool.on_pressed = &on_bucket_tool_pressed;

  return bucket_tool_entity;
}

void on_bucket_tool_pressed(Model& model,
                            Entity,
                            const ViewportMouseInfo& mouse,
                            Dispatcher& dispatcher)
{
  if (mouse.in_viewport && mouse.button == MouseButton::Left &&
      is_bucket_tool_available(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);

    const auto selected_pos = attached_tileset.selection->begin;
    const auto new_tile_id = attached_tileset.first_tile + tileset.index_of(selected_pos);

    dispatcher.enqueue<FloodEvent>(map.active_layer, mouse.tile_pos, new_tile_id);
  }
}

auto is_bucket_tool_available(const Model& model) -> bool
{
  if (const auto* map = try_get_active_map(model)) {
    if (map->active_tileset != kNullEntity &&  //
        map->active_layer != kNullEntity &&    //
        model.has<TileLayer>(map->active_layer)) {
      const auto& attached_tileset = model.get<AttachedTileset>(map->active_tileset);
      return attached_tileset.is_single_tile_selected();
    }
  }

  return false;
}

}  // namespace tactile::sys
