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

#include "stamp_tool_system.hpp"

#include <utility>  // move

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/common/tile_cache.hpp"
#include "core/components/layers.hpp"
#include "core/components/tiles.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tile_pos.hpp"
#include "editor/events/tool_events.hpp"
#include "misc/assert.hpp"
#include "tactile.hpp"

namespace tactile::sys {
namespace {

inline TileCache _old_state;
inline TileCache _sequence;

[[nodiscard]] auto _is_usable(const entt::registry& registry) -> bool
{
  return is_tile_layer_active(registry) && is_tileset_selection_not_empty(registry);
}

void _update_sequence(entt::registry& registry, const TilePos& cursor)
{
  TACTILE_ASSERT(_is_usable(registry));

  const auto layerEntity = get_active_layer(registry);
  auto& layer = checked_get<comp::TileLayer>(registry, layerEntity);

  const auto tilesetEntity = find_active_tileset(registry);
  const auto& selection = checked_get<comp::TilesetSelection>(registry, tilesetEntity);
  const auto& region = selection.region.value();

  const auto selectionSize = region.end - region.begin;
  const auto previewOffset = selectionSize / TilePos{2, 2};
  const auto endRow = selectionSize.row();
  const auto endCol = selectionSize.col();

  for (auto row = 0; row < endRow; ++row) {
    for (auto col = 0; col < endCol; ++col) {
      const auto index = TilePos{row, col};
      const auto selectionPosition = region.begin + index;

      const auto tile = get_tile_from_tileset(registry, tilesetEntity, selectionPosition);
      if (tile != empty_tile) {
        const auto pos = cursor + index - previewOffset;
        if (is_position_in_map(registry, pos)) {
          if (!_old_state.contains(pos)) {
            _old_state.emplace(pos, get_tile(layer, pos));
          }
          _sequence.insert_or_assign(pos, tile);
          set_tile(layer, pos, tile);
        }
      }
    }
  }
}

void _maybe_emit_event(entt::dispatcher& dispatcher)
{
  if (!_old_state.empty() && !_sequence.empty()) {
    dispatcher.enqueue<StampSequenceEvent>(std::move(_old_state), std::move(_sequence));
    _old_state.clear();
    _sequence.clear();
  }
}

}  // namespace

void on_stamp_tool_disabled(entt::dispatcher& dispatcher)
{
  _maybe_emit_event(dispatcher);
}

void on_stamp_tool_exited(entt::dispatcher& dispatcher)
{
  _maybe_emit_event(dispatcher);
}

void on_stamp_tool_pressed(entt::registry& registry, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      _is_usable(registry)) {
    _old_state.clear();
    _sequence.clear();

    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void on_stamp_tool_dragged(entt::registry& registry, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      _is_usable(registry)) {
    _update_sequence(registry, mouse.position_in_viewport);
  }
}

void on_stamp_tool_released(entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && _is_usable(registry)) {
    _maybe_emit_event(dispatcher);
  }
}

}  // namespace tactile::sys
