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

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/components/tiles.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/events/tool_events.hpp"
#include "core/model.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/tilesets/tileset_system.hpp"

namespace tactile {

void BucketTool::on_pressed(DocumentModel& model,
                            entt::dispatcher& dispatcher,
                            const MouseInfo& mouse)
{
  auto& registry = model.get_active_registry();
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      sys::is_tile_layer_active(registry) &&
      sys::is_single_tile_selected_in_tileset(registry)) {
    const auto& active = ctx_get<comp::ActiveState>(registry);

    const auto& selection = checked_get<comp::TilesetSelection>(registry, active.tileset);
    const auto& tilesetRef = checked_get<comp::TilesetRef>(registry, active.tileset);

    const auto& tileset = model.view_tileset(tilesetRef.source_tileset);
    const auto position = selection.region->begin;
    const auto replacement = tilesetRef.first_id + tileset.tile_at(position);

    dispatcher.enqueue<FloodEvent>(mouse.position_in_viewport, replacement);
  }
}

auto BucketTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  return sys::is_tile_layer_active(registry) &&
         sys::is_single_tile_selected_in_tileset(registry);
}

auto BucketTool::get_type() const -> ToolType
{
  return ToolType::Bucket;
}

}  // namespace tactile