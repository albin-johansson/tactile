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

#include <string_view>  // string_view

#include "core/fwd.hpp"

namespace tactile::io {

/// Saves a map document.
void save_document(const MapDocument& document);

/**
 * Saves a map document as a Godot scene.
 *
 * Note, all directory arguments will have "res://" prepended to them.
 *
 * \param document the map document that will be exported.
 * \param projectMapDir absolute path in the Godot project to the map directory.
 * \param projectTilesetDir absolute path in the Godot project to the tileset directory.
 * \param projectImageDir absolute path in the Godot project to the image directory.
 * \param embedTilesets `true` if tilesets should be embedded in the map scene; `false` otherwise.
 *
 * \see https://docs.godotengine.org/en/stable/development/file_formats/tscn.html
 */
void export_document_as_godot_scene(const MapDocument& document,
                                    std::string_view   projectMapDir,
                                    std::string_view   projectTilesetDir,
                                    std::string_view   projectImageDir,
                                    bool               embedTilesets);

}  // namespace tactile::io
