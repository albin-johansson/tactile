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

#include <centurion/fwd.hpp>

#include "core/ecs/entity.hpp"
#include "core/events/dispatcher.hpp"
#include "model/model_view.hpp"
#include "model/viewports/viewport_components.hpp"
#include "ui/dock/editor/map_editor_viewport.hpp"
#include "ui/dock/editor/tileset_editor_viewport.hpp"

namespace tactile {

struct EditorDockState final {
  MapEditorViewportState central_map_viewport;
  TilesetEditorViewportState tileset_viewport;
  bool is_focused {};
  bool is_hovered {};
};

void push_editor_dock_widget(ModelView& model, EditorDockState& state);

void on_mouse_wheel_event_in_central_viewport(Entity viewport_entity,
                                              const Viewport& viewport,
                                              Dispatcher& dispatcher,
                                              const cen::mouse_wheel_event& event);

}  // namespace tactile
