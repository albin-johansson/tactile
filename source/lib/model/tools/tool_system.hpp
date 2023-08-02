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

#pragma once

#include "core/containers/hash_map.hpp"
#include "core/containers/smart_ptr.hpp"
#include "core/enums/tool_type.hpp"
#include "core/functional/maybe.hpp"
#include "core/prelude.hpp"
#include "model/system.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Tool)
TACTILE_FWD_DECLARE_STRUCT(SelectToolEvent)
TACTILE_FWD_DECLARE_STRUCT(ViewportMousePressedEvent)
TACTILE_FWD_DECLARE_STRUCT(ViewportMouseDraggedEvent)
TACTILE_FWD_DECLARE_STRUCT(ViewportMouseReleasedEvent)
TACTILE_FWD_DECLARE_STRUCT(ViewportMouseEnteredEvent)
TACTILE_FWD_DECLARE_STRUCT(ViewportMouseExitedEvent)
TACTILE_FWD_DECLARE_STRUCT(StampSequenceEvent)
TACTILE_FWD_DECLARE_STRUCT(FloodEvent)

class ToolSystem final : public System {
 public:
  ToolSystem();

  ~ToolSystem() noexcept override;

  void reset() override;

  void on_select_tool(Registry& registry,
                      Dispatcher& dispatcher,
                      const SelectToolEvent& event);

  void on_viewport_mouse_pressed(Registry& registry,
                                 Dispatcher& dispatcher,
                                 const ViewportMousePressedEvent& event);

  void on_viewport_mouse_dragged(Registry& registry,
                                 Dispatcher& dispatcher,
                                 const ViewportMouseDraggedEvent& event);

  void on_viewport_mouse_released(Registry& registry,
                                  Dispatcher& dispatcher,
                                  const ViewportMouseReleasedEvent& event);

  void on_viewport_mouse_entered(Registry& registry,
                                 Dispatcher& dispatcher,
                                 const ViewportMouseEnteredEvent& event);

  void on_viewport_mouse_exited(Registry& registry,
                                Dispatcher& dispatcher,
                                const ViewportMouseExitedEvent& event);

  void on_stamp_sequence(Registry& registry, const StampSequenceEvent& event);

  void on_flood(Registry& registry, const FloodEvent& event);

  [[nodiscard]] auto is_tool_available(const Registry& registry, ToolType type) const
      -> bool;

  [[nodiscard]] auto is_tool_selected(ToolType type) const -> bool;

 private:
  HashMap<ToolType, Unique<Tool>> mTools;
  Tool* mActiveTool {};
  Maybe<ToolType> mActiveToolType;
};

}  // namespace tactile
