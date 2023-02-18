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

#include "common/type/string.hpp"

namespace tactile {

/// Represents the available save file formats.
/// The enumerator values must match the ones in the corresponding protobuf enum.
enum class SaveFormat {
  TactileYaml = 0,
  TiledJson = 1,
  TiledXml = 2
};

/// Returns a language-agnostic identifier for a save format identifier.
[[nodiscard]] auto get_human_readable_name(SaveFormat format) -> const char*;

/// Returns a valid file extension for a save format identifier.
/// Note, several file extensions are valid for most formats, e.g. ".yml" and ".yaml".
[[nodiscard]] auto get_file_extension(SaveFormat format) -> String;

// TODO move file path extension validation functions here

}  // namespace tactile
