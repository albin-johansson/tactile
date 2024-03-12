// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "io/map/parse/parse_error.hpp"
#include "tactile/base/prelude.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, DocumentModel)

namespace tactile::ui {

/// Updates the state of the GUI.
void update_widgets(const DocumentModel& model, entt::dispatcher& dispatcher);

void open_map_file_dialog();

void open_about_dear_imgui_dialog();

[[nodiscard]] auto is_editor_focused() -> bool;

}  // namespace tactile::ui
