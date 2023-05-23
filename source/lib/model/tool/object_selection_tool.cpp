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

#include "object_selection_tool.hpp"

#include "components/document.hpp"
#include "components/map.hpp"
#include "core/layer.hpp"
#include "core/object.hpp"
#include "core/viewport.hpp"
#include "model/event/object_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/object_layer_system.hpp"

namespace tactile {

void ObjectSelectionTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectSelectionTool::on_exited(Model& model, Dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void ObjectSelectionTool::on_pressed(Model& model,
                                     Dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  if ((mouse.button == cen::mouse_button::left ||
       mouse.button == cen::mouse_button::right) &&
      is_available(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& document_viewport = model.get<Viewport>(document_entity);

    const auto& map = model.get<Map>(map_document.map);
    auto& object_layer = model.get<ObjectLayer>(map.active_layer);

    const auto scaling_ratio = document_viewport.scaling_ratio(map.tile_size);
    const auto object_entity = sys::find_object_at_position(model,
                                                            object_layer.active_object,
                                                            mouse.pos / scaling_ratio,
                                                            map.tile_size);

    if (mouse.button == cen::mouse_button::left) {
      object_layer.active_object = object_entity;

      if (object_entity != kNullEntity) {
        const auto& object = model.get<Object>(object_entity);

        auto& drag = mDragInfo.emplace();
        drag.origin_object_pos = object.position;
        drag.last_mouse_pos = mouse.pos;
      }
    }
    else if (mouse.button == cen::mouse_button::right) {
      object_layer.active_object = object_entity;

      if (object_entity != kNullEntity) {
        dispatcher.enqueue<SpawnObjectContextMenuEvent>(object_entity);
      }
    }
  }
}

void ObjectSelectionTool::on_dragged(Model& model,
                                     Dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  if (mDragInfo && mouse.button == cen::mouse_button::left && is_available(model)) {
    const auto document_entity = sys::get_active_document(model);

    const auto& map_document = model.get<MapDocument>(document_entity);
    const auto& map = model.get<Map>(map_document.map);
    const auto& object_layer = model.get<ObjectLayer>(map.active_layer);

    if (object_layer.active_object != kNullEntity) {
      if (mouse.is_within_contents) {
        const auto& document_viewport = model.get<Viewport>(document_entity);

        const auto scaling_ratio = document_viewport.scaling_ratio(map.tile_size);
        const auto delta = (mouse.pos - mDragInfo->last_mouse_pos) / scaling_ratio;

        auto& object = model.get<Object>(object_layer.active_object);
        object.position += delta;

        mDragInfo->last_mouse_pos = mouse.pos;
      }
      else {
        // Stop if the user drags the object outside the map
        maybe_emit_event(model, dispatcher);
      }
    }
  }
}

void ObjectSelectionTool::on_released(Model& model,
                                      Dispatcher& dispatcher,
                                      const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto ObjectSelectionTool::is_available(const Model& model) const -> bool
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  return map.active_layer != kNullEntity && model.has<ObjectLayer>(map.active_layer);
}

void ObjectSelectionTool::maybe_emit_event(Model& model, Dispatcher& dispatcher)
{
  const auto document_entity = sys::get_active_document(model);

  const auto& map_document = model.get<MapDocument>(document_entity);
  const auto& map = model.get<Map>(map_document.map);

  if (mDragInfo.has_value()) {
    if (map.active_layer != kNullEntity) {
      const auto& object_layer = model.get<ObjectLayer>(map.active_layer);

      if (object_layer.active_object != kNullEntity) {
        const auto& object = model.get<Object>(object_layer.active_object);

        // Only emit an event if the object has been moved along any axis
        if (mDragInfo->origin_object_pos != object.position) {
          dispatcher.enqueue<MoveObjectEvent>(object_layer.active_object,
                                              mDragInfo->origin_object_pos,
                                              object.position);
        }
      }
    }

    mDragInfo.reset();
  }
}

}  // namespace tactile