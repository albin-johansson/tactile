/*
 *  source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 *  program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 *  program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with  program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "tool_event_handler.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "model/document/map_document.hpp"
#include "model/event/tool_events.hpp"
#include "model/model.hpp"
#include "ui/app_context.hpp"

namespace tactile {
namespace {

void on_tool_pressed(const ToolPressedEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.on_pressed(model, get_dispatcher(), event.info);
  }
}

void on_tool_dragged(const ToolDraggedEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.on_dragged(model, get_dispatcher(), event.info);
  }
}

void on_tool_released(const ToolReleasedEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.on_released(model, get_dispatcher(), event.info);
  }
}

void on_tool_entered()
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.on_entered(model, get_dispatcher());
  }
}

void on_tool_exited()
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.on_exited(model, get_dispatcher());
  }
}

void on_select_tool(const SelectToolEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.select_tool(event.type, model, get_dispatcher());
  }
}

void on_stamp_sequence(StampSequenceEvent event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->register_stamp_sequence(event.layer_id,
                                      std::move(event.old_state),
                                      std::move(event.sequence));
  }
}

void on_eraser_sequence(EraserSequenceEvent event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->register_eraser_sequence(event.layer_id, std::move(event.old_state));
  }
}

void on_flood(const FloodEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->flood(event.layer_id, event.origin, event.replacement);
  }
}

void on_add_rectangle(const AddRectangleEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->add_rectangle(event.layer_id, event.pos, event.size);
  }
}

void on_add_ellipse(const AddEllipseEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->add_ellipse(event.layer_id, event.pos, event.size);
  }
}

void on_add_point(const AddPointEvent& event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    document->add_point(event.layer_id, event.pos);
  }
}

void on_set_stamp_randomizer(const SetStampRandomizerEvent event)
{
  auto& model = get_model();
  if (auto* document = model.active_map()) {
    auto& tools = document->get_tools();
    tools.set_stamp_random_mode(event.enabled);
  }
}

}  // namespace

void install_tool_event_handler()
{
  auto& dispatcher = get_dispatcher();

  dispatcher.sink<ToolPressedEvent>().connect<&on_tool_pressed>();
  dispatcher.sink<ToolDraggedEvent>().connect<&on_tool_dragged>();
  dispatcher.sink<ToolReleasedEvent>().connect<&on_tool_released>();
  dispatcher.sink<ToolEnteredEvent>().connect<&on_tool_entered>();
  dispatcher.sink<ToolExitedEvent>().connect<&on_tool_exited>();

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
