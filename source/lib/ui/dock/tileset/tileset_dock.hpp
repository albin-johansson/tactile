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
#include <entt/signal/fwd.hpp>

#include "tactile/core/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, TilesetRef)

namespace tactile::ui {

void update_tileset_dock(const DocumentModel& model, entt::dispatcher& dispatcher);

void tileset_dock_mouse_wheel_event_handler(const TilesetRef& tileset_ref,
                                            const cen::mouse_wheel_event& event,
                                            entt::dispatcher& dispatcher);

[[nodiscard]] auto is_tileset_dock_focused() -> bool;

[[nodiscard]] auto is_tileset_dock_hovered() -> bool;

}  // namespace tactile::ui
