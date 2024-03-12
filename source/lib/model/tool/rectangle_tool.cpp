// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rectangle_tool.hpp"

#include <entt/signal/dispatcher.hpp>

#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

void RectangleTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void RectangleTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void RectangleTool::on_pressed(DocumentModel& model,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && mouse.is_within_contents &&
      is_available(model)) {
    auto& stroke = mStroke.emplace();
    stroke.start = mouse.pos;
    stroke.current = stroke.start;
  }
}

void RectangleTool::on_dragged(DocumentModel& model,
                               entt::dispatcher&,
                               const MouseInfo& mouse)
{
  if (mStroke && mouse.button == cen::mouse_button::left && is_available(model)) {
    mStroke->current = mouse.pos;
  }
}

void RectangleTool::on_released(DocumentModel& model,
                                entt::dispatcher& dispatcher,
                                const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

auto RectangleTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

auto RectangleTool::get_stroke() const -> const Maybe<CurrentRectangleStroke>&
{
  return mStroke;
}

void RectangleTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (mStroke) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto& viewport = map_document.get_viewport();

    const auto ratio = viewport.scaling_ratio(vector_cast<float>(map.get_tile_size()));
    const auto pos = min(mStroke->start, mStroke->current) / ratio;
    const auto size = abs(mStroke->current - mStroke->start) / ratio;

    if (size.x() != 0 && size.y() != 0) {
      const auto layer_id = map.get_active_layer_id().value();
      dispatcher.enqueue<AddRectangleEvent>(layer_id, pos, size);
    }

    mStroke.reset();
  }
}

}  // namespace tactile
