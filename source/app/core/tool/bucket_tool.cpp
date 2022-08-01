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

#include "bucket_tool.hpp"

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/document/map_document.hpp"
#include "core/document/tileset_document.hpp"
#include "core/event/tool_events.hpp"
#include "core/model.hpp"

namespace tactile {

void BucketTool::accept(IToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void BucketTool::on_pressed(DocumentModel&    model,
                            entt::dispatcher& dispatcher,
                            const MouseInfo&  mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto& tilesets = map.get_tilesets();

    const auto  tilesetId = tilesets.active_tileset_id().value();
    const auto& tilesetRef = tilesets.get_ref(tilesetId);
    const auto& tileset = tilesetRef.view_tileset();

    const auto selectedPos = tilesetRef.get_selection()->begin;
    const auto replacement = tilesetRef.first_tile() + tileset.index_of(selectedPos);

    const auto layerId = map.active_layer_id().value();
    dispatcher.enqueue<FloodEvent>(layerId, mouse.position_in_viewport, replacement);
  }
}

auto BucketTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  const auto& tilesets = map.get_tilesets();
  const auto  tilesetId = tilesets.active_tileset_id();

  return map.is_active_layer(LayerType::TileLayer) &&  //
         tilesetId && tilesets.get_ref(*tilesetId).is_single_tile_selected();
}

}  // namespace tactile