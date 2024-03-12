// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/save/save_format_id.hpp"

#include "tactile/core/debug/exception.hpp"

namespace tactile {

auto to_string(const SaveFormatId format) -> StringView
{
  switch (format) {
    case SaveFormatId::kTactileYaml:
      return "Tactile YAML";

    case SaveFormatId::kTiledJson:
      return "Tiled JSON";

    case SaveFormatId::kTiledXml:
      return "Tiled XML";

    case SaveFormatId::kGodotScene:
      return "Godot scene";

    default:
      throw Exception {"bad save format"};
  }
}

auto get_file_extension(const SaveFormatId format) -> String
{
  switch (format) {
    case SaveFormatId::kTactileYaml:
      return ".yml";

    case SaveFormatId::kTiledJson:
      return ".tmj";

    case SaveFormatId::kTiledXml:
      return ".tmx";

    case SaveFormatId::kGodotScene:
      return ".escn";

    default:
      throw Exception {"bad save format"};
  }
}

auto has_valid_tactile_yaml_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".yml" || ext == ".yaml";
}

auto has_valid_tiled_json_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".json" || ext == ".tmj";
}

auto has_valid_tiled_xml_extension(const Path& path) -> bool
{
  const auto ext = path.extension();
  return ext == ".xml" || ext == ".tmx";
}

}  // namespace tactile
