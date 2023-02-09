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
#include "io/ir/ir.hpp"
#include "io/ir/godot/godot_scene.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_STRUCT(GodotEmitOptions)

[[nodiscard]] auto convert_to_godot(const ir::MapData& map,
                                    const GodotEmitOptions& options) -> GodotScene;

}  // namespace tactile
