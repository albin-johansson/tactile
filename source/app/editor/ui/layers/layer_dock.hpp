/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include <entt/fwd.hpp>

#include "core/common/identifiers.hpp"
#include "core/fwd.hpp"

namespace tactile::ui {

/// \name Layer dock functions
/// \{

/**
 * \brief Updates the layer dock widget.
 *
 * \pre There must be an active map document when this function is called.
 *
 * \param model the current document model.
 * \param dispatcher the event dispatcher used.
 *
 * \ingroup gui
 */
void update_layer_dock(const DocumentModel& model, entt::dispatcher& dispatcher);

/**
 * \brief Makes the dialog for renaming layers visible.
 *
 * \param layerId the ID of the layer that will be renamed.
 *
 * \ingroup gui
 */
void show_rename_layer_dialog(LayerID layerId);

/**
 * \brief Indicates whether the layer dock widget has input focus.
 *
 * \return `true` if the layer dock is focused; `false` otherwise.
 *
 * \ingroup gui
 */
[[nodiscard]] auto is_layer_dock_focused() -> bool;

/// \} End of layer dock functions

}  // namespace tactile::ui
