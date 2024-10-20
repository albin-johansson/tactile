// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "object_selection_tool.hpp"

#include <entt/signal/dispatcher.hpp>

#include "core/layer/group_layer.hpp"
#include "core/layer/object_layer.hpp"
#include "model/document/map_document.hpp"
#include "model/event/object_events.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"

namespace tactile {

void ObjectSelectionTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void ObjectSelectionTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void ObjectSelectionTool::on_pressed(DocumentModel& model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  if ((mouse.button == cen::mouse_button::left ||
       mouse.button == cen::mouse_button::right) &&
      is_available(model)) {
    auto& document = model.require_active_map_document();
    auto& map = document.get_map();
    const auto& viewport = document.get_viewport();

    const auto layer_id = map.get_active_layer_id().value();
    auto& layer = map.get_invisible_root().get_object_layer(layer_id);

    const auto ratio = viewport.scaling_ratio(vector_cast<float>(map.get_tile_size()));
    const auto object_id =
        layer.object_at(mouse.pos / ratio, vector_cast<float>(map.get_tile_size()));

    switch (mouse.button) {
      case cen::mouse_button::left: {
        layer.select_object(object_id);

        if (object_id) {
          const auto& object = layer.get_object(*object_id);

          auto& drag = mDragInfo.emplace();
          drag.origin_object_pos = object.get_pos();
          drag.last_mouse_pos = mouse.pos;
        }

        break;
      }
      case cen::mouse_button::right: {
        layer.select_object(object_id);

        if (object_id) {
          dispatcher.enqueue<SpawnObjectContextMenuEvent>(*object_id);
        }

        break;
      }
      default:
        break;
    }
  }
}

void ObjectSelectionTool::on_dragged(DocumentModel& model,
                                     entt::dispatcher& dispatcher,
                                     const MouseInfo& mouse)
{
  if (mDragInfo && mouse.button == cen::mouse_button::left && is_available(model)) {
    auto& document = model.require_active_map_document();
    auto& map = document.get_map();

    const auto layer_id = map.get_active_layer_id().value();
    auto& layer = map.get_invisible_root().get_object_layer(layer_id);

    if (const auto object_id = layer.active_object_id()) {
      auto& object = layer.get_object(*object_id);

      if (mouse.is_within_contents) {
        const auto& viewport = document.get_viewport();

        const auto ratio =
            viewport.scaling_ratio(vector_cast<float>(map.get_tile_size()));
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

void ObjectSelectionTool::on_released(DocumentModel& model,
                                      entt::dispatcher& dispatcher,
                                      const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto ObjectSelectionTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  return map_document.get_map().is_active_layer(LayerType::ObjectLayer);
}

void ObjectSelectionTool::maybe_emit_event(DocumentModel& model,
                                           entt::dispatcher& dispatcher)
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();

  if (mDragInfo) {
    if (const auto layer_id = map.get_active_layer_id()) {
      const auto& layer = map.get_invisible_root().get_object_layer(*layer_id);

      if (const auto object_id = layer.active_object_id()) {
        const auto& object = layer.get_object(*object_id);

        // Only emit an event if the object has been moved along any axis
        if (mDragInfo->origin_object_pos != object.get_pos()) {
          dispatcher.enqueue<MoveObjectEvent>(object.get_uuid(),
                                              mDragInfo->origin_object_pos,
                                              object.get_pos());
        }
      }
    }

    mDragInfo.reset();
  }
}

}  // namespace tactile
