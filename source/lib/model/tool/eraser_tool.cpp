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

#include "eraser_tool.hpp"

#include <utility>  // move

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/mouse_info.hpp"
#include "model/systems/document_system.hpp"

namespace tactile {

void EraserTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void EraserTool::on_disabled(Model& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EraserTool::on_exited(Model& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EraserTool::on_pressed(Model& model, entt::dispatcher&, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    mPrevState.clear();
    update_sequence(model, mouse.position_in_viewport);
  }
}

void EraserTool::on_dragged(Model& model, entt::dispatcher&, const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    update_sequence(model, mouse.position_in_viewport);
  }
}

void EraserTool::on_released(Model& model,
                             entt::dispatcher& dispatcher,
                             const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto EraserTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);
  if (document_entity == kNullEntity || !model.has<MapDocument>(document_entity)) {
    return false;
  }

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  return map.active_layer != kNullEntity && model.has<TileLayer>(map.active_layer);
}

void EraserTool::update_sequence(Model& model, const TilePos& cursor)
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);
  auto& tile_layer = model.get<TileLayer>(map.active_layer);

  if (mPrevState.find(cursor) == mPrevState.end()) {
    mPrevState[cursor] = tile_layer.tile_at(cursor).value();
  }

  tile_layer.set_tile(cursor, kEmptyTile);
}

void EraserTool::maybe_emit_event(const Model& model, entt::dispatcher& dispatcher)
{
  if (!mPrevState.empty()) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);

    dispatcher.enqueue<EraserSequenceEvent>(map.active_layer, std::move(mPrevState));
    mPrevState.clear();
  }
}

}  // namespace tactile