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
#include "common/util/functional.hpp"
#include "common/util/random.hpp"
#include "components/document.hpp"
#include "core/map.hpp"
#include "core/tile_pos.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/tool_system.hpp"

namespace tactile {

void StampTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void StampTool::on_disabled(Model& model, Dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_exited(Model& model, Dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_pressed(Model& model, Dispatcher&, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();

    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_dragged(Model& model, Dispatcher&, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_released(Model& model, Dispatcher& dispatcher, const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_usable(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void StampTool::set_random(const bool random)
{
  mRandomMode = random;
}

auto StampTool::is_random() const -> bool
{
  return mRandomMode;
}

auto StampTool::behaves_as_if_random(const Model& model, const Entity map_entity) const
    -> bool
{
  return mRandomMode && sys::is_stamp_tool_randomizer_possible(model, map_entity);
}

auto StampTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  return map.active_layer != kNullEntity && model.has<TileLayer>(map.active_layer);
}

void StampTool::update_sequence(Model& model, const TilePos& cursor)
{
  TACTILE_ASSERT(is_usable(model));

  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  const auto& attached_tileset = model.get<AttachedTileset>(map.active_tileset);

  auto& tile_layer = model.get<TileLayer>(map.active_layer);

  if (behaves_as_if_random(model, map_document.map)) {
    update_sequence_random(model, tile_layer, attached_tileset, cursor);
  }
  else {
    update_sequence_normal(model, tile_layer, attached_tileset, cursor);
  }
}

void StampTool::update_sequence_normal(Model& model,
                                       TileLayer& tile_layer,
                                       const AttachedTileset& attached_tileset,
                                       const TilePos& cursor)
{
  const auto& tileset = model.get<Tileset>(attached_tileset.tileset);

  const auto& selection = attached_tileset.selection.value();
  const auto selection_size = selection.end - selection.begin;
  const auto preview_offset = selection_size / TilePos {2, 2};

  const auto selected_rows = selection_size.row();
  const auto selected_cols = selection_size.col();

  invoke_mn(selected_rows, selected_cols, [&](const int32 row, const int32 col) {
    const TilePos index {row, col};
    const auto selection_pos = selection.begin + index;

    const auto tile = attached_tileset.first_tile + tileset.index_of(selection_pos);
    if (tile != kEmptyTile) {
      const auto pos = cursor + index - preview_offset;

      if (tile_layer.contains(pos)) {
        if (mPrevious.find(pos) == mPrevious.end()) {
          mPrevious.emplace(pos, tile_layer.tile_at(pos).value());
        }

        mCurrent[pos] = tile;
        tile_layer.set_tile(pos, tile);
      }
    }
  });
}

void StampTool::update_sequence_random(Model& model,
                                       TileLayer& tile_layer,
                                       const AttachedTileset& attached_tileset,
                                       const TilePos& cursor)
{
  const auto& selection = attached_tileset.selection.value();
  const auto selection_size = selection.end - selection.begin;

  if (mLastChangedPos != cursor) {
    const auto index =
        next_random_i32(0, (selection_size.row() * selection_size.col()) - 1);
    const auto selection_pos =
        selection.begin + TilePos::from_index(index, selection_size.col());

    const auto& tileset = model.get<Tileset>(attached_tileset.tileset);
    const auto tile = attached_tileset.first_tile + tileset.index_of(selection_pos);

    if (mPrevious.find(cursor) == mPrevious.end()) {
      mPrevious.emplace(cursor, tile_layer.tile_at(cursor).value());
    }

    mCurrent[cursor] = tile;
    tile_layer.set_tile(cursor, tile);
  }

  mLastChangedPos = cursor;
}

void StampTool::maybe_emit_event(const Model& model, Dispatcher& dispatcher)
{
  if (!mPrevious.empty() && !mCurrent.empty()) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    dispatcher.enqueue<StampSequenceEvent>(map.active_layer,
                                           std::move(mPrevious),
                                           std::move(mCurrent));

    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();
  }
}

auto StampTool::is_usable(const Model& model) const -> bool
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

}  // namespace tactile