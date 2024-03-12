// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/fwd.hpp>
#include <entt/signal/fwd.hpp>

#include "tactile/base/prelude.hpp"

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
