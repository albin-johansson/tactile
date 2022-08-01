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

#include "io/fwd.hpp"

namespace tactile {
class MapDocument;
}  // namespace tactile

namespace tactile::io {

struct GodotEmitOptions final
{
  std::string project_map_dir;      ///< Path to the map directory.
  std::string project_tileset_dir;  ///< Path to the tileset directory.
  std::string project_image_dir;    ///< Path to the image directory.
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
void emit_godot_map(const EmitInfo& info);

}  // namespace tactile::io
