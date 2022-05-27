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

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/algorithms/invoke.hpp"
#include "core/common/ecs.hpp"
#include "core/common/random.hpp"
#include "core/renderer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/map_system.hpp"
#include "core/systems/tileset_system.hpp"
#include "core/tile_pos.hpp"
#include "core/documents/map_document.hpp"
#include "core/documents/tileset_document.hpp"
#include "editor/events/tool_events.hpp"
#include "editor/model.hpp"
#include "misc/assert.hpp"

namespace tactile {
namespace {

[[nodiscard]] constexpr auto _to_vec2(const TilePos& pos) noexcept -> glm::vec2
{
  return {static_cast<float>(pos.col()), static_cast<float>(pos.row())};
}

}  // namespace

void StampTool::draw_gizmos(const DocumentModel& model,
                            IRenderer& renderer,
                            const MouseInfo& mouse) const
{
  TACTILE_ASSERT(model.is_map_active());

  const auto mapId = model.active_document_id().value();
  const auto map = model.get_map(mapId);
  const auto& registry = map->get_registry();

  if (!mouse.is_within_contents || !sys::is_tileset_selection_not_empty(registry)) {
    return;
  }

  // TODO implement preview when randomizer is enabled
  if (mRandomMode) {
    return;
  }

  const auto& activeTileset = ctx_get<comp::ActiveTileset>(registry);

  const auto& selection =
      checked_get<comp::TilesetSelection>(registry, activeTileset.entity);
  if (!selection.region) {
    return;
  }

  const auto& region = selection.region.value();

  const auto selectionSize = region.end - region.begin;
  const auto offset = selectionSize / TilePos{2, 2};

  const auto& tilesetRef = checked_get<comp::TilesetRef>(registry, activeTileset.entity);
  const auto tileset = model.get_tileset(tilesetRef.source_tileset);

  const auto& texture = tileset->texture();
  const auto& uv = tileset->uv_size();

  const auto origin = renderer.get_origin();
  const auto gridSize = renderer.get_grid_size();

  invoke_mn(selectionSize.row(), selectionSize.col(), [&](int32 row, int32 col) {
    const TilePos index{row, col};
    const auto previewPos = mouse.position_in_viewport + index - offset;

    if (sys::is_position_in_map(registry, previewPos)) {
      const auto realPos = origin + _to_vec2(previewPos) * gridSize;

      const auto uvMin = _to_vec2(region.begin + index) * uv;
      const auto uvMax = uvMin + uv;

      constexpr uint8 opacity = 150;
      renderer.render_image(texture.id, realPos, gridSize, uvMin, uvMax, opacity);
    }
  });
}

void StampTool::on_disabled(entt::registry&, entt::dispatcher& dispatcher)
{
  maybe_emit_event(dispatcher);
}

void StampTool::on_exited(entt::registry&, entt::dispatcher& dispatcher)
{
  maybe_emit_event(dispatcher);
}

void StampTool::on_pressed(entt::registry& registry,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(registry)) {
    mPrevious.clear();
    mCurrent.clear();
    mLastChangedPos.reset();

    update_sequence(registry, mouse.position_in_viewport);
  }
}

void StampTool::on_dragged(entt::registry& registry,
                           entt::dispatcher&,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_usable(registry)) {
    update_sequence(registry, mouse.position_in_viewport);
  }
}

void StampTool::on_released(entt::registry& registry,
                            entt::dispatcher& dispatcher,
                            const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_usable(registry)) {
    maybe_emit_event(dispatcher);
  }
}

auto StampTool::is_available(const entt::registry& registry) const -> bool
{
  return sys::is_tile_layer_active(registry);
}

auto StampTool::get_type() const -> ToolType
{
  return ToolType::Stamp;
}

void StampTool::set_random(const bool random)
{
  mRandomMode = random;
}

auto StampTool::is_random() const -> bool
{
  return mRandomMode;
}

void StampTool::update_sequence(entt::registry& registry, const TilePos& cursor)
{
  TACTILE_ASSERT(is_usable(registry));

  const auto layerEntity = sys::get_active_layer(registry);
  auto& layer = checked_get<comp::TileLayer>(registry, layerEntity);

  const auto tsetEntity = sys::find_active_tileset(registry);
  const auto& selection = checked_get<comp::TilesetSelection>(registry, tsetEntity);

  const auto& region = selection.region.value();
  const auto selectionSize = region.end - region.begin;

  if (mRandomMode) {
    if (mLastChangedPos != cursor) {
      const auto index = next_random(0, (selectionSize.row() * selectionSize.col()) - 1);
      const auto selectionPos =
          region.begin + TilePos::from_index(index, selectionSize.col());
      const auto tile = sys::get_tile_from_tileset(registry, tsetEntity, selectionPos);

      if (!mPrevious.contains(cursor)) {
        mPrevious.emplace(cursor, sys::get_tile(layer, cursor));
      }

      mCurrent[cursor] = tile;
      sys::set_tile(layer, cursor, tile);
    }

    mLastChangedPos = cursor;
  }
  else {
    const auto previewOffset = selectionSize / TilePos{2, 2};

    invoke_mn(selectionSize.row(), selectionSize.col(), [&](int32 row, int32 col) {
      const TilePos index{row, col};
      const auto selectionPosition = region.begin + index;

      const auto tile =
          sys::get_tile_from_tileset(registry, tsetEntity, selectionPosition);
      if (tile != empty_tile) {
        const auto pos = cursor + index - previewOffset;
        if (sys::is_position_in_map(registry, pos)) {
          if (!mPrevious.contains(pos)) {
            mPrevious.emplace(pos, sys::get_tile(layer, pos));
          }
          mCurrent.insert_or_assign(pos, tile);
          sys::set_tile(layer, pos, tile);
        }
      }
    });
  }
}

void StampTool::maybe_emit_event(entt::dispatcher& dispatcher)
{
  if (!mPrevious.empty() && !mCurrent.empty()) {
    dispatcher.enqueue<StampSequenceEvent>(std::move(mPrevious), std::move(mCurrent));
    mPrevious.clear();
    mCurrent.clear();
    mLastChangedPos.reset();
  }
}

auto StampTool::is_usable(const entt::registry& registry) const -> bool
{
  return sys::is_tile_layer_active(registry) &&
         sys::is_tileset_selection_not_empty(registry);
}

}  // namespace tactile