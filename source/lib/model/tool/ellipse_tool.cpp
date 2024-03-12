// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "ellipse_tool.hpp"

#include <entt/signal/dispatcher.hpp>

#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

void EllipseTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

void EllipseTool::on_disabled(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_exited(DocumentModel& model, entt::dispatcher& dispatcher)
{
  maybe_emit_event(model, dispatcher);
}

void EllipseTool::on_pressed(DocumentModel& model,
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

void EllipseTool::on_dragged(DocumentModel& model,
                             entt::dispatcher&,
                             const MouseInfo& mouse)
{
  if (mStroke && mouse.button == cen::mouse_button::left && is_available(model)) {
    mStroke->current = mouse.pos;
  }
}

void EllipseTool::on_released(DocumentModel& model,
                              entt::dispatcher& dispatcher,
                              const MouseInfo& mouse)
{
  if (mouse.button == cen::mouse_button::left && is_available(model)) {
    maybe_emit_event(model, dispatcher);
  }
}

void EllipseTool::maybe_emit_event(DocumentModel& model, entt::dispatcher& dispatcher)
{
  TACTILE_ASSERT(is_available(model));

  if (mStroke) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto& viewport = map_document.get_viewport();

    const auto ratio = viewport.scaling_ratio(vector_cast<float>(map.get_tile_size()));

    const auto radius = (mStroke->current - mStroke->start) / ratio;
    auto pos = mStroke->start / ratio;

    if (radius.x() < 0) {
      pos[0] += radius.x() * 2.0f;
    }

    if (radius.y() < 0) {
      pos[1] += radius.y() * 2.0f;
    }

    if (radius.x() != 0 && radius.y() != 0) {
      const auto layer_id = map.get_active_layer_id().value();
      const auto diameter = abs(radius) * 2.0f;
      dispatcher.enqueue<AddEllipseEvent>(layer_id, pos, diameter);
    }

    mStroke.reset();
  }
}

auto EllipseTool::is_available(const DocumentModel& model) const -> bool
{
  const auto& map_document = model.require_active_map_document();
  const auto& map = map_document.get_map();
  return map.is_active_layer(LayerType::ObjectLayer);
}

auto EllipseTool::get_stroke() const -> const Maybe<CurrentEllipseStroke>&
{
  return mStroke;
}

}  // namespace tactile
