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

#include "tool_system.hpp"

#include <spdlog/spdlog.h>

#include "common/debug/assert.hpp"
#include "common/util/lookup.hpp"
#include "model/events/tool_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/tools/stamp_tool.hpp"
#include "model/tools/bucket_tool.hpp"
#include "model/tools/tool.hpp"

namespace tactile {

ToolSystem::ToolSystem(Registry& registry, Dispatcher& dispatcher)
    : System {registry, dispatcher}
{
  mTools[ToolType::Stamp] = std::make_unique<StampTool>();
  mTools[ToolType::Bucket] = std::make_unique<BucketTool>();
}

ToolSystem::~ToolSystem() noexcept = default;

void ToolSystem::reset()
{
  spdlog::trace("[ToolSystem] Resetting state");

  for (auto& [tool_type, tool]: mTools) {
    tool->reset();
  }

  mActiveToolType.reset();
  mActiveTool = nullptr;
}

void ToolSystem::install()
{
  // clang-format off
  auto& dispatcher = get_dispatcher();
  dispatcher.sink<SelectToolEvent>().connect<&ToolSystem::_on_select_tool>(this);
  dispatcher.sink<ViewportMousePressedEvent>().connect<&ToolSystem::_on_viewport_mouse_pressed>(this);
  dispatcher.sink<ViewportMouseDraggedEvent>().connect<&ToolSystem::_on_viewport_mouse_dragged>(this);
  dispatcher.sink<ViewportMouseReleasedEvent>().connect<&ToolSystem::_on_viewport_mouse_released>(this);
  dispatcher.sink<ViewportMouseEnteredEvent>().connect<&ToolSystem::_on_viewport_mouse_entered>(this);
  dispatcher.sink<ViewportMouseExitedEvent>().connect<&ToolSystem::_on_viewport_mouse_exited>(this);
  // clang-format on
}

auto ToolSystem::is_tool_available(const ToolType type) const -> bool
{
  return lookup_in(mTools, type)->is_available(get_registry());
}

void ToolSystem::_on_select_tool(const SelectToolEvent& event)
{
  spdlog::trace("[ToolSystem] SelectToolEvent");

  if (mActiveToolType == event.type) {
    return;
  }

  auto& registry = get_registry();
  auto& dispatcher = get_dispatcher();

  if (mActiveTool) {
    TACTILE_ASSERT(mActiveToolType.has_value());
    mActiveTool->on_deactivated(registry, dispatcher);
  }

  mActiveToolType = event.type;
  mActiveTool = lookup_in(mTools, *mActiveToolType).get();

  mActiveTool->on_activated(registry, dispatcher);
}

void ToolSystem::_on_viewport_mouse_pressed(const ViewportMousePressedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMousePressedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_pressed(get_registry(), get_dispatcher(), event.mouse_info);
  }
}

void ToolSystem::_on_viewport_mouse_dragged(const ViewportMouseDraggedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMouseDraggedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_dragged(get_registry(), get_dispatcher(), event.mouse_info);
  }
}

void ToolSystem::_on_viewport_mouse_released(const ViewportMouseReleasedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMouseReleasedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_released(get_registry(), get_dispatcher(), event.mouse_info);
  }
}

void ToolSystem::_on_viewport_mouse_entered(const ViewportMouseEnteredEvent&)
{
  spdlog::trace("[ToolSystem] ViewportMouseEnteredEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_entered(get_registry(), get_dispatcher());
  }
}

void ToolSystem::_on_viewport_mouse_exited(const ViewportMouseExitedEvent&)
{
  spdlog::trace("[ToolSystem] ViewportMouseExitedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_exited(get_registry(), get_dispatcher());
  }
}

}  // namespace tactile