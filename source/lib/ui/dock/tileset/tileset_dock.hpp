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

#include "common/type/dispatcher.hpp"
#include "common/type/ecs.hpp"
#include "components/tileset.hpp"
#include "model/model.hpp"

namespace tactile::ui {

void show_tileset_dock(const Model& model, Entity widget_entity, Dispatcher& dispatcher);

[[nodiscard]] auto is_tileset_dock_enabled(const Model& model) -> bool;

void tileset_dock_mouse_wheel_event_handler(const Model& model,
                                            Entity attached_tileset_entity,
                                            const cen::mouse_wheel_event& event,
                                            Dispatcher& dispatcher);

[[nodiscard]] auto is_tileset_dock_focused() -> bool;

[[nodiscard]] auto is_tileset_dock_hovered() -> bool;

}  // namespace tactile::ui
