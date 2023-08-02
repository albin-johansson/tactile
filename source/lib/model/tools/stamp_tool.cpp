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

#include "stamp_tool.hpp"

#include <utility>  // move

#include "core/debug/assert.hpp"
#include "core/ecs/registry.hpp"
#include "core/functional/invoke.hpp"
#include "core/functional/maybe.hpp"
#include "core/math/rng.hpp"
#include "core/tiles/tile_cache.hpp"
#include "core/tiles/tile_pos.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/events/tool_events.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/tile_layer_ops.hpp"
#include "model/maps/map_components.hpp"
#include "model/tilesets/attached_tileset_ops.hpp"
#include "model/tilesets/tileset_components.hpp"
#include "model/tilesets/tileset_ops.hpp"
#include "model/viewports/viewport_components.hpp"

namespace tactile {

void StampTool::reset()
{
  mOldState.clear();
  mNewState.clear();
  mLastChangedPos.reset();
  mIsRandom = false;
}

void StampTool::on_deactivated(Registry& registry, Dispatcher& dispatcher)
{
  _try_end_sequence(registry, dispatcher);
}

void StampTool::on_mouse_exited(Registry& registry, Dispatcher& dispatcher)
{
  _try_end_sequence(registry, dispatcher);
}

void StampTool::on_mouse_pressed(Registry& registry,
                                 Dispatcher&,
                                 const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(registry)) {
    _update_sequence(registry, mouse.tile_pos);
  }
}

void StampTool::on_mouse_dragged(Registry& registry,
                                 Dispatcher&,
                                 const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(registry)) {
    _update_sequence(registry, mouse.tile_pos);
  }
}

void StampTool::on_mouse_released(Registry& registry,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseInfo& mouse)
{
  if (mouse.button == MouseButton::Left && is_available(registry)) {
    _try_end_sequence(registry, dispatcher);
  }
}

void StampTool::_update_sequence(Registry& registry, const TilePos& mouse_pos)
{
  TACTILE_ASSERT(is_available(registry));
  const auto document_entity = sys::get_active_document(registry);

  const auto& map_document = registry.get<MapDocument>(document_entity);
  const auto& map = registry.get<Map>(map_document.map);

  if (_behaves_as_if_random(registry)) {
    _update_random_sequence(registry, map, mouse_pos);
  }
  else {
    _update_normal_sequence(registry, map, mouse_pos);
  }
}

void StampTool::_update_normal_sequence(Registry& registry,
                                        const Map& map,
                                        const TilePos& mouse_pos)
{
  auto& tile_layer = registry.get<TileLayer>(map.active_layer);

  const auto& attached_tileset = registry.get<AttachedTileset>(map.active_tileset);
  const auto& tileset = registry.get<Tileset>(attached_tileset.tileset);

  const auto& selection = attached_tileset.selection.value();
  const auto selection_size = selection.end - selection.begin;
  const auto preview_offset = selection_size / TilePos {2, 2};

  const auto selected_rows = selection_size.row();
  const auto selected_cols = selection_size.col();

  for (int32 row = 0; row < selected_rows; ++row) {
    for (int32 col = 0; col < selected_cols; ++col) {
      const TilePos index {row, col};
      const auto selection_pos = selection.begin + index;

      const auto tile_id =
          attached_tileset.first_tile + sys::tile_index_at(tileset, selection_pos);
      if (tile_id != kEmptyTile) {
        const auto pos = mouse_pos + index - preview_offset;

        if (sys::is_valid_tile(tile_layer, pos)) {
          if (!mOldState.contains(pos)) {
            mOldState[pos] = sys::tile_at(tile_layer, pos).value();
          }

          mNewState[pos] = tile_id;
          sys::set_tile(tile_layer, pos, tile_id);
        }
      }
    }
  }
}

void StampTool::_update_random_sequence(Registry& registry,
                                        const Map& map,
                                        const TilePos& mouse_pos)
{
  auto& tile_layer = registry.get<TileLayer>(map.active_layer);

  const auto& attached_tileset = registry.get<AttachedTileset>(map.active_tileset);
  const auto& selection = attached_tileset.selection.value();

  const auto selection_size = selection.end - selection.begin;
  const auto selected_tile_count = selection_size.row() * selection_size.col();

  if (mLastChangedPos != mouse_pos) {
    const auto index = next_random_i32(0, selected_tile_count - 1);
    const auto selection_pos =
        selection.begin + TilePos::from_index(index, selection_size.col());

    const auto& tileset = registry.get<Tileset>(attached_tileset.tileset);
    const auto tile_id =
        attached_tileset.first_tile + sys::tile_index_at(tileset, selection_pos);

    if (!mOldState.contains(mouse_pos)) {
      mOldState[mouse_pos] = sys::tile_at(tile_layer, mouse_pos).value();
    }

    mNewState[mouse_pos] = tile_id;
    sys::set_tile(tile_layer, mouse_pos, tile_id);
  }

  mLastChangedPos = mouse_pos;
}

void StampTool::_try_end_sequence(Registry& registry, Dispatcher& dispatcher)
{
  if (const auto* map = sys::try_get_active_map(registry)) {
    if (!mOldState.empty() && !mNewState.empty()) {
      dispatcher.enqueue<StampSequenceEvent>(map->active_layer,
                                             std::move(mOldState),
                                             std::move(mNewState));

      mOldState.clear();
      mNewState.clear();
      mLastChangedPos.reset();
    }
  }
}

auto StampTool::_behaves_as_if_random(const Registry& registry) const -> bool
{
  return mIsRandom && is_randomizer_available(registry);
}

auto StampTool::is_randomizer_available(const Registry& registry) const -> bool
{
  if (const auto* map = sys::try_get_active_map(registry)) {
    if (map->active_tileset != kNullEntity) {
      const auto& attached_tileset = registry.get<AttachedTileset>(map->active_tileset);
      return sys::is_single_tile_selected(attached_tileset);
    }
  }

  return false;
}

auto StampTool::is_available(const Registry& registry) const -> bool
{
  if (const auto* map = sys::try_get_active_map(registry)) {
    if (map->active_layer != kNullEntity && map->active_tileset != kNullEntity) {
      const auto& attached_tileset = registry.get<AttachedTileset>(map->active_tileset);

      return registry.has<TileLayer>(map->active_layer) &&
             attached_tileset.selection.has_value();
    }
  }

  return false;
}

}  // namespace tactile
