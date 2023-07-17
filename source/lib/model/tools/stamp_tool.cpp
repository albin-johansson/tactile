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

#include "common/debug/assert.hpp"
#include "common/tile_pos.hpp"
#include "common/type/maybe.hpp"
#include "common/type/tile_cache.hpp"
#include "common/util/functional.hpp"
#include "common/util/random.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/entity_validation.hpp"
#include "model/events/tool_events.hpp"
#include "model/layers/layer_components.hpp"
#include "model/layers/tile_layer_ops.hpp"
#include "model/maps/map_components.hpp"
#include "model/model.hpp"
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

void StampTool::on_deactivated(Model& model, Dispatcher& dispatcher)
{
  _try_end_sequence(model, dispatcher);
}

void StampTool::on_mouse_exited(Model& model, Dispatcher& dispatcher)
{
  _try_end_sequence(model, dispatcher);
}

void StampTool::on_mouse_pressed(Model& model,
                                 Dispatcher&,
                                 const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(model)) {
    _update_sequence(model, mouse.tile_pos);
  }
}

void StampTool::on_mouse_dragged(Model& model,
                                 Dispatcher&,
                                 const ViewportMouseInfo& mouse)
{
  if (mouse.over_content && mouse.button == MouseButton::Left && is_available(model)) {
    _update_sequence(model, mouse.tile_pos);
  }
}

void StampTool::on_mouse_released(Model& model,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseInfo& mouse)
{
  if (mouse.button == MouseButton::Left && is_available(model)) {
    _try_end_sequence(model, dispatcher);
  }
}

void StampTool::_update_sequence(Model& model, const TilePos& mouse_pos)
{
  TACTILE_ASSERT(is_available(model));
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  if (_behaves_as_if_random(model, map)) {
    _update_random_sequence(model, map, mouse_pos);
  }
  else {
    _update_normal_sequence(model, map, mouse_pos);
  }
}

void StampTool::_update_normal_sequence(Model& model,
                                        const Map& map,
                                        const TilePos& mouse_pos)
{
  auto& tile_layer = model.get<TileLayer>(map.active_layer);

  const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
  const auto& tileset = model.get<Tileset>(attached_tileset.tileset);

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

void StampTool::_update_random_sequence(Model& model,
                                        const Map& map,
                                        const TilePos& mouse_pos)
{
  auto& tile_layer = model.get<TileLayer>(map.active_layer);

  const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
  const auto& selection = attached_tileset.selection.value();

  const auto selection_size = selection.end - selection.begin;
  const auto selected_tile_count = selection_size.row() * selection_size.col();

  if (mLastChangedPos != mouse_pos) {
    const auto index = next_random_i32(0, selected_tile_count - 1);
    const auto selection_pos =
        selection.begin + TilePos::from_index(index, selection_size.col());

    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);
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

void StampTool::_try_end_sequence(Model& model, Dispatcher& dispatcher)
{
  if (const auto* map = sys::try_get_active_map(model)) {
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

auto StampTool::_behaves_as_if_random(const Model& model, const Map& map) const -> bool
{
  return mIsRandom && sys::is_stamp_tool_randomizer_possible(model, map);
}

auto StampTool::is_available(const Model& model) const -> bool
{
  if (const auto* map = sys::try_get_active_map(model)) {
    if (map->active_layer != kNullEntity && map->active_tileset != kNullEntity) {
      const auto& attached_tileset = model.get<AttachedTileset>(map->active_tileset);

      return model.has<TileLayer>(map->active_layer) &&
             attached_tileset.selection.has_value();
    }
  }

  return false;
}

namespace sys {

auto is_stamp_tool_available(const Model& model) -> bool
{
  if (const auto* map = try_get_active_map(model)) {
    if (map->active_layer != kNullEntity && map->active_tileset != kNullEntity) {
      const auto& attached_tileset = model.get<AttachedTileset>(map->active_tileset);

      return model.has<TileLayer>(map->active_layer) &&
             attached_tileset.selection.has_value();
    }
  }

  return false;
}

auto is_stamp_tool_randomizer_possible(const Model& model, const Map& map) -> bool
{
  if (map.active_tileset != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    return is_single_tile_selected(attached_tileset);
  }

  return false;
}

}  // namespace sys
}  // namespace tactile
