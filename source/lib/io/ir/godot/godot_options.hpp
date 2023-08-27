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

#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/prelude.hpp"

namespace tactile {

/// Provides options for the Godot scene emitter.
///
/// Note, all directory arguments will have "res://" prepended to them, i.e. they are
/// considered to be relative to the Godot project root.
///
/// There are of course differences in how Godot and Tactile handle tilemaps, so some
/// transformations have to be made in order to export a Tactile map to be usable within
/// Godot.
///
/// There is no one true way to organize a Godot project, so we let the user specify how
/// their repository is arranged.
struct GodotEmitOptions final {
  Path root_dir;                      ///< Path to the local project directory.
  Path project_map_dir;               ///< Relative path to the map directory.
  Path project_image_dir;             ///< Relative path to the image directory.
  Path project_tileset_dir;           ///< Relative path to the tileset directory.
  usize ellipse_polygon_point_count;  ///< Amount of points in ellipse approximations.
};

}  // namespace tactile
