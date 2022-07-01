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

#include "stamp_tool.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "core/common/functional.hpp"
#include "core/common/math.hpp"
#include "core/common/random.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "core/events/tool_events.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/model.hpp"
#include "core/renderer.hpp"
#include "core/tile_pos.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

// TODO move
[[nodiscard]] constexpr auto _to_vec2(const TilePos& pos) noexcept -> Vector2f
{
  return {static_cast<float>(pos.col()), static_cast<float>(pos.row())};
}

}  // namespace

void StampTool::draw_gizmos(const DocumentModel& model,
                            IRenderer&           renderer,
                            const MouseInfo&     mouse) const
{
  const auto& document = model.require_active_map();

  const auto& map = document.get_map();
  const auto& tilesets = map.get_tilesets();

  const auto  tilesetId = tilesets.active_tileset_id().value();
  const auto& tilesetRef = tilesets.get_ref(tilesetId);

  if (!mouse.is_within_contents || !tilesetRef.selection) {
    return;
  }

  // TODO implement preview when randomizer is enabled
  if (mRandomMode) {
    return;
  }

  const auto  layerId = map.active_layer_id().value();
  const auto& layer = map.view_tile_layer(layerId);

  const auto& selection = tilesetRef.selection.value();
  const auto  selectionSize = selection.end - selection.begin;
  const auto  offset = selectionSize / TilePos{2, 2};

  const auto& tileset = tilesetRef.tileset;
  const auto  textureId = tileset->texture_id();
  const auto& uv = tileset->uv_size();

  const auto origin = renderer.get_origin();
  const auto gridSize = renderer.get_grid_size();

  invoke_mn(selectionSize.row(), selectionSize.col(), [&](int32 row, int32 col) {
    const TilePos index{row, col};
    const auto    previewPos = mouse.position_in_viewport + index - offset;

    if (layer.is_valid(previewPos)) {
      const auto realPos = origin + _to_vec2(previewPos) * gridSize;

      const auto uvMin = _to_vec2(selection.begin + index) * uv;
      const auto uvMax = uvMin + uv;

      constexpr uint8 opacity = 150;
      renderer.render_image(textureId, realPos, gridSize, uvMin, uvMax, opacity);
    }
  });
}

void StampTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void StampTool::on_pressed(DocumentModel& model,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();

    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_dragged(DocumentModel& model,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(model)) {
    update_sequence(model, mouse.position_in_viewport);
  }
}

void StampTool::on_released(DocumentModel&    model,
                            entt::dispatcher& dispatcher,
                            const MouseInfo&  mouse)
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

auto StampTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  return document.get_map().is_active_layer(LayerType::TileLayer);
}

void StampTool::update_sequence(DocumentModel& model, const TilePos& cursor)
{
  TACTILE_ASSERT(is_usable(model));

  auto& mapDocument = model.require_active_map();
  auto& map = mapDocument.get_map();

  const auto activeLayerId = map.active_layer_id().value();
  auto&      layer = map.view_tile_layer(activeLayerId);

  const auto& tilesets = map.get_tilesets();
  const auto  tilesetId = tilesets.active_tileset_id().value();
  const auto& tilesetRef = tilesets.get_ref(tilesetId);

  if (mRandomMode) {
    update_sequence_random(layer, tilesetRef, cursor);
  }
  else {
    update_sequence_normal(layer, tilesetRef, cursor);
  }
}

void StampTool::update_sequence_normal(core::TileLayer&        layer,
                                       const core::TilesetRef& tilesetRef,
                                       const TilePos&          cursor)
{
  const auto& selection = tilesetRef.selection.value();
  const auto  selectionSize = selection.end - selection.begin;
  const auto  previewOffset = selectionSize / TilePos{2, 2};

  invoke_mn(selectionSize.row(), selectionSize.col(), [&](int32 row, int32 col) {
    const TilePos index{row, col};
    const auto    selectionPosition = selection.begin + index;

    const auto tile =
        tilesetRef.first_tile + tilesetRef.tileset->index_of(selectionPosition);

    if (tile != empty_tile) {
      const auto pos = cursor + index - previewOffset;

      if (layer.is_valid(pos)) {
        if (!mPrevious.contains(pos)) {
          mPrevious.emplace(pos, layer.tile_at(pos));
        }
        mCurrent.insert_or_assign(pos, tile);
        layer.set_tile(pos, tile);
      }
    }
  });
}

void StampTool::update_sequence_random(core::TileLayer&        layer,
                                       const core::TilesetRef& tilesetRef,
                                       const TilePos&          cursor)
{
  const auto& selection = tilesetRef.selection.value();
  const auto  selectionSize = selection.end - selection.begin;

  if (mLastChangedPos != cursor) {
    const auto index = next_random(0, (selectionSize.row() * selectionSize.col()) - 1);
    const auto selectionPos =
        selection.begin + TilePos::from_index(index, selectionSize.col());
    const auto tile = tilesetRef.first_tile + tilesetRef.tileset->index_of(selectionPos);

    if (!mPrevious.contains(cursor)) {
      mPrevious.emplace(cursor, layer.tile_at(cursor));
    }

    mCurrent[cursor] = tile;
    layer.set_tile(cursor, tile);
  }

  mLastChangedPos = cursor;
}

void StampTool::maybe_emit_event(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (!mPrevious.empty() && !mCurrent.empty()) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto  layerId = map.active_layer_id().value();

    dispatcher.enqueue<StampSequenceEvent>(layerId,
                                           std::move(mPrevious),
                                           std::move(mCurrent));

    mPrevious.clear();
    mCurrent.clear();

    mLastChangedPos.reset();
  }
}

auto StampTool::is_usable(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  const auto& tilesets = map.get_tilesets();

  if (const auto tilesetId = tilesets.active_tileset_id()) {
    return map.is_active_layer(LayerType::TileLayer) &&
           tilesets.get_ref(*tilesetId).is_single_tile_selected();
  }
  else {
    return false;
  }
}

}  // namespace tactile