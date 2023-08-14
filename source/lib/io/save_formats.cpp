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

#include "save_formats.hpp"

#include <fmt/format.h>

#include "tactile/core/common/error.hpp"

namespace tactile {

auto get_human_readable_name(const SaveFormat format) -> StringView
{
  switch (format) {
    case SaveFormat::TactileYaml:
      return "YAML";

    case SaveFormat::TiledJson:
      return "TMJ (JSON)";

    case SaveFormat::TiledXml:
      return "TMX (XML)";

    default:
      throw Error {"Invalid save format identifier"};
  }
}

auto get_file_extension(const SaveFormat format) -> String
{
  switch (format) {
    case SaveFormat::TactileYaml:
      return ".yml";

    case SaveFormat::TiledJson:
      return ".tmj";

    case SaveFormat::TiledXml:
      return ".tmx";

    default:
      throw Error {"Invalid save format identifier"};
  }
}

auto has_supported_tactile_yaml_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".yml" || ext == ".yaml";
}

auto has_supported_tiled_json_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".json" || ext == ".tmj";
}

auto has_supported_tiled_xml_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".xml" || ext == ".tmx";
}

auto serialize_to_save_format(const Int2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize_to_save_format(const Float2& vec) -> String
{
  return fmt::format("{};{}", vec.x, vec.y);
}

auto serialize_to_save_format(const Int3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize_to_save_format(const Float3& vec) -> String
{
  return fmt::format("{};{};{}", vec.x, vec.y, vec.z);
}

auto serialize_to_save_format(const Int4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

auto serialize_to_save_format(const Float4& vec) -> String
{
  return fmt::format("{};{};{};{}", vec.x, vec.y, vec.z, vec.w);
}

auto serialize_to_save_format(const PropertyType type) -> StringView
{
  switch (type) {
    case PropertyType::Str:
      return "string";

    case PropertyType::Int:
      return "int";

    case PropertyType::Int2:
      return "int2";

    case PropertyType::Int3:
      return "int3";

    case PropertyType::Int4:
      return "int4";

    case PropertyType::Float:
      return "float";

    case PropertyType::Float2:
      return "float2";

    case PropertyType::Float3:
      return "float3";

    case PropertyType::Float4:
      return "float4";

    case PropertyType::Bool:
      return "bool";

    case PropertyType::Path:
      return "file";

    case PropertyType::Color:
      return "color";

    case PropertyType::Object:
      return "object";

    default:
      throw Error {"Invalid attribute type"};
  }
}

auto parse_attr_type(StringView name) -> Maybe<PropertyType>
{
  if (name == "string") {
    return PropertyType::Str;
  }
  else if (name == "int") {
    return PropertyType::Int;
  }
  else if (name == "int2") {
    return PropertyType::Int2;
  }
  else if (name == "int3") {
    return PropertyType::Int3;
  }
  else if (name == "int4") {
    return PropertyType::Int4;
  }
  else if (name == "float") {
    return PropertyType::Float;
  }
  else if (name == "float2") {
    return PropertyType::Float2;
  }
  else if (name == "float3") {
    return PropertyType::Float3;
  }
  else if (name == "float4") {
    return PropertyType::Float4;
  }
  else if (name == "bool") {
    return PropertyType::Bool;
  }
  else if (name == "file" || name == "path") {
    return PropertyType::Path;
  }
  else if (name == "color") {
    return PropertyType::Color;
  }
  else if (name == "object") {
    return PropertyType::Object;
  }
  else {
    return kNone;
  }
}

}  // namespace tactile