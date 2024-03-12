// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <centurion/fwd.hpp>
#include <entt/fwd.hpp>

namespace tactile {
class DocumentModel;
class Viewport;
}  // namespace tactile

namespace tactile::ui {

/// Updates the state of the primary viewport widget.
void update_viewport_widget(const DocumentModel& model, entt::dispatcher& dispatcher);

void viewport_widget_mouse_wheel_event_handler(const Viewport& viewport,
                                               entt::dispatcher& dispatcher,
                                               const cen::mouse_wheel_event& event);

[[nodiscard]] auto is_viewport_focused() noexcept -> bool;

[[nodiscard]] auto is_mouse_within_viewport() noexcept -> bool;

}  // namespace tactile::ui
