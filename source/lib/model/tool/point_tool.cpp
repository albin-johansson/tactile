// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "point_tool.hpp"

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"

namespace tactile {

void PointTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void PointTool::on_pressed(DocumentModel& model,
                           entt::dispatcher& dispatcher,
                           const MouseInfo& mouse)
{
  if (mouse.is_within_contents && mouse.button == cen::mouse_button::left &&
      is_available(model)) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto& viewport = map_document.get_viewport();

    const auto ratio = viewport.scaling_ratio(vector_cast<float>(map.get_tile_size()));
    const auto pos = mouse.pos / ratio;

    const auto layer_id = map.get_active_layer_id().value();
    dispatcher.enqueue<AddPointEvent>(layer_id, pos);
  }
}

auto PointTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

}  // namespace tactile
