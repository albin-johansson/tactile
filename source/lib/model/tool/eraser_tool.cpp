// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "eraser_tool.hpp"

#include <utility>  // move

#include <centurion/mouse.hpp>
#include <entt/signal/dispatcher.hpp>

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "model/mouse_info.hpp"

namespace tactile {

void EraserTool::accept(ToolVisitor& visitor) const
{
  visitor.visit(*this);
}

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
  const auto& document = model.require_active_map_document();
  const auto& map = document.get_map();
  return map.is_active_layer(LayerType::TileLayer);
}

void EraserTool::update_sequence(DocumentModel& model, const TilePos& cursor)
{
  auto& map_document = model.require_active_map_document();
  auto& map = map_document.get_map();

  const auto layer_id = map.get_active_layer_id().value();
  auto& layer = map.get_invisible_root().get_tile_layer(layer_id);

  if (mPrevState.find(cursor) == mPrevState.end()) {
    mPrevState[cursor] = layer.tile_at(cursor).value();
  }

  layer.set_tile(cursor, kEmptyTile);
}

void EraserTool::maybe_emit_event(const DocumentModel& model,
                                  entt::dispatcher& dispatcher)
{
  if (!mPrevState.empty()) {
    const auto& map_document = model.require_active_map_document();
    const auto& map = map_document.get_map();
    const auto layer_id = map.get_active_layer_id().value();

    dispatcher.enqueue<EraserSequenceEvent>(layer_id, std::move(mPrevState));
    mPrevState.clear();
  }
}

}  // namespace tactile
