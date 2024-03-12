// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

void update_component_dock(const DocumentModel& model, entt::dispatcher& dispatcher);

}  // namespace tactile::ui
