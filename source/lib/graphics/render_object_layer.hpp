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

#include <centurion/fwd.hpp>

#include "common/macros.hpp"
#include "common/numeric.hpp"

TACTILE_FWD_DECLARE_CLASS_NS(tactile, ObjectLayer)
TACTILE_FWD_DECLARE_CLASS_NS(tactile, Object)

namespace tactile::ui {

TACTILE_FWD_DECLARE_CLASS(Graphics)

void render_object(Graphics& graphics, const Object& object, uint32 color);

void render_object_layer(Graphics& graphics,
                         const ObjectLayer& layer,
                         float parent_opacity);

}  // namespace tactile::ui
