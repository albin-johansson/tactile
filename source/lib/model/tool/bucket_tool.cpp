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

#include <centurion/mouse.hpp>

#include "components/document.hpp"
#include "core/map.hpp"
#include "core/tileset.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/layer_system.hpp"

namespace tactile {

void BucketTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void BucketTool::on_pressed(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);

    const auto selected_pos = attached_tileset.selection.value().begin;
    const auto replacement = attached_tileset.first_tile + tileset.index_of(selected_pos);

    dispatcher.enqueue<FloodEvent>(map.active_layer,
                                   mouse.position_in_viewport,
                                   replacement);
  }
}

auto BucketTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity == kNullEntity || !model.has<MapDocument>(document_entity)) {
    return false;
  }

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  if (map.active_layer == kNullEntity || !model.has<TileLayer>(map.active_layer)) {
    return false;
  }

  if (map.active_tileset != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    if (attached_tileset.selection.has_value()) {
      const auto selected_region =
          attached_tileset.selection->end - attached_tileset.selection->begin;
      return selected_region == TilePos {1, 1};
    }
  }

  return false;
}

}  // namespace tactile