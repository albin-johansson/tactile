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
#include "components/document.hpp"
#include "components/layer.hpp"
#include "components/map.hpp"
#include "components/tileset.hpp"
#include "components/tool.hpp"
#include "components/viewport.hpp"
#include "model/event/tool_events.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/tool_system.hpp"
#include "model/systems/validation.hpp"

namespace tactile::sys {
namespace {

struct StampToolData final {
  TileCache old_state;
  TileCache new_state;
  Maybe<TilePos> last_changed_pos;
  bool is_random {false};
};

[[nodiscard]] auto _is_usable(const Model& model) -> bool
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  if (map.active_layer == kNullEntity || !model.has<TileLayer>(map.active_layer)) {
    return false;
  }

  if (map.active_tileset != kNullEntity) {
    const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
    return attached_tileset.selection.has_value();
  }

  return false;
}

[[nodiscard]] auto _behaves_as_if_random(const Model& model,
                                         const StampToolData& tool_data,
                                         const Map& map) -> bool
{
  return tool_data.is_random && sys::is_stamp_tool_randomizer_possible(model, map);
}

void _update_random_sequence(Model& model,
                             StampToolData& tool_data,
                             const Map& map,
                             const TilePos& mouse_pos)
{
  auto& tile_layer = model.get<TileLayer>(map.active_layer);

  const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);
  const auto& selection = attached_tileset.selection.value();

  const auto selection_size = selection.end - selection.begin;
  const auto selected_tile_count = selection_size.row() * selection_size.col();

  if (tool_data.last_changed_pos != mouse_pos) {
    const auto index = next_random_i32(0, selected_tile_count - 1);
    const auto selection_pos =
        selection.begin + TilePos::from_index(index, selection_size.col());

    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);
    const auto tile_id = attached_tileset.first_tile + tileset.index_of(selection_pos);

    if (!tool_data.old_state.contains(mouse_pos)) {
      tool_data.old_state[mouse_pos] = tile_layer.tile_at(mouse_pos).value();
    }

    tool_data.new_state[mouse_pos] = tile_id;
    tile_layer.set_tile(mouse_pos, tile_id);
  }

  tool_data.last_changed_pos = mouse_pos;
}

void _update_normal_sequence(Model& model,
                             StampToolData& tool_data,
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

  invoke_mn(selected_rows, selected_cols, [&](const int32 row, const int32 col) {
    const TilePos index {row, col};
    const auto selection_pos = selection.begin + index;

    const auto tile_id = attached_tileset.first_tile + tileset.index_of(selection_pos);
    if (tile_id != kEmptyTile) {
      const auto pos = mouse_pos + index - preview_offset;

      if (tile_layer.contains(pos)) {
        if (!tool_data.old_state.contains(pos)) {
          tool_data.old_state[pos] = tile_layer.tile_at(pos).value();
        }

        tool_data.new_state[pos] = tile_id;
        tile_layer.set_tile(pos, tile_id);
      }
    }
  });
}

void _update_sequence(Model& model, StampToolData& tool_data, const TilePos& mouse_pos)
{
  TACTILE_ASSERT(_is_usable(model));

  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  if (_behaves_as_if_random(model, tool_data, map)) {
    _update_random_sequence(model, tool_data, map, mouse_pos);
  }
  else {
    _update_normal_sequence(model, tool_data, map, mouse_pos);
  }
}

void _try_end_sequence(Model& model, const Entity tool_entity, Dispatcher& dispatcher)
{
  if (const auto* map = sys::try_get_active_map(model)) {
    auto& tool_data = model.get<StampToolData>(tool_entity);

    if (!tool_data.old_state.empty() && !tool_data.new_state.empty()) {
      dispatcher.enqueue<StampSequenceEvent>(map->active_layer,
                                             std::move(tool_data.old_state),
                                             std::move(tool_data.new_state));

      tool_data.old_state.clear();
      tool_data.new_state.clear();
      tool_data.last_changed_pos.reset();
    }
  }
}

}  // namespace

auto create_stamp_tool(Model& model) -> Entity
{
  const auto stamp_tool_entity = model.create_entity();

  auto& stamp_tool = model.add<Tool>(stamp_tool_entity);
  stamp_tool.on_deactivated = &on_stamp_tool_deactivated;
  stamp_tool.on_exited = &on_stamp_tool_exited;
  stamp_tool.on_pressed = &on_stamp_tool_pressed;
  stamp_tool.on_dragged = &on_stamp_tool_dragged;
  stamp_tool.on_released = &on_stamp_tool_released;
  stamp_tool.is_available = &is_stamp_tool_available;

  TACTILE_ASSERT(is_tool_entity(model, stamp_tool_entity));
  return stamp_tool_entity;
}

void on_stamp_tool_deactivated(Model& model,
                               const Entity tool_entity,
                               Dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_tool_entity(model, tool_entity));
  _try_end_sequence(model, tool_entity, dispatcher);
}

void on_stamp_tool_exited(Model& model, const Entity tool_entity, Dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_tool_entity(model, tool_entity));
  _try_end_sequence(model, tool_entity, dispatcher);
}

void on_stamp_tool_pressed(Model& model,
                           const Entity tool_entity,
                           const ViewportMouseInfo& mouse,
                           Dispatcher&)
{
  TACTILE_ASSERT(is_tool_entity(model, tool_entity));

  if (mouse.in_viewport && mouse.button == MouseButton::Left && _is_usable(model)) {
    auto& tool_data = model.add<StampToolData>(tool_entity);
    _update_sequence(model, tool_data, mouse.tile_pos);
  }
}

void on_stamp_tool_dragged(Model& model,
                           const Entity tool_entity,
                           const ViewportMouseInfo& mouse,
                           Dispatcher&)
{
  TACTILE_ASSERT(is_tool_entity(model, tool_entity));

  if (mouse.in_viewport && mouse.button == MouseButton::Left && _is_usable(model)) {
    auto& tool_data = model.add<StampToolData>(tool_entity);
    _update_sequence(model, tool_data, mouse.tile_pos);
  }
}

void on_stamp_tool_released(Model& model,
                            const Entity tool_entity,
                            const ViewportMouseInfo& mouse,
                            Dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_tool_entity(model, tool_entity));

  if (mouse.button == MouseButton::Left && _is_usable(model)) {
    _try_end_sequence(model, tool_entity, dispatcher);
  }
}

auto is_stamp_tool_available(const Model& model) -> bool
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

}  // namespace tactile::sys
