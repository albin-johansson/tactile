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

#include "save_format.hpp"

#include "common/debug/panic.hpp"

namespace tactile {

auto get_human_readable_name(const SaveFormat format) -> const char*
{
  switch (format) {
    case SaveFormat::TactileYaml:
      return "YAML";

    case SaveFormat::TiledJson:
      return "TMJ (JSON)";

    case SaveFormat::TiledXml:
      return "TMX (XML)";

    default:
      throw TactileError {"Invalid save format identifier"};
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
      throw TactileError {"Invalid save format identifier"};
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

}  // namespace tactile