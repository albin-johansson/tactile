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

namespace tactile {

class DocumentModel;
class IconManager;

/// \name Viewport widget functions
/// \{

/**
 * \brief Updates the state of the primary viewport widget.
 *
 * \param model the current document model.
 * \param icons the loaded icons.
 * \param dispatcher the event dispatcher that will be used.
 */
void update_viewport_widget(const DocumentModel& model,
                            const IconManager& icons,
                            entt::dispatcher& dispatcher);

/**
 * \brief Centers the main viewport over the current map.
 */
void center_viewport();

[[nodiscard]] auto is_viewport_focused() noexcept -> bool;

[[nodiscard]] auto is_mouse_within_viewport() noexcept -> bool;

/// \} End of viewport widget functions

}  // namespace tactile
