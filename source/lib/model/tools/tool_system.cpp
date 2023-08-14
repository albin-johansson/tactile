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

#include "cmd/tool/bucket_fill.hpp"
#include "cmd/tool/stamp_sequence.hpp"
#include "core/formatters/entity_formatter.hpp"
#include "core/formatters/tile_pos_formatter.hpp"
#include "model/documents/command_system.hpp"
#include "model/events/tool_events.hpp"
#include "model/events/viewport_events.hpp"
#include "model/tools/bucket_tool.hpp"
#include "model/tools/eraser_tool.hpp"
#include "model/tools/stamp_tool.hpp"
#include "model/tools/tool.hpp"
#include "tactile/core/common/assert.hpp"
#include "tactile/core/containers/lookup.hpp"

namespace tactile {

ToolSystem::ToolSystem()
{
  mTools[ToolType::Stamp] = std::make_unique<StampTool>();
  mTools[ToolType::Bucket] = std::make_unique<BucketTool>();
  mTools[ToolType::Eraser] = std::make_unique<EraserTool>();
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

void ToolSystem::on_select_tool(Registry& registry,
                                Dispatcher& dispatcher,
                                const SelectToolEvent& event)
{
  spdlog::trace("[ToolSystem] SelectToolEvent");

  if (mActiveToolType == event.type) {
    return;
  }

  if (mActiveTool) {
    TACTILE_ASSERT(mActiveToolType.has_value());
    mActiveTool->on_deactivated(registry, dispatcher);
  }

  mActiveToolType = event.type;
  mActiveTool = lookup_in(mTools, *mActiveToolType).get();

  mActiveTool->on_activated(registry, dispatcher);
}

void ToolSystem::on_viewport_mouse_pressed(Registry& registry,
                                           Dispatcher& dispatcher,
                                           const ViewportMousePressedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMousePressedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_pressed(registry, dispatcher, event.mouse_info);
  }
}

void ToolSystem::on_viewport_mouse_dragged(Registry& registry,
                                           Dispatcher& dispatcher,
                                           const ViewportMouseDraggedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMouseDraggedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_dragged(registry, dispatcher, event.mouse_info);
  }
}

void ToolSystem::on_viewport_mouse_released(Registry& registry,
                                            Dispatcher& dispatcher,
                                            const ViewportMouseReleasedEvent& event)
{
  spdlog::trace("[ToolSystem] ViewportMouseReleasedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_released(registry, dispatcher, event.mouse_info);
  }
}

void ToolSystem::on_viewport_mouse_entered(Registry& registry,
                                           Dispatcher& dispatcher,
                                           const ViewportMouseEnteredEvent&)
{
  spdlog::trace("[ToolSystem] ViewportMouseEnteredEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_entered(registry, dispatcher);
  }
}

void ToolSystem::on_viewport_mouse_exited(Registry& registry,
                                          Dispatcher& dispatcher,
                                          const ViewportMouseExitedEvent&)
{
  spdlog::trace("[ToolSystem] ViewportMouseExitedEvent");
  if (mActiveTool) {
    mActiveTool->on_mouse_exited(registry, dispatcher);
  }
}

void ToolSystem::on_stamp_sequence(Registry& registry, const StampSequenceEvent& event)
{
  spdlog::trace("[ToolSystem] StampSequenceEvent (layer: {})", event.layer);

  // TODO no need to execute it, only store it.
  sys::try_execute<cmd::StampSequence>(registry,
                                       event.layer,
                                       event.old_state,
                                       event.sequence);
}

void ToolSystem::on_flood(Registry& registry, const FloodEvent& event)
{
  spdlog::trace("[ToolSystem] FloodEvent (layer: {} origin: {} replacement: {})",
                event.layer,
                event.origin,
                event.replacement);

  sys::try_execute<cmd::BucketFill>(registry,
                                    event.layer,
                                    event.origin,
                                    event.replacement);
}

auto ToolSystem::is_tool_available(const Registry& registry, const ToolType type) const
    -> bool
{
  return lookup_in(mTools, type)->is_available(registry);
}

auto ToolSystem::is_tool_selected(const ToolType type) const -> bool
{
  return mActiveToolType == type;
}

}  // namespace tactile