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

#include "common/enum/attribute_type.hpp"
#include "common/enum/save_format.hpp"
#include "common/type/math.hpp"
#include "common/type/maybe.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"

namespace tactile {

/// Returns a language-agnostic identifier for a save format identifier.
[[nodiscard]] auto get_human_readable_name(SaveFormat format) -> StringView;

/// Returns a valid file extension for a save format identifier.
/// Note, several file extensions are valid for most formats, e.g. ".yml" and ".yaml".
[[nodiscard]] auto get_file_extension(SaveFormat format) -> String;

/// Indicates whether a file has one of the supported Tactile YAML file extensions.
[[nodiscard]] auto has_supported_tactile_yaml_extension(const Path& path) -> bool;

/// Indicates whether a file has one of the supported Tiled JSON file extensions.
[[nodiscard]] auto has_supported_tiled_json_extension(const Path& path) -> bool;

/// Indicates whether a file has one of the supported Tiled XML file extensions.
[[nodiscard]] auto has_supported_tiled_xml_extension(const Path& path) -> bool;

[[nodiscard]] auto serialize_to_save_format(const Int2& vec) -> String;
[[nodiscard]] auto serialize_to_save_format(const Float2& vec) -> String;
[[nodiscard]] auto serialize_to_save_format(const Int3& vec) -> String;
[[nodiscard]] auto serialize_to_save_format(const Float3& vec) -> String;
[[nodiscard]] auto serialize_to_save_format(const Int4& vec) -> String;
[[nodiscard]] auto serialize_to_save_format(const Float4& vec) -> String;

/// Returns the name of an attribute type for use in save files.
[[nodiscard]] auto serialize_to_save_format(AttributeType type) -> StringView;

/// Parses an attribute type from a type name used in save files.
[[nodiscard]] auto parse_attr_type(StringView name) -> Maybe<AttributeType>;

}  // namespace tactile
