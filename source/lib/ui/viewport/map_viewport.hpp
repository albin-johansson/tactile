// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

namespace tactile {
class DocumentModel;
class MapDocument;
}  // namespace tactile

namespace tactile::ui {

void show_map_viewport(const DocumentModel& model,
                       const MapDocument& map,
                       entt::dispatcher& dispatcher);

void center_map_viewport();

void open_object_context_menu();

}  // namespace tactile::ui
