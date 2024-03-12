// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

void update_map_viewport_toolbar(const DocumentModel& model,
                                 entt::dispatcher& dispatcher);

[[nodiscard]] auto is_map_toolbar_visible() -> bool;

[[nodiscard]] auto is_map_toolbar_hovered() -> bool;

[[nodiscard]] auto is_map_toolbar_focused() -> bool;

}  // namespace tactile::ui
