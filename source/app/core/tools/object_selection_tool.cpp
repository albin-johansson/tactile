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

#include "object_selection_tool.hpp"

#include <entt/signal/dispatcher.hpp>

#include "core/documents/map_document.hpp"
#include "core/events/object_events.hpp"
#include "core/events/tool_events.hpp"
#include "core/layers/object_layer.hpp"
#include "core/model.hpp"

namespace tactile {

void ObjectSelectionTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void ObjectSelectionTool::on_pressed(DocumentModel&    model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo&  mouse)
{
  if ((mouse.button == cen::mouse_button::left ||
       mouse.button == cen::mouse_button::right) &&
      is_available(model)) {
    auto& document = model.require_active_map();
    auto& map = document.get_map();

    const auto layerId = map.active_layer_id().value();
    auto&      layer = map.view_object_layer(layerId);

    const auto objectId = layer.object_at(mouse.pos);

    switch (mouse.button) {
      case cen::mouse_button::left: {
        layer.select_object(objectId);

        if (objectId) {
          const auto& object = layer.get_object(*objectId);

          auto& drag = mDragInfo.emplace();
          drag.origin_object_pos = object.get_pos();
          drag.last_mouse_pos = mouse.pos;
        }

        break;
      }
      case cen::mouse_button::right: {
        layer.select_object(objectId);

        if (objectId) {
          dispatcher.enqueue<SpawnObjectContextMenuEvent>(layerId, *objectId);
        }

        break;
      }
      default:
        break;
    }
  }
}

void ObjectSelectionTool::on_dragged(DocumentModel&    model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo&  mouse)
{
  if (mDragInfo && mouse.button == cen::mouse_button::left && is_available(model)) {
    auto& document = model.require_active_map();
    auto& map = document.get_map();

    const auto layerId = map.active_layer_id().value();
    auto&      layer = map.view_object_layer(layerId);

    if (const auto objectId = layer.active_object_id()) {
      auto& object = layer.get_object(*objectId);

      if (mouse.is_within_contents) {
        const auto& viewport = document.get_viewport();

        const auto ratio = viewport.get_scaling_ratio(map.tile_size());
        const auto delta = (mouse.pos - mDragInfo->last_mouse_pos) / ratio;

        object.set_pos(object.get_pos() + delta);
        mDragInfo->last_mouse_pos = mouse.pos;
      }
      else {
        // Stop if the user drags the object outside the map
        maybe_emit_event(model, dispatcher);
      }
    }
  }
}

void ObjectSelectionTool::on_released(DocumentModel&    model,
                                      entt::dispatcher& dispatcher,
                                      const MouseInfo&  mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto ObjectSelectionTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_map();
  return document.get_map().is_active_layer(LayerType::ObjectLayer);
}

void ObjectSelectionTool::maybe_emit_event(DocumentModel&    model,
                                           entt::dispatcher& dispatcher)
{
  const auto& document = model.require_active_map();
  const auto& map = document.get_map();

  if (mDragInfo) {
    if (const auto layerId = map.active_layer_id()) {
      const auto& layer = map.view_object_layer(*layerId);

      if (const auto objectId = layer.active_object_id()) {
        const auto& object = layer.get_object(*objectId);

        // Only emit an event if the object has been moved along any axis
        if (mDragInfo->origin_object_pos != object.get_pos()) {
          dispatcher.enqueue<MoveObjectEvent>(layer.get_uuid(),
                                              object.get_uuid(),
                                              mDragInfo->origin_object_pos,
                                              object.get_pos());
        }
      }
    }

    mDragInfo.reset();
  }
}

}  // namespace tactile