// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <entt/fwd.hpp>

#include "tactile/core/util/uuid.hpp"

namespace tactile {
class DocumentModel;
}  // namespace tactile

namespace tactile::ui {

/// Updates the layer dock widget.
///
/// There must be an active map document when this function is called.
///
/// \param model the current document model.
/// \param dispatcher the event dispatcher used.
void update_layer_dock(const DocumentModel& model, entt::dispatcher& dispatcher);

/// Makes the dialog for renaming layers visible.
void show_rename_layer_dialog(const UUID& layer_id);

/// Indicates whether the layer dock widget has input focus.
[[nodiscard]] auto is_layer_dock_focused() -> bool;

}  // namespace tactile::ui
