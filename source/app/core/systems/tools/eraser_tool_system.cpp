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

#include "eraser_tool_system.hpp"

#include <utility>  // move

#include <entt/entt.hpp>

#include "core/common/tile_cache.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/tile_pos.hpp"
#include "editor/events/tool_events.hpp"
#include "tactile.hpp"

namespace tactile::sys {
namespace {

inline TileCache _old_state;

void _update_sequence(entt::registry& registry, const TilePos& cursor)
{
  const auto entity = get_active_layer(registry);
  auto& layer = checked_get<comp::TileLayer>(registry, entity);

  if (!_old_state.contains(cursor)) {
    _old_state.emplace(cursor, get_tile(layer, cursor));
  }

  set_tile(layer, cursor, empty_tile);
}

void _maybe_emit_event(entt::dispatcher& dispatcher)
{
  if (!_old_state.empty()) {
    dispatcher.enqueue<EraserSequenceEvent>(std::move(_old_state));
    _old_state.clear();
  }
}

}  // namespace

void on_eraser_tool_disabled(entt::dispatcher& dispatcher)
{
  _maybe_emit_event(dispatcher);
}

void on_eraser_tool_exited(entt::dispatcher& dispatcher)
{
  _maybe_emit_event(dispatcher);
}

void on_eraser_tool_pressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_tile_layer_active(registry)) {
    _old_state.clear();
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void on_eraser_tool_dragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_tile_layer_active(registry)) {
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void on_eraser_tool_released(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_tile_layer_active(registry)) {
    _maybe_emit_event(dispatcher);
  }
}

}  // namespace tactile::sys
