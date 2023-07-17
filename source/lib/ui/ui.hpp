/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/macros.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, ModelView)
TACTILE_FWD_DECLARE_STRUCT_NS(tactile::ui, WidgetState)

namespace tactile::ui {

void poll_global_shortcuts(ModelView& model);

/**
 * Renders the user interface and enqueues user events as needed.
 *
 * \details The UI doesn't directly modify the model. Instead, events are issued using an
 *          event dispatcher bundled with the model view instance. The UI only ever
 *          directly modifies state not directly related to the core model, this
 *          corresponds to the widget state instance.
 *
 * \param model   a view into the associated model.
 * \param widgets dynamic state used by miscellaneous widgets.
 */
void render_ui(ModelView& model, WidgetState& widgets);

}  // namespace tactile::ui
