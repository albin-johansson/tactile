// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/path.hpp"
#include "tactile/base/int.hpp"

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
