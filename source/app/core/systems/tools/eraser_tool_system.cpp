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

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/tile_cache.hpp"
#include "core/tile_position.hpp"
#include "editor/events/tool_events.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile::sys {
namespace {

inline TileCache _old_state;

[[nodiscard]] auto _is_usable(const entt::registry& registry) -> bool
{
  return is_tile_layer_active(registry);
}

void _update_sequence(entt::registry& registry, const tile_position& cursor)
{
  const auto entity = get_active_layer(registry);
  TACTILE_ASSERT(entity != entt::null);

  TACTILE_ASSERT(registry.all_of<comp::tile_layer>(entity));
  auto& layer = registry.get<comp::tile_layer>(entity);

  if (!_old_state.contains(cursor)) {
    _old_state.emplace(cursor, get_tile(layer, cursor));
  }

  set_tile(layer, cursor, empty_tile);
}

}  // namespace

void eraser_tool_on_pressed(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    _old_state.clear();
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void eraser_tool_on_dragged(entt::registry& registry, const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void eraser_tool_on_released(entt::registry& registry,
                             entt::dispatcher& dispatcher,
                             const mouse_info& mouse)
{
  if (_is_usable(registry) && mouse.button == cen::mouse_button::left) {
    dispatcher.enqueue<eraser_sequence_event>(std::move(_old_state));
  }
}

}  // namespace tactile::sys
