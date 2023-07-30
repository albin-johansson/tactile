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

#include "eraser_tool.hpp"

#include <utility>  // move

#include "cmd/tool/eraser_sequence.hpp"
#include "model/documents/command_system.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/tile_layer_ops.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile {

void EraserTool::reset()
{
  mOldState.clear();
}

void EraserTool::on_deactivated(Registry& registry, Dispatcher& dispatcher)
{
  (void) dispatcher;
  _try_end_sequence(registry);
}

void EraserTool::on_mouse_exited(Registry& registry, Dispatcher& dispatcher)
{
  (void) dispatcher;
  _try_end_sequence(registry);
}

void EraserTool::on_mouse_pressed(Registry& registry,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseInfo& mouse)
{
  (void) dispatcher;
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(registry)) {
    _update_sequence(registry, mouse.tile_pos);
  }
}

void EraserTool::on_mouse_dragged(Registry& registry,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseInfo& mouse)
{
  (void) dispatcher;
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(registry)) {
    _update_sequence(registry, mouse.tile_pos);
  }
}

void EraserTool::on_mouse_released(Registry& registry,
                                   Dispatcher& dispatcher,
                                   const ViewportMouseInfo& mouse)
{
  (void) dispatcher;
  if (mouse.button == MouseButton::Left && is_available(registry)) {
    _try_end_sequence(registry);
  }
}

void EraserTool::_update_sequence(Registry& registry, const TilePos& mouse_pos)
{
  const auto map_entity = sys::get_active_map(registry);

  const auto& map = registry.get<Map>(map_entity);
  auto& tile_layer = registry.get<TileLayer>(map.active_layer);

  if (const auto tile_id = sys::tile_at(tile_layer, mouse_pos)) {
    if (tile_id != kEmptyTile) {
      mOldState.try_emplace(mouse_pos, *tile_id);
    }

    sys::set_tile(tile_layer, mouse_pos, kEmptyTile);
  }
}

void EraserTool::_try_end_sequence(Registry& registry)
{
  if (!mOldState.empty() && is_available(registry)) {
    const auto map_entity = sys::get_active_map(registry);
    const auto& map = registry.get<Map>(map_entity);

    sys::try_store<cmd::EraserSequence>(registry, map.active_layer, std::move(mOldState));
  }

  mOldState.clear();
}

auto EraserTool::is_available(const Registry& registry) const -> bool
{
  if (const auto* map = sys::try_get_active_map(registry)) {
    return map->active_layer != kNullEntity && registry.has<TileLayer>(map->active_layer);
  }

  return false;
}

}  // namespace tactile
