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

#include <string>  // string

#include "core/common/filesystem.hpp"
#include "io/fwd.hpp"

namespace tactile {
class MapDocument;
}  // namespace tactile

namespace tactile::io {

/**
 * Provides options for the Godot scene emitter.
 *
 * Note, all directory arguments will have "res://" prepended to them, i.e. they are
 * considered to be "project relative".
 *
 * There are of course differences in how Godot and Tactile handle tilemaps, so some
 * transformations have to be made in order to export a Tactile map to be usable within
 * Godot.
 *
 * The major differences include:
 * - Ellipses are not supported in Godot
 * - A `TileMap` in Godot is really just a tile layer, so we generate multiple such nodes
 * - A `TileMap` node in Godot may only have a single associated `TileSet`
 *
 * There is no one true way to organize a Godot project, so we let the user specify how
 * their repository is arranged.
 */
struct GodotEmitOptions final
{
  fs::path    root_dir;             ///< Path to the local project directory.
  fs::path    project_map_dir;      ///< Relative path to the map directory.
  fs::path    project_image_dir;    ///< Relative path to the image directory.
  fs::path    project_tileset_dir;  ///< Relative path to the tileset directory.
  std::string rectangle_node_type;  ///< Godot node used for rectangle objects.
  std::string ellipse_node_type;    ///< Godot node used for ellipse objects.
  std::string point_node_type;      ///< Godot node used for point objects.
  bool        embed_tilesets : 1 {};
};

/// Emits a map document, inferring the format from the path (or the preferred format)
void emit_map(const MapDocument& document);

/// Emits a map document as a Godot scene, see options struct for details.
void emit_map_as_godot_scene(const MapDocument&      document,
                             const GodotEmitOptions& options);

void emit_json_map(const EmitInfo& info);
void emit_xml_map(const EmitInfo& info);
void emit_yaml_map(const EmitInfo& info);
void emit_godot_map(const EmitInfo& info, const GodotEmitOptions& options);

}  // namespace tactile::io
