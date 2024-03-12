// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <magic_enum.hpp>
#include <spdlog/spdlog.h>

#include "common/util/fmt.hpp"
#include "handlers/event_handlers.hpp"
#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"

namespace tactile {
namespace {

void on_select_tool(const SelectToolEvent& event)
{
  spdlog::trace("SelectToolEvent(type: {})", magic_enum::enum_name(event.type));

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    auto& tools = document->get_tools();
    tools.select_tool(event.type, model, get_dispatcher());
  }
}

void on_stamp_sequence(StampSequenceEvent event)
{
  spdlog::trace("StampSequenceEvent(layer_id: {})", event.layer_id);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->register_stamp_sequence(event.layer_id,
                                      std::move(event.old_state),
                                      std::move(event.sequence));
  }
}

void on_eraser_sequence(EraserSequenceEvent event)
{
  spdlog::trace("EraserSequenceEvent(layer_id: {})", event.layer_id);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->register_eraser_sequence(event.layer_id, std::move(event.old_state));
  }
}

void on_flood(const FloodEvent& event)
{
  spdlog::trace("FloodEvent(layer_id: {}, origin: {}, replacement: {})",
                event.layer_id,
                event.origin,
                event.replacement);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->flood(event.layer_id, event.origin, event.replacement);
  }
}

void on_add_rectangle(const AddRectangleEvent& event)
{
  spdlog::trace("AddRectangleEvent(layer_id: {}, pos: {}, size: {})",
                event.layer_id,
                event.pos,
                event.size);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->add_rectangle(event.layer_id, event.pos, event.size);
  }
}

void on_add_ellipse(const AddEllipseEvent& event)
{
  spdlog::trace("AddEllipseEvent(layer_id: {}, pos: {}, size: {})",
                event.layer_id,
                event.pos,
                event.size);

  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->add_ellipse(event.layer_id, event.pos, event.size);
  }
}

void on_add_point(const AddPointEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    document->add_point(event.layer_id, event.pos);
  }
}

void on_set_stamp_randomizer(const SetStampRandomizerEvent event)
{
  auto& model = get_model();
  if (auto* document = model.active_map_document()) {
    auto& tools = document->get_tools();
    tools.set_stamp_random_mode(event.enabled);
  }
}

}  // namespace

void install_tool_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<SelectToolEvent>().connect<&on_select_tool>();

  dispatcher.sink<StampSequenceEvent>().connect<&on_stamp_sequence>();
  dispatcher.sink<EraserSequenceEvent>().connect<&on_eraser_sequence>();
  dispatcher.sink<FloodEvent>().connect<&on_flood>();

  dispatcher.sink<AddRectangleEvent>().connect<&on_add_rectangle>();
  dispatcher.sink<AddEllipseEvent>().connect<&on_add_ellipse>();
  dispatcher.sink<AddPointEvent>().connect<&on_add_point>();

  dispatcher.sink<SetStampRandomizerEvent>().connect<&on_set_stamp_randomizer>();
}

}  // namespace tactile
