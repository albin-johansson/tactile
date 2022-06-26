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

#include "eraser_tool.hpp"

#include <utility>  // move

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/documents/map_document.hpp"
#include "core/events/tool_events.hpp"
#include "core/layers/tile_layer.hpp"
#include "core/model.hpp"
#include "core/mouse_info.hpp"

namespace tactile {

void EraserTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EraserTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EraserTool::on_pressed(DocumentModel& model,
                            entt::dispatcher&,
                            const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    mPrevState.clear();
    update_sequence(model, mouse.position_in_viewport);
  }
}

void EraserTool::on_dragged(DocumentModel& model,
                            entt::dispatcher&,
                            const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    update_sequence(model, mouse.position_in_viewport);
  }
}

void EraserTool::on_released(DocumentModel& model,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto EraserTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();
  return map.is_active_layer(LayerType::TileLayer);
}

void EraserTool::update_sequence(DocumentModel& model, const TilePos& cursor)
{
  auto& document = model.require_active_map();
  auto& map = document.get_map();

  const auto layerId = map.active_layer_id().value();
  auto&      layer = map.view_tile_layer(layerId);

  mPrevState.try_emplace(cursor, layer.tile_at(cursor));
  layer.set_tile(cursor, empty_tile);
}

void EraserTool::maybe_emit_event(const DocumentModel& model,
                                  entt::dispatcher&    dispatcher)
{
  if (!mPrevState.empty()) {
    const auto& document = model.require_active_map();
    const auto& map = document.get_map();
    const auto  layerId = map.active_layer_id().value();

    dispatcher.enqueue<EraserSequenceEvent>(layerId, std::move(mPrevState));
    mPrevState.clear();
  }
}

}  // namespace tactile