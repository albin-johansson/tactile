// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>

#include "handlers/event_handlers.hpp"
#include "model/cmd/command_stack.hpp"
#include "model/event/command_events.hpp"
#include "model/model.hpp"
#include "runtime/app_context.hpp"

namespace tactile {
namespace {

void on_undo(const UndoEvent&)
{
  spdlog::trace("UndoEvent");

  if (auto* document = get_model().active_document()) {
    auto& commands = document->get_history();
    commands.undo();
  }
}

void on_redo(const RedoEvent&)
{
  spdlog::trace("RedoEvent");

  if (auto* document = get_model().active_document()) {
    auto& commands = document->get_history();
    commands.redo();
  }
}

void on_set_command_capacity(const SetCommandCapacityEvent event)
{
  spdlog::trace("SetCommandCapacityEvent(capacity: {})", event.capacity);
  get_model().set_command_capacity(event.capacity);
}

}  // namespace

void install_command_event_handler()
{
  auto& dispatcher = get_dispatcher();
  dispatcher.sink<UndoEvent>().connect<&on_undo>();
  dispatcher.sink<RedoEvent>().connect<&on_redo>();
  dispatcher.sink<SetCommandCapacityEvent>().connect<&on_set_command_capacity>();
}

}  // namespace tactile
