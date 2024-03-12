// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

/// Updates the state of the main menu bar.
void update_menu_bar(const DocumentModel& model, entt::dispatcher& dispatcher);

}  // namespace tactile::ui
